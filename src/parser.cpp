/*

 PHI
 2026/02/04

 Phi C++ Project
 src/parser.cpp

 Zevi Berlin

*/

#include "parser.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <memory>
#include <fstream>

#include "termcolor/termcolor.hpp"
#include "cxxopts/cxxopts.hpp"

#include "encryption/asymmetric/rsa.hpp"
#include "encryption/asymmetric/kyber.hpp"
#include "encryption/symmetric/aes.hpp"
#include "encryption/symmetric/ccp.hpp"
#include "database/Database.hpp"
#include "database/structs.hpp"
#include "utils/str_utils.hpp"

namespace tmc = termcolor;

#define RED tmc::red
#define ITL tmc::italic
#define BOLD tmc::bold
#define RST tmc::reset

//---------> [ Config. Separator ] <---------\\ 

int phi::parse_commands(int argc, char** argv,
                        const std::shared_ptr<phi::database::Database>& DATABASE) {
  try {
    cxxopts::Options options("phi",
                             R"desc(Quantum-Safe TUI Encryption/Decryption Tool with a Contact Book
      https://github.com/the-phi-project/phi
      )desc");

    options.custom_help(
      R"help([MODE] [FILE?] [--asymmetric TYPE] [--symmetric TYPE] [--contact-id ID] [FILE?])help");

    /*
      The stuff below is called method chaining. The original call to `add_options()` returns
       an object which is then called to add an option so when the parentheses are chained
       together it calls the function multiple times and adds mutliple options
    */
    options.add_options()("h,help", "Open the help menu")                  //
      ("list-contacts", "Print all contacts in the format `(id) name`\n")  //
      ("encrypt", "Enter encryption MODE for a message")                   //
      ("decrypt", "Enter decryption MODE for a message\n")                 //
      ("in-file", "(OPTIONAL, default stdin) Input FILEpath to encrypt/decrypt",
       cxxopts::value<std::string>())  //
      ("out-file", "(OPTIONAL, default stdout) Output FILEpath for encrypted/decrypted message\n",
       cxxopts::value<std::string>())  //
      ("asymmetric",
       "TYPE of asymmetric encryption/decryption, any {rsa2048, rsa4096, kyber512, kyber768}",
       cxxopts::value<std::string>())  //
      ("symmetric",
       "TYPE of symmetric encryption/decryption, any {aes128, aes192, aes256, chacha20_poly1305}\n",
       cxxopts::value<std::string>())  //
      ("contact-id", "Which contact's keys to use, by ID", cxxopts::value<int>());

    auto result = options.parse(argc, argv);

    if (result.contains("help")) {
      std::cout << options.help() << "\n";
    } else if (result.contains("list-contacts")) {
      std::vector<std::tuple<int, std::string>> contacts = DATABASE->getAllContacts();
      for (const auto& contact : contacts) {
        std::cout << "(" << std::get<0>(contact) << ") " << std::get<1>(contact) << "\n";
      }
    }
  } catch (const cxxopts::exceptions::exception& exc) {
    std::cout << RED << "Error parsing program arguments: " << ITL << exc.what() << RST << "\n";
    return 1;
  }

  return 0;
}