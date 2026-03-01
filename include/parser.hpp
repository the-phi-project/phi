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

namespace phi::parser {

// static const std::vector<std::string> asymtypes{"rsa2048", "rsa4096", "kyber512", "kyber768"};

// static const std::vector<std::string> symtypes{"aes128", "aes192", "aes256",
// "chacha20_poly1305"};

//=====[ Declaration Separator ]=====\\ 

static cxxopts::Options createOptions() {
  cxxopts::Options options("phi",
                           R"desc(Quantum-Safe TUI Encryption/Decryption Tool with a Contact Book
https://github.com/the-phi-project/phi
)desc");

  /*
  The stuff below is called method chaining. The original call to `add_options()` returns
  an object which is then called to add an option so when the parentheses are chained
  together it calls the function multiple times and adds mutliple options
  */
  options.add_options()("v,version", "Show the version")  //
    ("h,help", "Open the help menu")                      //
    ("list-contacts", "")                                 //
    ("e,encrypt", "")                                     //
    ("d,decrypt", "")                                     //
    ("i,in-file", "", cxxopts::value<std::string>())      //
    ("str", "", cxxopts::value<std::string>())            //
    ("o,out-file", "", cxxopts::value<std::string>())     //
    ("a,asymmetric", "", cxxopts::value<std::string>())   //
    ("s,symmetric", "", cxxopts::value<std::string>())    //
    ("c,contact-id", "", cxxopts::value<int>());

  return options;
}

//================={ Header Item Separator }=================\\ 

int parseArguments(cxxopts::Options& options, int argc, char** argv,
                   const std::shared_ptr<phi::database::Database>& DATABASE);

//================={ Header Item Separator }=================\\ 

}  // namespace phi::parser


#endif /* PARSER_HPP */
