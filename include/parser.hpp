/*

 PHI
 2026/02/04

 Phi C++ Project
 include/parser.hpp

 Zevi Berlin

*/

#ifndef PARSER_HPP
#define PARSER_HPP

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
#include "encryption/hashing.hpp"
#include "encryption/gzip.hpp"
#include "database/Database.hpp"
#include "database/structs.hpp"
#include "utils/str_utils.hpp"

namespace tmc = termcolor;

//---------> [ Config. Separator ] <---------\\ 

namespace phi {

static const std::vector<std::string> asymtypes{"rsa2048", "rsa4096", "kyber512", "kyber768"};

static const std::vector<std::string> symtypes{"aes128", "aes192", "aes256", "chacha20_poly1305"};

//=====[ Declaration Separator ]=====\\ 

static cxxopts::Options createOptions() {
  cxxopts::Options options("phi",
                           R"desc(Quantum-Safe TUI Encryption/Decryption Tool with a Contact Book
https://github.com/the-phi-project/phi
)desc");

  options.custom_help(
    R"help([MODE] [FILE or B64 STR] [--asymmetric TYPE] [--symmetric TYPE] [--contact-id ID] [FILE?])help");

  /*
  The stuff below is called method chaining. The original call to `add_options()` returns
  an object which is then called to add an option so when the parentheses are chained
  together it calls the function multiple times and adds mutliple options
  */
  options.add_options()("h,help", "Open the help menu")                  //
    ("list-contacts", "Print all contacts in the format `(id) name`\n")  //
    ("encrypt", "Enter encryption MODE for a message")                   //
    ("decrypt", "Enter decryption MODE for a message\n")                 //
    ("in-file",
     "(OPTIONAL, default stdin) Input FILEpath to encrypt/decrypt (encrypted must be base64)",
     cxxopts::value<std::string>())  //
    ("str",
     "(OPTIONAL) If not using an input file, the encrypted message or message to encrypt goes "
     "here",
     cxxopts::value<std::string>())  //
    ("out-file", "(OPTIONAL, default stdout) Output FILEpath for encrypted/decrypted message\n",
     cxxopts::value<std::string>())  //
    ("asymmetric",
     "(Not necessary for decrypt mode) TYPE of asymmetric encryption, any {rsa2048, rsa4096, "
     "kyber512, kyber768}",
     cxxopts::value<std::string>())  //
    ("symmetric",
     "(Not necessary for decrypt mode) TYPE of symmetric encryption, any {aes128, aes192, "
     "aes256, "
     "chacha20_poly1305}\n",
     cxxopts::value<std::string>())  //
    ("contact-id", "Which contact's keys to use, by ID", cxxopts::value<int>());

  return options;
}

//================={ Header Item Separator }=================\\ 

int parseArguments(cxxopts::Options& options, int argc, char** argv,
                   const std::shared_ptr<phi::database::Database>& DATABASE);

//================={ Header Item Separator }=================\\ 

bool encryptMessage(const std::shared_ptr<phi::database::Database>& DATABASE, int contact_id,
                    const std::string& message, int symmode, int asymmode,
                    phi::database::message_t& op);


//================={ Header Item Separator }=================\\ 

bool decryptMessage(const std::shared_ptr<phi::database::Database>& DATABASE,
                    const phi::database::message_t& message, std::string& op);

}  // namespace phi


#endif /* PARSER_HPP */
