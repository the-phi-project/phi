/*

 PHI
 2026/01/31

 Phi C++ Project
 src/ccp.cpp

 Zevi Berlin

*/

#include "encryption/symmetric/ccp.hpp"

#include <string>
#include <array>
#include <vector>
#include <algorithm>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

std::string phi::encryption::ccpGenKey() {
  std::array<unsigned char, crypto_aead_chacha20poly1305_KEYBYTES> key;

  crypto_aead_chacha20poly1305_keygen(key.data());

  return std::string(key.begin(), key.end());
}

//------------[ Func. Implementation Separator ]------------\\ 

void ccpEncryptText(const std::string& text, const std::string& ccp_key, std::string& op_content,
                    std::string& op_nonce) {
  std::array<unsigned char, crypto_aead_chacha20poly1305_NPUBBYTES> nonce{};
  unsigned long long outlen = text.length() + crypto_aead_chacha20poly1305_ABYTES;
  std::vector<unsigned char> cipher(outlen);  // ciphertext

  std::array<unsigned char, crypto_aead_chacha20poly1305_KEYBYTES> key;
  std::copy(ccp_key.begin(), ccp_key.end(), key.begin());
  randombytes_buf(nonce.data(), nonce.size());

  crypto_aead_chacha20poly1305_encrypt(
    cipher.data(),                                         // ciphertext output
    &outlen,                                               // ciphertext output length
    reinterpret_cast<const unsigned char*>(text.data()),   // plaintext input
    text.size(),                                           // plaintext length
    nullptr,                                               // additional data, not needed here
    0,                                                     // additional data length
    nullptr,                                               // "nsec always NULL" ~ libsodium docs
    reinterpret_cast<const unsigned char*>(nonce.data()),  // cryptosign nonce
    reinterpret_cast<const unsigned char*>(key.data())     // encryption/decryption key
  );

  op_content = std::string(cipher.begin(), cipher.end());
  op_nonce = std::string(nonce.begin(), nonce.end());
}

//------------[ Func. Implementation Separator ]------------\\ 

bool ccpDecryptText(const std::string& enc_text, const std::string& ccp_key,
                    const std::string& ccp_nonce, std::string& op_text) {
  unsigned long long outlen = enc_text.size() - crypto_aead_chacha20poly1305_ABYTES;
  std::vector<unsigned char> text(outlen);

  int success = crypto_aead_chacha20poly1305_decrypt(
    text.data(),                                              // message output
    &outlen,                                                  // message output length
    nullptr,                                                  // "nsec always NULL" ~ libsodium docs
    reinterpret_cast<const unsigned char*>(enc_text.data()),  // ciphertext input
    enc_text.size(),                                          // ciphertext length
    nullptr,                                                  // additional data, not needed here
    0,                                                        // AD length
    reinterpret_cast<const unsigned char*>(ccp_nonce.data()),  // cryptosign nonce
    reinterpret_cast<const unsigned char*>(ccp_key.data())     // encryption/decryption key
  );

  if (success == -1) {
    op_text = std::string("");
    return false;
  }

  op_text = std::string(text.begin(), text.end());
  return true;
}