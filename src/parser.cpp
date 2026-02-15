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

//---------> [ Config. Separator ] <---------\\ 

int phi::parseArguments(cxxopts::Options& options, int argc, char** argv,
                        const std::shared_ptr<phi::database::Database>& DATABASE) {
  try {
    auto result = options.parse(argc, argv);

    if (result.contains("help")) {
      std::cout << options.help() << "\n";
      return 0;
    }
    if (result.contains("list-contacts")) {
      std::vector<std::tuple<int, std::string>> contacts = DATABASE->getAllContacts();
      for (const auto& contact : contacts) {
        std::cout << "(" << std::get<0>(contact) << ") " << std::get<1>(contact) << "\n";
      }
      return 0;
    }

    /*########## LONG FUNC ###########*\
    \*######## IN-SEPARATOR ##########*/

    if (result.contains("encrypt")) {
      if (result.contains("decrypt")) {
        std::cout << RED << "Cannot have both encrypt mode and decrypt mode enabled\n" << RST;
        return 1;
      }

      /*########## LONG FUNC ###########*\
      \*######## IN-SEPARATOR ##########*/

      if (!result.contains("contact-id")) {
        std::cout << RED
                  << "Arguments must include a contact ID after --contact-id to encrypt a message\n"
                  << RST;
        return 2;
      }
      int contact_id = result["contact-id"].as<int>();

      /*########## LONG FUNC ###########*\
      \*######## IN-SEPARATOR ##########*/

      int symmode = -1;
      int asymmode = -1;
      if (result.count("symmetric") == 1 && result.count("asymmetric") == 1) {
        std::string sym = result["symmetric"].as<std::string>();
        std::string asym = result["asymmetric"].as<std::string>();

        for (size_t i = 0; i < phi::symtypes.size(); i++) {
          if (phi::symtypes.at(i) == sym) {
            symmode = i;
            break;
          }
        }
        for (size_t i = 0; i < phi::asymtypes.size(); i++) {
          if (phi::asymtypes.at(i) == asym) {
            asymmode = i;
            break;
          }
        }
      } else {
        std::cout << RED
                  << "Arguments must include a single argument for both symmetric and asymmetric "
                     "modes of encryption after --symmetric and --asymmetric (respectively)\n"
                  << RST;
        return 3;
      }
      if (symmode == -1) {
        std::cout << RED << "Invalid symmetric mode of encryption, options are `" << ITL
                  << vecStrToStr(phi::symtypes) << RST << RED << "`\n"
                  << RST;
        return 3;
      }
      if (asymmode == -1) {
        std::cout << RED << "Invalid asymmetric mode of encryption, options are `" << ITL
                  << vecStrToStr(phi::asymtypes) << RST << RED << "`\n"
                  << RST;
        return 3;
      }

      /*########## LONG FUNC ###########*\
      \*######## IN-SEPARATOR ##########*/

      std::string in_msg;
      if (result.contains("in-file")) {
        std::ifstream file(result["in-file"].as<std::string>());
        if (!file) {
          std::cout << RED << "Failed to open and read the file `" << ITL
                    << result["in-file"].as<std::string>() << RST << RED << "`\n"
                    << RST;
          return 4;
        }

        std::stringstream medium;
        medium << file.rdbuf();
        in_msg = medium.str();
      } else if (result.contains("str")) {
        in_msg = result["str"].as<std::string>();
      } else {
        std::cout << RED
                  << "Arguments must contain either an input filepath after --in-file or an input "
                     "as `--str \"<input>\"`\n"
                  << RST;
        return 4;
      }

      /*########## LONG FUNC ###########*\
      \*######## IN-SEPARATOR ##########*/

      phi::database::message_t output{};
      if (!phi::encryptMessage(DATABASE, contact_id, in_msg, symmode, asymmode, output)) {
        std::cout << tmc::bright_red << BOLD << "ENCRYPTION FAILED.\n" << RST;
        return 5;
      }

      /*########## LONG FUNC ###########*\
      \*######## IN-SEPARATOR ##########*/

      if (result.contains("out-file")) {
        std::ofstream file(result["out-file"].as<std::string>());
        if (!file) {
          std::cout << RED << "Failed to open and write to the file `" << ITL
                    << result["out-file"].as<std::string>() << RST << RED << "`\n"
                    << RST;
          return 6;
        }

        file << toB64(output.to_json_str());
      } else {
        std::cout << toB64(output.to_json_str()) << "\n";
      }

      std::cout << tmc::bright_green << BOLD << "ENCRYPTION SUCCESS.\n" << RST;
      return 0;

    } else if (result.contains("decrypt")) {
      std::string in_msg;
      if (result.contains("in-file")) {
        std::ifstream file(result["in-file"].as<std::string>());
        if (!file) {
          std::cout << RED << "Failed to open and read the file `" << ITL
                    << result["in-file"].as<std::string>() << RST << RED << "`\n"
                    << RST;
          return 4;
        }

        std::stringstream medium;
        medium << file.rdbuf();
        in_msg = fromB64(medium.str());
      } else if (result.contains("str")) {
        in_msg = fromB64(result["str"].as<std::string>());
      } else {
        std::cout << RED
                  << "Arguments must contain either an input filepath after --in-file or an input "
                     "as `--str \"<base64 input>\"`\n"
                  << RST;
        return 4;
      }

      /*########## LONG FUNC ###########*\
      \*######## IN-SEPARATOR ##########*/

      phi::database::message_t message{};
      if (!message.from_json_str(in_msg)) {
        std::cout << RED
                  << "Input message is not a valid JSON, try base64 decoding it and checking for "
                     "formatting errors\n"
                  << RST;
        return 7;
      }

      /*########## LONG FUNC ###########*\
      \*######## IN-SEPARATOR ##########*/

      std::string decrypted;
      if (!phi::decryptMessage(DATABASE, message, decrypted)) {
        std::cout << tmc::bright_red << BOLD << "DECRYPTION FAILED.\n" << RST;
        return 5;
      }

      /*########## LONG FUNC ###########*\
      \*######## IN-SEPARATOR ##########*/

      if (result.contains("out-file")) {
        std::ofstream file(result["out-file"].as<std::string>());
        if (!file) {
          std::cout << RED << "Failed to open and write to the file `" << ITL
                    << result["out-file"].as<std::string>() << RST << RED << "`\n"
                    << RST;
          return 6;
        }

        file << decrypted;
      } else {
        std::cout << decrypted << "\n";
      }

      std::cout << tmc::bright_green << BOLD << "DECRYPTION SUCCESS.\n" << RST;
      return 0;

    } else {
      std::cout << RED
                << "Arguments must contain either --encrypt or --decrypt for encrypt or decrypt "
                   "mode (respectively)\n"
                << RST;
      return 8;
    }
  } catch (const cxxopts::exceptions::exception& exc) {
    std::cout << RED << "Error parsing program arguments: " << ITL << exc.what() << RST << "\n";
    return 9;
  }

  return 0;
}

