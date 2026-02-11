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

int phi::parseCommands(cxxopts::Options& options, int argc, char** argv,
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

    bool encrypt_mode = false;
    if (result.contains("encrypt")) {
      if (result.contains("decrypt")) {
        std::cout << RED << "Error inciting program, argments must contain ONLY ONE mode, not both"
                  << "\n"
                  << RST;
        return 1;
      }
      encrypt_mode = true;
    } else if (result.contains("decrypt")) {
      encrypt_mode = false;
    } else {
      std::cout << RED << "Error inciting program, argments must contain either `" << ITL
                << "--encrypt" << RST << RED << "` or `" << ITL << "--decrypt" << RST << RED
                << "`\n"
                << RST;
      return 1;
    }

    bool in_file_mode = result.contains("in-file");
    bool out_file_mode = result.contains("out-file");

    if (in_file_mode && result.contains("str")) {
      std::cout
        << RED << "Error inciting program, argments must contain ONLY ONE form of input, not both\n"
        << RST;
      return 1;
    }

    if (encrypt_mode) {
      int asymmetric_mode = -1;
      for (const auto& type : phi::asymtypes) {
        if (result["asymmetric"].as<std::string>() == std::get<0>(type)) {
          asymmetric_mode = std::get<1>(type);
          break;
        }
      }
      if (asymmetric_mode == -1) {
        std::cout << RED
                  << "Error inciting program, value must come after --asymmetric argument, any of "
                  << "{rsa2048, rsa4096, kyber512, kyber768}\n"
                  << RST;
        return 1;
      }

      int symmetric_mode = -1;
      for (const auto& type : phi::symtypes) {
        if (result["symmetric"].as<std::string>() == std::get<0>(type)) {
          symmetric_mode = std::get<1>(type);
          break;
        }
      }
      if (symmetric_mode == -1) {
        std::cout << RED
                  << "Error inciting program, value must come after --symmetric argument, any of "
                  << "{aes128, aes192, aes256, chacha20_poly1305}\n"
                  << RST;
        return 1;
      }
    }

    int contact_id = 0;
    if (result.contains("contact-id")) {
      contact_id = result["contact-id"].as<int>();
    } else {
      std::cout << RED << "Error inciting program, `" << ITL << "--contact-id ID" << RST << RED
                << "` argument missing\n"
                << RST;
      return 1;
    }

    std::string in_msg;

    if (in_file_mode) {
      std::ifstream file(result["in-file"].as<std::string>());
      if (!file) {
        std::cout << RED << "Error, could not open input file `" << ITL
                  << result["in-file"].as<std::string>() << RST << RED << "`\n"
                  << RST;
        return 1;
      }
      std::stringstream medium;
      medium << file.rdbuf();
      in_msg = medium.str();
    } else {
      in_msg = result["str"].as<std::string>();
    }

    phi::database::contact_t contact;
    if (!DATABASE->getContact(contact_id, contact)) {
      std::cout << RED << "Error, no contact with ID `" << ITL << std::to_string(contact_id) << RST
                << RED << "`\n"
                << RST;
      return 1;
    }

    if (encrypt_mode) {
    } else {
      in_msg = fromB64(in_msg);

      phi::database::message_t message;
      if (!message.from_json_str(in_msg)) {
        std::cout << RED << "Error, encrypted message is not a valid JSON\n" << RST;
        return 1;
      }

      std::string sym_key;
      switch (message.asymmetric) {
        case 0:
          if (DATABASE->self.rsa2048_priv.empty()) {
            std::cout
              << RED << "Error, you don't have an RSA-2048 key with which to decrypt this message\n"
              << RST;
            return 1;
          }

          if (!phi::encryption::rsaDecrypt(message.symmetric_key_len, message.encrypted_key,
                                           DATABASE->self.rsa2048_priv, sym_key)) {
            std::cout << RED
                      << "Error, your RSA-2048 key is not the right one to decrypt this message\n"
                      << RST;
            return 1;
          }
          break;
        case 1:
          if (DATABASE->self.rsa4096_priv.empty()) {
            std::cout
              << RED << "Error, you don't have an RSA-4096 key with which to decrypt this message\n"
              << RST;
            return 1;
          }

          if (!phi::encryption::rsaDecrypt(message.symmetric_key_len, message.encrypted_key,
                                           DATABASE->self.rsa4096_priv, sym_key)) {
            std::cout << RED
                      << "Error, your RSA-4096 key is not the right one to decrypt this message\n"
                      << RST;
            return 1;
          }

          break;
        case 2:
          // kyber512
          break;
        case 3:
          // kyber768
          break;
        default:
          std::cout << RED
                    << "Error, encrypted message contains an invalid asymmetric encryption mode, "
                       "perhaps you need to update the app?\n"
                    << RST;
          return 1;
      }

      std::string decrypted_msg;
      if (message.symmetric == 3) {  // ChaCha20-Poly1305
        if (!phi::encryption::ccpDecryptText(message.content, sym_key, message.nonce,
                                             decrypted_msg)) {
          std::cout << RED
                    << "Error, the ChaCha20-Poly1305 key given will not decrypt the message\n"
                    << RST;
          return 1;
        }
      } else if (message.symmetric >= 0) {  // AES
        if (!phi::encryption::aesDecrypt(message.content, sym_key, message.iv, decrypted_msg)) {
          std::cout << RED << "Error, the AES key given will not decrypt the message\n" << RST;
          return 1;
        }
      } else {
        std::cout << RED
                  << "Error, encrypted message contains an invalid symmetric encryption mode, "
                     "perhaps you need to update the app?\n"
                  << RST;
        return 1;
      }

      decrypted_msg = phi::encryption::gzipDecompress(decrypted_msg);
      if (!phi::encryption::blake2bVerifyHash(decrypted_msg, message.hash)) {
        std::cout << RED
                  << "Error, the decompressed content does not match the hash signature, "
                     "perhaps the message was tampered with\n"
                  << RST;
        return 1;
      }

      if (out_file_mode) {
        std::ofstream file(result["out-file"].as<std::string>());
        if (!file) {
          std::cout << RED << "Error, could not open output file `" << ITL
                    << result["out-file"].as<std::string>() << RST << RED << "`\n"
                    << RST;
          return 1;
        }

        if (encrypt_mode) {
          ;
        } else {
          file << decrypted_msg << "\n";
        }
      }
    }
  } catch (const cxxopts::exceptions::exception& exc) {
    std::cout << RED << "Error parsing program arguments: " << ITL << exc.what() << RST << "\n";
    return 1;
  }

  return 0;
}
