/*

 PHI
 2026/01/31

 Phi C++ Project
 include/encryption/symmetric/twofish.hpp

 Zevi Berlin

*/

#ifndef TWOFISH_HPP
#define TWOFISH_HPP

#include <string>
#include <array>
#include <algorithm>

#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/twofish.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>

#include "encryption/symmetric/aes.hpp"  // for byteblockToStr() and byteblockFromStr()

//---------> [ Config. Separator ] <---------\\ 

namespace phi::encryption {

/*
Generate a twofish key using Crypto++ implementation
*/
std::string twofishGenKey();

//================={ Header Item Separator }=================\\ 

/*
Encrypt a std::string using a twofish key
*/
void twofishEncrypt(const std::string& str, const std::string& twofish_key, std::string& encrypted,
                    std::string& twofish_iv);

//================={ Header Item Separator }=================\\ 

/*
Decrypt a std::string using a twofish key

Returns false and op_text becomes an empty string
 upon decryption failure
*/
bool twofishDecrypt(const std::string& str, const std::string& twofish_key,
                    const std::string& twofish_iv, std::string& op_text);

}  // namespace phi::encryption

#endif /* TWOFISH_HPP */
