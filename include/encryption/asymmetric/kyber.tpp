// NOLINTBEGIN -- I had to do this because this file was causing fake problems

/*

 PHI
 2026/02/20

 Phi C++ Project
 include/encryption/asymmetric/kyber.tpp

 Zevi Berlin

*/

#ifndef KYBER_TPP
#define KYBER_TPP

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

namespace phi::encryption {
template <typename variant>
void kyberGenKeyPair(std::array<uint8_t, variant::pkey_len>& op_public,
                     std::array<uint8_t, variant::skey_len>& op_private) {
  //
  randomshake::randomshake_t rng;

  std::array<uint8_t, kyber_seed_d_len>
    seed_d{};  // used for deterministic generation of the public matrix
  std::array<uint8_t, kyber_seed_z_len>
    seed_z{};  // harden the secret key against multi-target attacks

  rng.generate(seed_d);
  rng.generate(seed_z);

  variant::keygen(seed_d, seed_z, op_public, op_private);
}

//------------[ Func. Implementation Separator ]------------\\ 

template <typename variant>
bool kyberGenData(const std::array<uint8_t, variant::pkey_len>& kyber_pub_key,
                  std::array<uint8_t, variant::ciphertext_len>& op_ciphertext,
                  std::array<uint8_t, kyber_ss_len>& op_ss) {
  //
  randomshake::randomshake_t rng;

  std::array<uint8_t, kyber_seed_m_len> encapsulation_seed{};
  rng.generate(encapsulation_seed);

  return variant::encapsulate(encapsulation_seed, kyber_pub_key, op_ciphertext, op_ss);
}

//------------[ Func. Implementation Separator ]------------\\ 

template <typename variant>
std::vector<uint8_t> kyberDeriveKey(int key_size, const std::string& context,
                                    const std::array<uint8_t, kyber_ss_len>& ss) {
  //
  shake256::shake256_t xof;  // extendable output function
  xof.absorb(ss);
  xof.absorb(
    std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(context.data()), context.size()));
  xof.finalize();

  std::vector<uint8_t> derived(key_size);

  xof.squeeze(derived);

  return derived;
}

}  // namespace phi::encryption

#endif /* KYBER_TPP */

// NOLINTEND