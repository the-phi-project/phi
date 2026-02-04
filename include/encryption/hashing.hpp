/*

 PHI
 2026/01/31

 Phi C++ Project
 include/encryption/hashing.hpp

 Zevi Berlin

*/

#ifndef HASHING_HPP
#define HASHING_HPP

#include <string>

#include <cryptopp/blake2.h>

//---------> [ Config. Separator ] <---------\\ 

namespace phi::encryption {

std::string blake2bHashString(const std::string& str);

//================={ Header Item Separator }=================\\ 

bool blake2bVerifyHash(const std::string& str, const std::string& hash);

}  // namespace phi::encryption

#endif /* HASHING_HPP */
