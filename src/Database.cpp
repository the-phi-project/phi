/*

 PHI
 2025/09/19

 Phi C++ Project
 src/Database.cpp

 Zevi Berlin

*/

#include "database/Database.hpp"

#include <mutex>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

#include <SQLiteCpp/SQLiteCpp.h>

#include "database/structs.hpp"
#include "utils/file_utils.hpp"

//---------> [ Config. Separator ] <---------\\ 

std::unique_lock<std::mutex> phi::database::Database::checkContact(
  std::unique_lock<std::mutex>&& lock, int contact_id, bool& exists) {
  this->contact_check_query->bind(":id", contact_id);
  exists = this->contact_check_query->executeStep();
  this->contact_check_query->reset();

  return std::move(lock);
}

/*::::::::::::::::::::::::::::::::::::::*\
|*:::::::::[ Access Separator ]:::::::::*|
\*::::::::::::::::::::::::::::::::::::::*/

phi::database::Database::Database() {
  this->createTables();

  /**/

  SQLite::Statement getSelf(this->db, "SELECT * FROM self WHERE id = 1");
  if (!getSelf.executeStep()) {
    this->createSelf();
  } else {
    this->self.rsa2048_priv = fromB64(getSelf.getColumn("rsa2048_priv").getString());
    this->self.rsa2048_pub = fromB64(getSelf.getColumn("rsa2048_pub").getString());
    this->self.rsa4096_priv = fromB64(getSelf.getColumn("rsa4096_priv").getString());
    this->self.rsa4096_pub = fromB64(getSelf.getColumn("rsa4096_pub").getString());
    this->self.kyber512_pub = fromB64(getSelf.getColumn("kyber512_pub").getString());
    this->self.kyber512_priv = fromB64(getSelf.getColumn("kyber512_priv").getString());
    this->self.kyber768_pub = fromB64(getSelf.getColumn("kyber768_pub").getString());
    this->self.kyber768_priv = fromB64(getSelf.getColumn("kyber768_priv").getString());
  }
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::database::Database::createTables() {
  this->db.exec(
    R"sql(
      CREATE TABLE IF NOT EXISTS self (
        id INTEGER PRIMARY KEY,
        rsa2048_pub TEXT,
        rsa2048_priv TEXT,
        rsa4096_pub TEXT,
        rsa4096_priv TEXT,
        kyber512_pub TEXT,
        kyber512_priv TEXT,
        kyber768_pub TEXT,
        kyber768_priv TEXT
      );

      CREATE TABLE IF NOT EXISTS contacts (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT DEFAULT "Contact",
        rsa2048 TEXT,
        rsa4096 TEXT,
        kyber512 TEXT,
        kyber768 TEXT,
        UNIQUE(name)
      );
    )sql");

  this->contact_check_query =
    std::make_unique<SQLite::Statement>(this->db, "SELECT 1 FROM contacts WHERE id = :id");
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::database::Database::createSelf() {
  this->db.exec("INSERT INTO self (id) VALUES (1)");
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::database::Database::updateSelf(phi::database::self_t& new_self) {
  std::lock_guard<std::mutex> lock(this->mtx);

  std::unordered_map<std::string, std::string> updatedFields;

  for (const auto& [field, ptr] : this->self.MAP) {
    if (*ptr != *(new_self.MAP[field])) {
      updatedFields[field] = *(new_self.MAP[field]);
    }
  }

  for (const auto& [field, val] : updatedFields) {
    SQLite::Statement change(this->db, "UPDATE self SET " + field + " = :val WHERE id = 1");
    change.bind(":val", toB64(val));
    change.exec();
  }

  this->self = new_self;
}

//------------[ Func. Implementation Separator ]------------\\ 

bool phi::database::Database::createContact(const std::string& name, int& erc, int& op_id) {
  std::lock_guard<std::mutex> lock(this->mtx);
  op_id = -1;

  SQLite::Statement check(this->db, "SELECT 1 FROM contacts WHERE name = :name");
  check.bind(":name", name);
  if (check.executeStep()) {
    erc = 1;
    return false;
  }

  SQLite::Statement create(this->db, "INSERT INTO contacts (name) VALUES (:name) RETURNING id");
  create.bind(":name", name);

  if (!create.executeStep()) {
    erc = 2;
    return false;
  }

  op_id = create.getColumn("id").getInt();
  return true;
}

//------------[ Func. Implementation Separator ]------------\\ 

std::vector<std::tuple<int, std::string>> phi::database::Database::getAllContacts() {
  std::lock_guard<std::mutex> lock(this->mtx);

  int rows = this->db.execAndGet("SELECT COUNT(id) FROM contacts").getInt();
  if (rows == 0) return {};

  auto contacts = std::vector<std::tuple<int, std::string>>(rows);

  SQLite::Statement get_contacts(this->db, "SELECT id, name FROM contacts");
  int idx = 0;
  while (get_contacts.executeStep() && idx < rows) {
    contacts[idx] = std::make_tuple<int, std::string>(get_contacts.getColumn("id").getInt(),
                                                      get_contacts.getColumn("name").getString());
    idx++;
  }

  return contacts;
}

//------------[ Func. Implementation Separator ]------------\\ 

bool phi::database::Database::getContact(int contact_id, phi::database::contact_t& op) {
  std::lock_guard<std::mutex> lock(this->mtx);

  SQLite::Statement query(this->db, "SELECT * FROM contacts WHERE id = :id");
  query.bind(":id", contact_id);
  if (!query.executeStep()) {
    return false;
  }

  op.id = contact_id;
  op.name = query.getColumn("name").getString();
  op.rsa2048 = fromB64(query.getColumn("rsa2048").getString());
  op.rsa4096 = fromB64(query.getColumn("rsa4096").getString());
  op.kyber512 = fromB64(query.getColumn("kyber512").getString());
  op.kyber768 = fromB64(query.getColumn("kyber768").getString());

  return true;
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::database::Database::updateContact(phi::database::contact_t& current,
                                            phi::database::contact_t& to_set) {
  std::lock_guard<std::mutex> lock(this->mtx);

  std::unordered_map<std::string, std::string> updatedFields;
  for (const auto& [field, ptr] : current.MAP) {
    if (*ptr != *(to_set.MAP[field])) {
      if (field != "name") {
        updatedFields[field] = toB64(*(to_set.MAP[field]));
      } else {
        updatedFields[field] = *(to_set.MAP[field]);
      }
    }
  }

  for (const auto& [field, val] : updatedFields) {
    SQLite::Statement change(this->db, "UPDATE contacts SET " + field + " = :val WHERE id = :id");
    change.bind(":val", val);
    change.bind(":id", current.id);
    change.exec();
  }
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::database::Database::deleteContact(int contact_id) {
  std::lock_guard<std::mutex> lock(this->mtx);

  this->db.exec("DELETE FROM contacts WHERE id = " + std::to_string(contact_id));
}