#ifndef INPUT_VALIDATION_UTILS_HPP
#define INPUT_VALIDATION_UTILS_HPP


#include <string>

#include "encryption/asymmetric/rsa.hpp"
#include "encryption/asymmetric/kyber.hpp"
#include "encryption/asymmetric/ed25519.hpp"
#include "encryption/asymmetric/ntru.h"

//---------> [ Config. Separator ] <---------\\ 

// value should be BASE64
[[nodiscard]] static bool validateGivenKeyContact(const std::string& field,
                                                  const std::string& value) {
  if (field.starts_with("rsa")) {
    return phi::encryption::rsaValidateKey<CryptoPP::RSA::PublicKey>(value);
  } else if (field.starts_with("kyber")) {
    // wont fail because we already validated the field
    int suffix = std::stoi(field.substr(5));
    switch (suffix) {
      case 512:
        return (value.size() == ml_kem_512::PKEY_BYTE_LEN);  // only real check we can do
      case 768:
        return (value.size() == ml_kem_768::PKEY_BYTE_LEN);  // only real check we can do
      case 1024:
        return (value.size() == ml_kem_1024::PKEY_BYTE_LEN);  // only real check we can do
    }
  } else if (field.starts_with("ntru")) {
    return true;  // TODO: Implement NTRU everywhere not just here
  } else if (field.starts_with("ed25519")) {
    return phi::encryption::ed25519ValidateKey<CryptoPP::ed25519PublicKey>(value);
  }

  return false;
}

//------------[ Func. Implementation Separator ]------------\\ 

// TODO: Find an efficient way to implement this func
[[nodiscard]] static bool validateGivenKeySelf(const std::string& field, const std::string& value) {
  return true;
}

#endif /* INPUT_VALIDATION_UTILS_HPP */