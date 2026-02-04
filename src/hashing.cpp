/*

 PHI
 2026/01/31

 Phi C++ Project
 src/hashing.cpp

 Zevi Berlin

*/

#include "encryption/hashing.hpp"

#include <string>

#include <cryptopp/blake2.h>

//---------> [ Config. Separator ] <---------\\ 

std::string phi::encryption::blake2bHashString(const std::string& str) {
  CryptoPP::BLAKE2b hasher;

  hasher.Update(reinterpret_cast<const unsigned char*>(str.data()), str.size());

  std::string hashed_str;
  hashed_str.resize(hasher.DigestSize());

  hasher.Final(reinterpret_cast<unsigned char*>(hashed_str.data()));

  return hashed_str;
}

//------------[ Func. Implementation Separator ]------------\\ 

bool phi::encryption::blake2bVerifyHash(const std::string& str, const std::string& hash) {
  std::string hashed_str = phi::encryption::blake2bHashString(str);

  return hash == hashed_str;
}