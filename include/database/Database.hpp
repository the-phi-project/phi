/*

 PHI
 2025/09/19

 Phi C++ Project
 include/database/Database.hpp

 Zevi Berlin

*/

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <mutex>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

#include <SQLiteCpp/SQLiteCpp.h>

#include "database/structs.hpp"
#include "utils/file_utils.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace phi::database {

class Database {
  private:
    mutable std::mutex mtx;

    SQLite::Database db = SQLite::Database(expand("~/.phi/main.db"), SQLite::OPEN_READWRITE);

    // std::string password;

    std::unique_ptr<SQLite::Statement> contact_check_query;
    std::unique_lock<std::mutex> checkContact(std::unique_lock<std::mutex>&& lock, int contact_id,
                                              bool& exists);

  public:
    self_t self;

    Database();

    // bool login(const std::string& password);

    void createTables();

    // bool changePassword(const std::string& oldpass, const std::string& newpass,
    //                     const std::string& newhint);


    //~~~~~~~< Misc. Separator >~~~~~~~\\ 

    /*
      erc: 0 if none, 1 if self can't be created
    */
    void createSelf();

    /*
      erc: 0 if none, 1 if self can't be opened
    */
    void updateSelf(self_t& new_self);


    //~~~~~~~< Misc. Separator >~~~~~~~\\ 

    /*
      erc: 0 if none, 1 if name exists, 2 if unknown error

      op_id gets set to -1 if operation fails
    */
    bool createContact(const std::string& name, int& erc, int& op_id);

    /*
      returns a vector of tuples, the tuples being:

      {id, name} or a nullptr if there are no contacts
    */
    std::vector<std::tuple<int, std::string>> getAllContacts();

    bool getContact(int contact_id, contact_t& op);

    void updateContact(contact_t& current, contact_t& to_set);

    /*
      this will NOT give any indication as to whether or
      not the operation succeeded because errors aren't that important
    */
    void deleteContact(int contact_id);
};

}  // namespace phi::database

#endif /* DATABASE_HPP */
