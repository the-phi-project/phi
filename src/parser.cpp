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
#include "encryption/hashing.hpp"
#include "encryption/gzip.hpp"
#include "database/Database.hpp"
#include "database/structs.hpp"
#include "utils/str_utils.hpp"

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
constexpr int DECRYPTION_FAILURE = 5;

//---------> [ Config. Separator ] <---------\\ 

int phi::parser::parseArguments(cxxopts::Options& options, int argc, char** argv,
                                const std::shared_ptr<phi::database::Database>& DATABASE) {
  try {
    auto result = options.parse(argc, argv);

    if (result.contains("help")) {
      std::cout << "Use the command `" << ITL << "man phi" << RST << "` to access the man page.\n"
                << "If that fails or is outdated, download the man page at this link `" << ITL
                << "https://github.com/the-phi-project/phi/blob/main/documentation/phi.1" << RST
                << "` then\nmove it into wherever your OS keeps manpages."
                << "\n";
      return SUCCESS;
    }
    if (result.contains("version")) {
      std::cout << "Phi v" << PHI_VERSION << "\n";
      return SUCCESS;
    }


    if (result.contains("list-contacts")) {
      std::vector<std::tuple<int, std::string>> contacts = DATABASE->getAllContacts();
      for (const auto& contact : contacts) {
        std::cout << "(" << std::get<0>(contact) << ") " << std::get<1>(contact) << "\n";
      }
      return SUCCESS;
    }

    /*########## LONG FUNC ###########*\
    \*######## IN-SEPARATOR ##########*/

  } catch (const cxxopts::exceptions::exception& exc) {
    std::cout << RED << "Error parsing program arguments: " << ITL << exc.what() << RST << "\n";
    return USAGE_FAILURE;
  }

  return SUCCESS;
}

//------------[ Func. Implementation Separator ]------------\\ 
