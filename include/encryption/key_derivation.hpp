/*

 PHI
 2026/03/01

 Phi C++ Project
 include/encryption/key_derivation.hpp

 Zevi Berlin

*/

#ifndef KEY_DERIVATION_HPP
#define KEY_DERIVATION_HPP

#include <string>
#include <vector>
#include <cstdint>

#include "sha3/shake256.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace phi::encryption {

/*
Derive a symmetric key of size `key_size` from
 a shared secret. context should be the name of the
 algorithm like "aes128" or "chacha20_poly1305"
*/
std::string deriveKey(int key_size, const std::string& context, const std::string& shared_secret);

}  // namespace phi::encryption

#endif /* KEY_DERIVATION_HPP */
