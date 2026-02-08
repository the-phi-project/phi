/*

 PHI
 2026/01/31

 Phi C++ Project
 include/encryption/symmetric/aes.hpp

 Zevi Berlin

*/

#ifndef AES_HPP
#define AES_HPP

#include <string>
#include <array>
#include <algorithm>

#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/aes.h>

//---------> [ Config. Separator ] <---------\\ 

namespace phi::encryption {

/*
Generate a public/private key
 pair using Crypto++ AES implementation

size is either 128, 192, or 256
*/
std::string aesGenKey(int size);

//================={ Header Item Separator }=================\\ 

/*
Encrypt a std::string using an AES key
*/
void aesEncrypt(const std::string& str, const std::string& aes_key, std::string& encrypted,
                std::string& aes_iv);

//================={ Header Item Separator }=================\\ 

/*
Decrypt a std::string using an AES key

Returns false and op_text becomes an empty string
 upon decryption failure
*/
bool aesDecrypt(const std::string& str, const std::string& aes_key, const std::string& aes_iv,
                std::string& op_text);

//================={ Header Item Separator }=================\\ 

/*
Convert a CryptoPP::SecByteBlock to a std::string
*/
inline std::string aesToStr(const CryptoPP::SecByteBlock& key) {
  return std::string(reinterpret_cast<const char*>(key.data()), key.size());
}

/*
Convert a std::string to a CryptoPP::SecByteBlock
*/
inline CryptoPP::SecByteBlock aesFromStr(const std::string& key) {
  CryptoPP::SecByteBlock output(static_cast<size_t>(key.size()));

  if (!key.empty()) std::memcpy(output.data(), key.data(), key.size());

  return output;
}

}  // namespace phi::encryption

#endif /* RSA_HPP */
