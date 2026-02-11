/*

 PHI
 2026/01/31

 Phi C++ Project
 src/rsa.cpp

 Zevi Berlin

*/

#include "encryption/asymmetric/rsa.hpp"

#include <string>

#include <cryptopp/queue.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>

//---------> [ Config. Separator ] <---------\\ 

void phi::encryption::rsaGenKeyPair(int size, std::string& op_public, std::string& op_private) {
  op_public.resize(size / 8);
  op_private.resize(size / 8);

  CryptoPP::AutoSeededRandomPool rng;

  CryptoPP::RSA::PrivateKey priv;
  CryptoPP::RSA::PublicKey pub;

  priv.GenerateRandomWithKeySize(rng, size);
  pub = CryptoPP::RSA::PublicKey(priv);

  /**/

  op_public = phi::encryption::rsaToStr<CryptoPP::RSA::PublicKey>(pub);
  op_private = phi::encryption::rsaToStr<CryptoPP::RSA::PrivateKey>(priv);
}

//------------[ Func. Implementation Separator ]------------\\ 

std::string phi::encryption::rsaEncrypt(const std::string& str, const std::string& rsa_pub_key) {
  CryptoPP::RSA::PublicKey key = phi::encryption::rsaFromStr<CryptoPP::RSA::PublicKey>(rsa_pub_key);

  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(key);

  std::string output;
  CryptoPP::StringSource css(
    reinterpret_cast<const char*>(str.data()), true,
    new CryptoPP::PK_EncryptorFilter(rng, encryptor, new CryptoPP::StringSink(output)));

  return output;
}

//------------[ Func. Implementation Separator ]------------\\ 

bool phi::encryption::rsaDecrypt(int nbytes, const std::string& str,
                                 const std::string& rsa_priv_key, std::string& op_text) {
  try {
    CryptoPP::RSA::PrivateKey key =
      phi::encryption::rsaFromStr<CryptoPP::RSA::PrivateKey>(rsa_priv_key);

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(key);

    std::string output;
    CryptoPP::StringSource css(
      str, true,
      new CryptoPP::PK_DecryptorFilter(rng, decryptor, new CryptoPP::StringSink(output)));

    op_text = output;
    return true;
  } catch (const CryptoPP::Exception& exc) {
    op_text = "";
    return false;
  }
}