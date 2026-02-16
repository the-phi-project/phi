/*

  PHI
  2025/09/20

  Phi C++ Project
  src/main.cpp

  Zevi Berlin

*/

#include <iostream>
#include <string>
#include <memory>
#include <cstring>

#include "termcolor/termcolor.hpp"

#include "database/Database.hpp"
#include "ui/Manager.hpp"
#include "parser.hpp"
#include "utils/misc_utils.hpp"
#include "utils/str_utils.hpp"

#include "encryption/asymmetric/rsa.hpp"
#include "encryption/symmetric/aes.hpp"
#include "encryption/symmetric/ccp.hpp"

#define RESET_TERM "\x1b[2J\x1b[H"

namespace tmc = termcolor;

int main(int argc, char** argv) {
  bool cli_mode = (argc > 1);

  /**** GLOBAL CONFIG ****/

  int erc = 0;

  // TODO: Add features for updates (~/.phi/updates/) and a backup database (~/.phi/__backup.db)
  if (!createDataFiles(erc)) {
    std::cout << tmc::bright_red;
    if (erc == 1) {
      std::cout << "⛔️ Failed to create data file directory (~/.phi/) ⛔️\n";
    }
    if (erc == 2) {
      std::cout << "⛔️ Failed to create database file (~/.phi/main.db) ⛔️\n";
    }
    std::cout << tmc::reset;
    return erc;
  }  // exit if not valid permissions

  /**/

  const std::shared_ptr<phi::database::Database> DATABASE =
    std::make_shared<phi::database::Database>();

  /*std::string rsa2048_priv, rsa2048_pub;
  std::string rsa4096_priv, rsa4096_pub;

  std::string aes128, aes192, aes256;
  std::string chacha20_poly1305;

  phi::encryption::rsaGenKeyPair(2048, rsa2048_pub, rsa2048_priv);
  phi::encryption::rsaGenKeyPair(4096, rsa4096_pub, rsa4096_priv);

  aes128 = phi::encryption::aesGenKey(128);
  aes192 = phi::encryption::aesGenKey(192);
  aes256 = phi::encryption::aesGenKey(256);

  chacha20_poly1305 = phi::encryption::ccpGenKey();

  std::vector<std::tuple<std::string, std::string>> data{
    {"tests/rsa_2048/priv", rsa2048_priv}, {"tests/rsa_2048/pub", rsa2048_pub},
    {"tests/rsa_4096/priv", rsa4096_priv}, {"tests/rsa_4096/pub", rsa4096_pub},
    {"tests/aes128/key", aes128},          {"tests/aes192/key", aes192},
    {"tests/aes256/key", aes256},          {"tests/chacha20_poly1305/key", chacha20_poly1305}};

  for (const auto& pair : data) {
    std::ofstream file(std::get<0>(pair));
    if (!file) return 1;
    file << toB64(std::get<1>(pair));
  }

  int id = 0;
  DATABASE->deleteContact(21);
  DATABASE->createContact("TESTER", erc, id);
  phi::database::contact_t contact;
  if (!DATABASE->getContact(id, contact)) {
    std::cout << "couldnt get contact with id " << id << "\n";
    return 1;
  }
  phi::database::contact_t new_contact;
  new_contact.name = "TESTER";
  new_contact.id = id;
  new_contact.rsa2048 = rsa2048_pub;
  new_contact.rsa4096 = rsa4096_pub;
  DATABASE->updateContact(contact, new_contact);

  //

  phi::encryption::rsaGenKeyPair(2048, rsa2048_pub, rsa2048_priv);
  phi::encryption::rsaGenKeyPair(4096, rsa4096_pub, rsa4096_priv);

  phi::database::self_t new_self;
  new_self.rsa2048_priv = rsa2048_priv;
  new_self.rsa2048_pub = rsa2048_pub;
  new_self.rsa4096_priv = rsa4096_priv;
  new_self.rsa4096_pub = rsa4096_pub;
  DATABASE->updateSelf(new_self);

  return 0;*/

  /*phi::database::message_t msg{};
  msg.symmetric = 3;
  msg.asymmetric = 1;

  const std::string ccp_key = phi::encryption::ccpGenKey();
  const std::string input =
    "Hello Nikhil, how are you doing? You are a sub5 not an HTN and certainly not a trujeet";
  const std::string hash = phi::encryption::blake2bHashString(input);
  const std::string compressed = phi::encryption::gzipCompress(input);
  phi::encryption::ccpEncryptText(compressed, ccp_key, msg.content, msg.nonce);

  msg.hash = hash;
  msg.symmetric_key_len = ccp_key.size();

  msg.encrypted_key = phi::encryption::rsaEncrypt(ccp_key, DATABASE->self.rsa4096_pub);

  const std::string output = toB64(msg.to_json_str());
  std::cout << output << std::endl;

  return 0;*/


  /*phi::database::message_t msg{};
  std::ifstream file("tests/msg.txt");
  std::stringstream medium;
  medium << file.rdbuf();
  file.close();
  if (!msg.from_json_str(fromB64(medium.str()))) return 1;
  std::string sym_key;
  if (!phi::encryption::rsaDecrypt(msg.symmetric_key_len, msg.encrypted_key,
                                   DATABASE->self.rsa4096_priv, sym_key))
    return 2;
  std::string content;
  if (!phi::encryption::ccpDecryptText(msg.content, sym_key, msg.nonce, content)) return 3;
  content = phi::encryption::gzipDecompress(content);
  if (!phi::encryption::blake2bVerifyHash(content, msg.hash)) return 4;

  std::cout << content << "\n";

  return 0;*/

  /**** ****/

  if (cli_mode) {
    auto options = phi::createOptions();  // cxxopts::Options
    return phi::parseArguments(options, argc, argv, DATABASE);
  }

  auto size = phi::ui::getTerminalSize();
  if (size.ws_col < phi::ui::COLS || size.ws_row < phi::ui::ROWS) {
    std::cout << tmc::bright_red << "⛔️ Phi requires a terminal size of at least " << phi::ui::COLS
              << "x" << phi::ui::ROWS << " to properly render the TUI ⛔️\n"
              << tmc::reset;
    return 1;
  }

  const std::shared_ptr<phi::ui::Manager> MANAGER = std::make_shared<phi::ui::Manager>(DATABASE);

  std::cout << RESET_TERM << std::flush;
  MANAGER->eventLoop();
  std::cout << RESET_TERM << std::flush;

  return 0;
}