/*

 PHI
 2026/01/31

 Phi C++ Project
 src/kyber.cpp

 Zevi Berlin

*/

#include "encryption/asymmetric/kyber.hpp"

#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <cstring>

#include "ml_kem/ml_kem_512.hpp"
#include "ml_kem/ml_kem_768.hpp"
#include "randomshake/randomshake.hpp"
#include "sha3/shake256.hpp"

//---------> [ Config. Separator ] <---------\\ 

// TODO: make all of these functions templates
void phi::encryption::kyber512GenKeyPair(
  std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN>& op_public,
  std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN>& op_private) {
  //
  randomshake::randomshake_t rng;

  std::array<uint8_t, ml_kem_512::SEED_D_BYTE_LEN>
    seed_d{};  // used for deterministic generation of the public matrix
  std::array<uint8_t, ml_kem_512::SEED_Z_BYTE_LEN>
    seed_z{};  // harden the secret key against multi-target attacks

  rng.generate(seed_d);
  rng.generate(seed_z);

  ml_kem_512::keygen(seed_d, seed_z, op_public, op_private);
}

//------------[ Func. Implementation Separator ]------------\\ 

bool phi::encryption::kyber512GenData(
  const std::array<uint8_t, ml_kem_512::PKEY_BYTE_LEN>& kyber_pub_key,
  std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN>& op_ciphertext,
  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN>& op_ss) {
  //
  randomshake::randomshake_t rng;

  std::array<uint8_t, ml_kem_512::SEED_M_BYTE_LEN> encapsulation_seed{};
  rng.generate(encapsulation_seed);

  return ml_kem_512::encapsulate(encapsulation_seed, kyber_pub_key, op_ciphertext, op_ss);
}

//------------[ Func. Implementation Separator ]------------\\ 

void phi::encryption::kyber512DeriveSS(
  const std::array<uint8_t, ml_kem_512::CIPHER_TEXT_BYTE_LEN>& ciphertext,
  const std::array<uint8_t, ml_kem_512::SKEY_BYTE_LEN>& kyber_priv_key,
  std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN>& op_ss) {
  //
  ml_kem_512::decapsulate(kyber_priv_key, ciphertext, op_ss);
}

//------------[ Func. Implementation Separator ]------------\\ 

std::vector<uint8_t> phi::encryption::kyber512DeriveKey(
  int key_size, const std::array<uint8_t, ml_kem_512::SHARED_SECRET_BYTE_LEN>& ss) {
  //
  shake256::shake256_t xof;  // extendable output function
  xof.absorb(ss);
  xof.finalize();

  std::vector<uint8_t> derived(key_size);

  xof.squeeze(derived);

  return derived;
}