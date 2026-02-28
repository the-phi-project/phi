/*

 PHI
 2026/01/31

 Phi C++ Project
 src/aes.cpp

 Zevi Berlin

*/

#include "encryption/symmetric/twofish.hpp"

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

constexpr int TAG_SIZE = 12;

//---------> [ Config. Separator ] <---------\\ 

std::string phi::encryption::twofishGenKey() {
  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::SecByteBlock key(32);  // maybe in the future we'll allow 128 and 192
  rng.GenerateBlock(key, key.size());

  return phi::encryption::byteblockToStr(key);
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::encryption::twofishEncrypt(const std::string& str, const std::string& twofish_key,
                                     std::string& encrypted, std::string& twofish_iv) {
  CryptoPP::SecByteBlock key = phi::encryption::byteblockFromStr(twofish_key);

  CryptoPP::AutoSeededRandomPool rng;
  std::array<unsigned char, CryptoPP::Twofish::BLOCKSIZE> iv{};
  rng.GenerateBlock(iv.data(), iv.size());

  CryptoPP::GCM<CryptoPP::Twofish>::Encryption enc;
  enc.SetKeyWithIV(key, key.size(), iv.data(), iv.size());

  CryptoPP::StringSource css(str, true,
                             new CryptoPP::AuthenticatedEncryptionFilter(
                               enc, new CryptoPP::StringSink(encrypted), false, TAG_SIZE));

  twofish_iv = std::string(reinterpret_cast<const char*>(iv.data()), iv.size());
}

//------------[ Func. Implementation Separator ]------------\\ 

bool phi::encryption::twofishDecrypt(const std::string& str, const std::string& twofish_key,
                                     const std::string& twofish_iv, std::string& op_text) {
  try {
    CryptoPP::SecByteBlock key = phi::encryption::byteblockFromStr(twofish_key);

    std::array<unsigned char, CryptoPP::Twofish::BLOCKSIZE> iv{};
    const size_t copy_len = std::min(twofish_iv.size(), iv.size());
    if (copy_len > 0) std::memcpy(iv.data(), twofish_iv.data(), copy_len);

    CryptoPP::GCM<CryptoPP::Twofish>::Decryption dec;
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
