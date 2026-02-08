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
#include "database/Database.hpp"
#include "database/structs.hpp"
#include "utils/str_utils.hpp"

namespace tmc = termcolor;

//---------> [ Config. Separator ] <---------\\ 

namespace phi {

int parse_commands(int argc, char** argv, const std::shared_ptr<phi::database::Database>& DATABASE);

}


#endif /* PARSER_HPP */
