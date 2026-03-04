/*

 PHI
 2026/02/04

 Phi C++ Project
 src/parser.cpp

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
constexpr int DECRYPTION_FAILURE = 5;

//---------> [ Config. Separator ] <---------\\ 

cxxopts::Options phi::parser::createOptions() {
  cxxopts::Options options("phi",
                           R"desc(Quantum-Safe TUI Encryption/Decryption Tool with a Contact Book
https://github.com/the-phi-project/phi
)desc");

  /*
  The stuff below is called method chaining. The original call to `add_options()` returns
  an object which is then called to add an option so when the parentheses are chained
  together it calls the function multiple times and adds mutliple options
  */
  options.add_options()                // '//' for line separation
    ("v,version", "Show the version")  //
    ("h,help", "Open the help menu")   //
    // PRIMARY MODES
    ("contact", "Enter contact edit mode")  //
    ("encrypt", "Enter encryption mode")    //
    ("decrypt", "Enter decryption mode")    //
    // PARAMETERS
    // contact
    ("g,get", "Retrieve a contact")               //
    ("e,edit", "Edit a contact")                  //
    ("d,delete", "Delete a contact")              //
    ("l,list", "List all contact names and IDs")  //
    // encrypt
    ("a,asymmetric", "Asymmetric alg. for key encapsulation", cxxopts::value<std::string>())  //
    ("s,symmetric", "Symmetric alg. for content encryption", cxxopts::value<std::string>())   //
    // OPTIONS
    ("c,contact-id", "Contact ID for any of the params above", cxxopts::value<int>())    //
    ("field", "The field of the contact to edit", cxxopts::value<std::string>())         //
    ("value", "The value to set the contact's field to", cxxopts::value<std::string>())  //
    ("str", "Input content from a string", cxxopts::value<std::string>())                //
    ("i,in-file", "Input content from a file", cxxopts::value<std::string>())            //
    ("o,out-file", "Output content into a file", cxxopts::value<std::string>());

  return options;
}

//------------[ Func. Implementation Separator ]------------\\ 

int phi::parser::parseArguments(cxxopts::Options& options, int argc, char** argv,
                                const std::shared_ptr<phi::database::Database>& DATABASE) {
  try {
    cxxopts::ParseResult result = options.parse(argc, argv);

    size_t arg_count = result.count("help") + result.count("version") + result.count("contact") +
                       result.count("encrypt") + result.count("decrypt");
    if (arg_count != 1) {
      std::cout << RED << "Request must contain exactly ONE primary mode\n" << RST;
      return USAGE_FAILURE;
    }

    if (result.contains("help")) {
      std::cout << "Use the command `" << ITL << "man phi" << RST << "` to access the man page.\n"
                << "If that fails or is outdated, download the man page at this link `" << ITL
                << "https://github.com/the-phi-project/phi/blob/main/documentation/phi.1" << RST
                << "` then\nmove it into wherever your OS keeps manpages."
                << "\n";
      //
    } else if (result.contains("version")) {
      std::cout << "Phi v" << PHI_VERSION << "\n";
      //
    } else if (result.contains("contact")) {
      return phi::parser::parseContactRequest(result, DATABASE);
      //
    } else if (result.contains("encrypt")) {
      return phi::parser::parseEncryptRequest(result, DATABASE);
      //
    } else if (result.contains("decrypt")) {
      return phi::parser::parseDecryptRequest(result, DATABASE);
    }

    return SUCCESS;

  } catch (const cxxopts::exceptions::exception& exc) {
    std::cout << RED << "Error parsing program arguments: " << ITL << exc.what() << RST << "\n";
    return USAGE_FAILURE;
  }
}
