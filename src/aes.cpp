/*

 PHI
 2026/01/31

 Phi C++ Project
 src/aes.cpp

 Zevi Berlin

*/

#include "encryption/symmetric/aes.hpp"

#include <string>
#include <array>
#include <algorithm>
#include <cstring>

#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>

#define TAG_SIZE 12

//---------> [ Config. Separator ] <---------\\ 

std::string phi::encryption::aesGenKey(int size) {
  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::SecByteBlock key(size / 8);
  rng.GenerateBlock(key, key.size());

  return phi::encryption::aesToStr(key);
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::encryption::aesEncrypt(const std::string& str, const std::string& aes_key,
                                 std::string& encrypted, std::string& aes_iv) {
  CryptoPP::SecByteBlock key = phi::encryption::aesFromStr(aes_key);

  CryptoPP::AutoSeededRandomPool rng;
  std::array<unsigned char, CryptoPP::AES::BLOCKSIZE> iv{};
  rng.GenerateBlock(iv.data(), iv.size());

  CryptoPP::GCM<CryptoPP::AES>::Encryption enc;
  enc.SetKeyWithIV(key, key.size(), iv.data(), iv.size());

  CryptoPP::StringSource css(str, true,
                             new CryptoPP::AuthenticatedEncryptionFilter(
                               enc, new CryptoPP::StringSink(encrypted), false, TAG_SIZE));

  aes_iv = std::string(reinterpret_cast<const char*>(iv.data()), iv.size());
}

//------------[ Func. Implementation Separator ]------------\\ 

bool phi::encryption::aesDecrypt(const std::string& str, const std::string& aes_key,
                                 const std::string& aes_iv, std::string& op_text) {
  try {
    CryptoPP::SecByteBlock key = phi::encryption::aesFromStr(aes_key);

    std::array<unsigned char, CryptoPP::AES::BLOCKSIZE> iv{};
    const size_t copy_len = std::min(aes_iv.size(), iv.size());
    if (copy_len > 0) std::memcpy(iv.data(), aes_iv.data(), copy_len);

    CryptoPP::GCM<CryptoPP::AES>::Decryption dec;
    dec.SetKeyWithIV(key, key.size(), iv.data(), iv.size());

    std::string output;
    // Use CryptoPP::AuthenticatedDecryptionFilter and its DEFAULT_FLAGS
    // constant (namespaced). This filter will verify the authentication tag
    // and only produce output if verification succeeds.
    CryptoPP::AuthenticatedDecryptionFilter adf(
      dec, new CryptoPP::StringSink(output), CryptoPP::AuthenticatedDecryptionFilter::DEFAULT_FLAGS,
      TAG_SIZE);

    CryptoPP::StringSource css(str, true, new CryptoPP::Redirector(adf));

    if (adf.GetLastResult()) {
      op_text = output;
      return true;
    }
  } catch (const CryptoPP::Exception& exc) {
    (void)exc;
  }

  op_text = "";
  return false;
}
