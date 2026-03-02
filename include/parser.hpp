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

/*
Create the Options object that outlines all possible
 command line arguments, checkout documentation/phi.1
*/
cxxopts::Options createOptions();

//================={ Header Item Separator }=================\\ 

/*
Small function to parse arguments and sort into the
 catch functions below
*/
int parseArguments(cxxopts::Options& options, int argc, char** argv,
                   const std::shared_ptr<phi::database::Database>& DATABASE);

//================={ Header Item Separator }=================\\ 

int parseContactRequest(const cxxopts::ParseResult& result,
                        const std::shared_ptr<phi::database::Database>& DATABASE);

//================={ Header Item Separator }=================\\ 

int parseEncryptRequest(const cxxopts::ParseResult& result,
                        const std::shared_ptr<phi::database::Database>& DATABASE);

//================={ Header Item Separator }=================\\ 

int parseDecryptRequest(const cxxopts::ParseResult& result,
                        const std::shared_ptr<phi::database::Database>& DATABASE);


}  // namespace phi::parser


#endif /* PARSER_HPP */
