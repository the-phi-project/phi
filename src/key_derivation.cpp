/*

 PHI
 2026/03/01

 Phi C++ Project
 src/key_derivation.cpp

 Zevi Berlin

*/

#include "encryption/key_derivation.hpp"

#include <string>
#include <vector>
#include <cstdint>

#include "sha3/shake256.hpp"

//---------> [ Config. Separator ] <---------\\ 

std::string phi::encryption::deriveKey(int key_size, const std::string& context,
                                       const std::string& shared_secret) {
  shake256::shake256_t xof;  // extendable output function
  xof.absorb(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(shared_secret.data()),
                                      shared_secret.size()));
  xof.absorb(
    std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(context.data()), context.size()));
  xof.finalize();

  std::vector<uint8_t> derived(key_size);

  xof.squeeze(derived);

  return std::string(derived.begin(), derived.end());
}