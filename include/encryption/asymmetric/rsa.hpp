/*

 PHI
 2026/01/31

 Phi C++ Project
 include/encryption/asymmetric/rsa.hpp

 Zevi Berlin

*/

#ifndef RSA_HPP
#define RSA_HPP

#include <string>
#include <iostream>

#include <cryptopp/queue.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

//---------> [ Config. Separator ] <---------\\ 

namespace phi::encryption {

/*
Generate a public/private key
 pair using Crypto++ RSA implementation

size is either 2048 or 4096
*/
void rsaGenKeyPair(int size, std::string& op_public, std::string& op_private);

//================={ Header Item Separator }=================\\ 

/*
Encrypt a std::string using an RSA public key
*/
std::string rsaEncrypt(const std::string& str, const std::string& rsa_pub_key);

//================={ Header Item Separator }=================\\ 

/*
Decrypt a std::string using an RSA private key

Returns false and op_text becomes an empty string
 if decryption failure
*/
bool rsaDecrypt(int nbytes, const std::string& str, const std::string& rsa_priv_key,
                std::string& op_text);

//================={ Header Item Separator }=================\\ 

/*
Convert a CryptoPP::RSA:: to a std::string
*/
template <typename KeyType>
static std::string rsaToStr(const KeyType& key) {
  CryptoPP::ByteQueue bqu;

  key.Save(bqu);

  std::string output;
  output.resize(bqu.CurrentSize());
  bqu.Get(reinterpret_cast<unsigned char*>(output.data()), output.size());

  return output;
}

/*
Convert a std::string to a CryptoPP::RSA::
*/
template <typename KeyType>
static KeyType rsaFromStr(const std::string& key) {
  CryptoPP::ByteQueue bqu;

  bqu.Put(reinterpret_cast<const unsigned char*>(key.data()), key.size());
  bqu.MessageEnd();
  bqu.Skip(0);

  KeyType output;
  output.Load(bqu);

  return output;
}

}  // namespace phi::encryption

#endif /* RSA_HPP */