//------------[ Func. Implementation Separator ]------------\\ 


bool phi::encryptMessage(const std::shared_ptr<phi::database::Database>& DATABASE, int contact_id,
                         const std::string& message, int symmode, int asymmode,
                         phi::database::message_t& op) {
  //
  op.symmetric = symmode;
  op.asymmetric = asymmode;
  op.hash = phi::encryption::blake2bHashString(message);

  const std::string compressed = phi::encryption::gzipCompress(message);

  phi::database::contact_t contact;
  if (!DATABASE->getContact(contact_id, contact)) {
    std::cout << RED << "Failed to encrypt message, no contact found in database with ID `" << ITL
              << contact_id << RST << RED << "`\n"
              << RST;
    return false;
  }

  std::string sym_key;
  if (symmode >= 0 && symmode < 3) {
    // if symmode is 0 this evals to 128, if symmode is 1 it evals to 192, if 2 its 256
    sym_key = phi::encryption::aesGenKey(128 + (symmode * 64));
    phi::encryption::aesEncrypt(compressed, sym_key, op.content, op.iv);
  } else if (symmode == 3) {
    sym_key = phi::encryption::ccpGenKey();
    phi::encryption::ccpEncryptText(compressed, sym_key, op.content, op.nonce);
  }

  if (asymmode == 0 || asymmode == 1) {
    try {
      std::string rsa;
      asymmode == 0 ? rsa = contact.rsa2048 : rsa = contact.rsa4096;
      op.encrypted_key = phi::encryption::rsaEncrypt(sym_key, rsa);
      op.symmetric_key_len = sym_key.size();
    } catch (const CryptoPP::BERDecodeErr& err) {
      std::cout << RED << "Failed to encrypt message, contact does not have a valid RSA-"
                << (asymmode == 0 ? "2048" : "4096") << " public key\n"
                << RST;
      return false;
    }
  } else if (asymmode == 2 || asymmode == 3) {
    ;  // KYBER
  }

  return true;
}

//------------[ Func. Implementation Separator ]------------\\


bool phi::decryptMessage(const std::shared_ptr<phi::database::Database>& DATABASE,
                         const phi::database::message_t& message, std::string& op) {
  std::string sym_key;
  if (message.asymmetric == 0 || message.asymmetric == 1) {
    try {
      std::string rsa;
      message.asymmetric == 0 ? rsa = DATABASE->self.rsa2048_priv
                              : rsa = DATABASE->self.rsa4096_priv;
      if (!phi::encryption::rsaDecrypt(message.symmetric_key_len, message.encrypted_key, rsa,
                                       sym_key)) {
        std::cout << RED << "Failed to decrypt message, your RSA-"
                  << (message.asymmetric == 0 ? "2048" : "4096")
                  << " private key will not decrypt the message\n"
                  << RST;
        return false;
      }
    } catch (const CryptoPP::BERDecodeErr& err) {
      std::cout << RED << "Failed to decrypt message, you do not have a valid RSA-"
                << (message.asymmetric == 0 ? "2048" : "4096") << " private key\n"
                << RST;
      return false;
    }
  } else if (message.asymmetric == 2 || message.asymmetric == 3) {
    ;  // KYBER
  }

  std::string compressed;
  if (message.symmetric >= 0 && message.symmetric < 3) {
    if (!phi::encryption::aesDecrypt(message.content, sym_key, message.iv, compressed)) {
      std::cout << RED << "Failed to decrypt message, the given AES-"
                << (message.symmetric_key_len * 8) << " key will not decrypt the message\n"
                << RST;
      return false;
    }
  } else if (message.symmetric == 3) {
    if (!phi::encryption::ccpDecryptText(message.content, sym_key, message.nonce, compressed)) {
      std::cout << RED
                << "Failed to decrypt message, the given ChaCha20-Poly1305 key will not decrypt "
                   "the message\n"
                << RST;
      return false;
    }
  }

  op = phi::encryption::gzipDecompress(compressed);
  if (!phi::encryption::blake2bVerifyHash(op, message.hash)) {
    std::cout << RED
              << "Message successfully decrypted, however the hash signature does not match, be "
                 "wary of a tampered message\n"
              << RST;
    return true;
  }

  return true;
}