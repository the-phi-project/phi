/*

 PHI
 2026/02/22

 Phi C++ Project
 include/encryption/asymmetric/ed25519.hpp

 Zevi Berlin

*/

#ifndef ED25519_HPP
#define ED25519_HPP

#include <string>

#include <cryptopp/cryptlib.h>
#include <cryptopp/xed25519.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>

//---------> [ Config. Separator ] <---------\\ 

namespace phi::encryption {

/*
Generate a public/private key
 pair using Crypto++ ed25519 implementation
*/
void ed25519GenKeyPair(std::string& op_public, std::string& op_private);

//================={ Header Item Separator }=================\\ 

/*
Sign a message hash using an ed25519
 private key

Returns the signature
*/
std::string ed25519SignMessage(const std::string& msg, const std::string& private_key);

//================={ Header Item Separator }=================\\ 

/*
Verify a message's signed hash using an
 ed25519 public key
*/
bool ed25519VerifySignedMessage(const std::string& msg, const std::string& public_key,
                                const std::string& signature);

//================={ Header Item Separator }=================\\ 

template <typename KeyType>
static std::string ed25519ToStr(const KeyType& key) {
  std::string output;

  CryptoPP::StringSink sink(output);
  key.Save(sink);

  return output;
}

//================={ Header Item Separator }=================\\ 

template <typename KeyType>
static KeyType ed25519FromStr(const std::string& str) {
  KeyType key;
  CryptoPP::StringSource key_source(str, true);
  key.Load(key_source);

  return key;
}

//================={ Header Item Separator }=================\\ 

/*
Validate a CryptoPP::ed25519 key fully
*/
template <typename KeyType>
static bool ed25519ValidateKey(const std::string& str) {
  CryptoPP::AutoSeededRandomPool rng;
  try {
    KeyType key = ed25519FromStr<KeyType>(str);
    // level 2 - make sure this object will function correctly, and do reasonable security checks
    return key.Validate(rng, 2);
  } catch (const CryptoPP::BERDecodeErr& exc) {
    return false;
  }
}

}  // namespace phi::encryption

#endif /* ED25519_HPP */
