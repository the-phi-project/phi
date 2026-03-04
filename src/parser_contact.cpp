/*

 PHI
 2026/03/03

 Phi C++ Project
 src/parser_contact.cpp

 Zevi Berlin

*/

#include "parser.hpp"

namespace tmc = termcolor;

#define RED tmc::red
#define ITL tmc::italic
#define BOLD tmc::bold
#define RST tmc::reset

// error codes
constexpr int SUCCESS = 0;
constexpr int USAGE_FAILURE = 1;
constexpr int FILE_ERROR = 2;
constexpr int CONTACT_ERROR = 6;

//=====[ Declaration Separator ]=====\\ 

int list_contacts(const cxxopts::ParseResult& result,
                  const std::shared_ptr<phi::database::Database>& DATABASE);

int get_contact(const cxxopts::ParseResult& result,
                const std::shared_ptr<phi::database::Database>& DATABASE,
                const phi::database::contact_t& contact);

int edit_contact(const cxxopts::ParseResult& result,
                 const std::shared_ptr<phi::database::Database>& DATABASE,
                 phi::database::contact_t& contact);

//---------> [ Config. Separator ] <---------\\ 

int phi::parser::parseContactRequest(const cxxopts::ParseResult& result,
                                     const std::shared_ptr<phi::database::Database>& DATABASE) {
  size_t arg_count =
    result.count("get") + result.count("edit") + result.count("delete") + result.count("list");
  if (arg_count != 1) {
    std::cout << RED << "Contact request must contain exactly ONE parameter\n" << RST;
    return USAGE_FAILURE;
  }

  /**/

  int contact_id = -1;
  phi::database::contact_t contact{};

  /**/

  if (result.contains("list")) {
    return list_contacts(result, DATABASE);
    /**/
  } else {  // NOLINT otherwise we should load contact based on ID
    contact_id = result["contact-id"].as<int>();
    if (!DATABASE->getContact(contact_id, contact)) {
      std::cout << RED << "No contact with ID `" << ITL << contact_id << RST << RED << "`\n" << RST;
      return CONTACT_ERROR;
    }
  }

  /**/

  if (result.contains("get")) {
    return get_contact(result, DATABASE, contact);
    /**/
  } else if (result.contains("edit")) {
    return edit_contact(result, DATABASE, contact);
    /**/
  } else if (result.contains("delete")) {
    DATABASE->deleteContact(contact_id);
  }

  return SUCCESS;
}

//
//
//------------[ Func. Implementation Separator ]------------\\ 
// HELPER FUNCTIONS
//

int list_contacts(const cxxopts::ParseResult& result,
                  const std::shared_ptr<phi::database::Database>& DATABASE) {
  std::vector<std::tuple<int, std::string>> contacts = DATABASE->getAllContacts();

  std::stringstream medium;

  medium << "{ ";
  for (size_t i = 0; i < contacts.size() - 1; i++) {
    medium << "{" << std::get<0>(contacts[i]) << ": \"" << std::get<1>(contacts[i]) << "\"}, ";
  }
  medium << "{" << std::get<0>(contacts[contacts.size() - 1]) << ": \""
         << std::get<1>(contacts[contacts.size() - 1]) << "\"} }\n";

  if (result.contains("out-file")) {
    const std::string path = result["out-file"].as<std::string>();
    std::ofstream file(path);
    if (!file) {
      std::cout << RED << "Unable to write to `" << ITL << path << RST << RED << "`\n" << RST;
      return FILE_ERROR;
    }

    file << medium.rdbuf();
  } else {
    std::cout << medium.rdbuf();
  }

  return SUCCESS;
}

//------------[ Func. Implementation Separator ]------------\\ 

int get_contact(const cxxopts::ParseResult& result,
                const std::shared_ptr<phi::database::Database>& DATABASE,
                const phi::database::contact_t& contact) {
  if (result.contains("out-file")) {
    const std::string path = result["out-file"].as<std::string>();
    std::ofstream file(path);
    if (!file) {
      std::cout << RED << "Unable to write to `" << ITL << path << RST << RED << "`\n" << RST;
      return FILE_ERROR;
    }

    file << contact.to_json_str() << "\n";
  } else {
    std::cout << contact.to_json_str() << "\n";
  }

  return SUCCESS;
}

//------------[ Func. Implementation Separator ]------------\\ 

int edit_contact(const cxxopts::ParseResult& result,
                 const std::shared_ptr<phi::database::Database>& DATABASE,
                 phi::database::contact_t& contact) {
  std::string field, value;
  if (result.contains("field")) field = result["field"].as<std::string>();
  if (result.contains("value")) value = result["value"].as<std::string>();

  if (field.empty() || value.empty()) {
    std::cout << RED << "Edit request must contain a non-empty field and a value\n" << RST;
    return USAGE_FAILURE;
  }

  phi::database::contact_t editable = contact;  // custom copy constructor to deal with MAP ptrs
  if (field == "id") {
    try {
      editable.id = std::stoi(value);
      /**/
    } catch (const std::invalid_argument& exc) {
      std::cout << RED << "Invalid argument `" << ITL << value << RST << RED
                << "`, ID must be an integer\n"
                << RST;
      return USAGE_FAILURE;
    }
    /**/
  } else {
    if (contact.MAP.find(field) != contact.MAP.end()) {  // key exists
      std::string decoded = fromB64(value);
      if (!validateGivenKeyContact(field, decoded)) {
        std::cout << RED << "Invalid " << field
                  << " public key; must be a base64 chunk without guards\n"
                  << RST;
        return USAGE_FAILURE;
      }

      *(editable.MAP[field]) = decoded;  // MAP is {const std::string, *std::string}

      /**/

      DATABASE->updateContact(contact, editable);
    } else {
      std::cout << RED << "Field `" << ITL << field << RST << RED << "` does not exist\n" << RST;
      return USAGE_FAILURE;
    }
  }

  return SUCCESS;
}