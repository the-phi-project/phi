/*

 PHI
 2026/02/22

 Phi C++ Project
 src/ed25519.cpp

 Zevi Berlin

*/

#include "encryption/asymmetric/ed25519.hpp"

#include <string>

#include <cryptopp/cryptlib.h>
#include <cryptopp/xed25519.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>

//---------> [ Config. Separator ] <---------\\ 

void phi::encryption::ed25519GenKeyPair(std::string& op_public, std::string& op_private) {
  CryptoPP::AutoSeededRandomPool rng;
  CryptoPP::ed25519::Signer signer;

  signer.AccessPrivateKey().GenerateRandom(rng);

  const CryptoPP::ed25519PrivateKey& priv =
    dynamic_cast<const CryptoPP::ed25519PrivateKey&>(signer.GetPrivateKey());

  CryptoPP::ed25519::Verifier verifier(signer);
  const CryptoPP::ed25519PublicKey& pub =
    dynamic_cast<const CryptoPP::ed25519PublicKey&>(verifier.GetPublicKey());

  op_private = phi::encryption::ed25519ToStr<CryptoPP::ed25519PrivateKey>(priv);
  op_public = phi::encryption::ed25519ToStr<CryptoPP::ed25519PublicKey>(pub);
}

//------------[ Func. Implementation Separator ]------------\\ 

std::string phi::encryption::ed25519SignMessage(const std::string& msg,
                                                const std::string& private_key) {
  CryptoPP::AutoSeededRandomPool rng;

  CryptoPP::ed25519::Signer signer(
    phi::encryption::ed25519FromStr<CryptoPP::ed25519PrivateKey>(private_key));

  size_t siglen = signer.MaxSignatureLength();
  std::string signature;
  signature.resize(siglen);

  siglen = signer.SignMessage(rng, reinterpret_cast<const CryptoPP::byte*>(msg.data()), msg.size(),
                              reinterpret_cast<CryptoPP::byte*>(signature.data()));
  signature.resize(siglen);

  return signature;
}

//------------[ Func. Implementation Separator ]------------\\ 

bool phi::encryption::ed25519VerifySignedMessage(const std::string& msg,
                                                 const std::string& public_key,
                                                 const std::string& signature) {
  CryptoPP::ed25519::Verifier verifier(
    phi::encryption::ed25519FromStr<CryptoPP::ed25519PublicKey>(public_key));

  return verifier.VerifyMessage(reinterpret_cast<const CryptoPP::byte*>(msg.data()), msg.size(),
                                reinterpret_cast<const CryptoPP::byte*>(signature.data()),
                                signature.size());
}