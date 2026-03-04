/*

 PHI
 2026/03/03

 Phi C++ Project
 src/parser_encrypt.cpp

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
constexpr int ENCRYPTION_FAILURE = 4;

//=====[ Declaration Separator ]=====\\ 

int encrypt_message(const cxxopts::ParseResult& result,
                    const std::shared_ptr<phi::database::Database>& DATABASE,
                    phi::database::contact_t& contact);

//---------> [ Config. Separator ] <---------\\ 

int phi::parser::parseEncryptRequest(const cxxopts::ParseResult& result,
                                     const std::shared_ptr<phi::database::Database>& DATABASE) {
  phi::database::contact_t contact{};
  if (result.contains("contact-id")) {
    int contact_id = result["contact-id"].as<int>();
    if (!DATABASE->getContact(contact_id, contact)) {
      std::cout << RED << "No contact with ID `" << ITL << contact_id << RST << RED << "`\n" << RST;
      return CONTACT_ERROR;
    }
  } else {
    std::cout << RED << "Encryption operation requires a contact ID\n" << RST;
    return USAGE_FAILURE;
  }

  /**/

  if (!(result.contains("symmetric") && result.contains("asymmetric"))) {
    std::cout
      << RED
      << "Encryption requires two modes, one for asymmetric and one for symmetric encryption\n"
      << RST;
    return USAGE_FAILURE;
  }

  const std::string symmetric = result["symmetric"].as<std::string>();
  const std::string asymmetric = result["asymmetric"].as<std::string>();

  if (contact.MAP.find(asymmetric) == contact.MAP.end()) {
    std::cout << RED << "Asymmetric encryption mode `" << ITL << asymmetric << RST << RED
              << "` is not available\n"
              << RST;
    return USAGE_FAILURE;
  }

  if (!validateGivenKeyContact(asymmetric, *(contact.MAP[asymmetric]))) {
    std::cout << RED << "Contact does not have a valid " << asymmetric << " public key\n" << RST;
    return CONTACT_ERROR;
  }

  if (result.contains("str") == result.contains("in-file")) {
    std::cout
      << RED
      << "Encryption requires input from either a string (--str) or from a file (--in-file)\n"
      << RST;
    return USAGE_FAILURE;
  }

  return encrypt_message(result, DATABASE, contact);
}

//------------[ Func. Implementation Separator ]------------\\ 

int encrypt_message(const cxxopts::ParseResult& result,
                    const std::shared_ptr<phi::database::Database>& DATABASE,
                    phi::database::contact_t& contact) {
  //
}