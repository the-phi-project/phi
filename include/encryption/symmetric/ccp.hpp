/*

 PHI
 2026/01/31

 Phi C++ Project
 include/encryption/symmetric/ccp.hpp

 Zevi Berlin

*/

#ifndef CCP_HPP
#define CCP_HPP

#include <string>
#include <array>
#include <vector>
#include <algorithm>

#include <sodium.h>

//---------> [ Config. Separator ] <---------\\ 

namespace phi::encryption {

/*
Generate a ChaCha20-Poly1305 key in the form of
 a std::string of length crypto_aead_chacha20poly1305_KEYBYTES
*/
std::string ccpGenKey();

//================={ Header Item Separator }=================\\ 

/*
Use ChaCha20-Poly1305 key to encrypt text

Outputs the encrypted text in op_content and the
 none in op_nonce
*/
void ccpEncryptText(const std::string& text, const std::string& ccp_key, std::string& op_content,
                    std::string& op_nonce);

//================={ Header Item Separator }=================\\

/*
Use ChaCha20-Poly1305 key to decrypt text

Returns true and outputs text to the op_text if successful
 otherwise returns false and outputs an empty string
*/
bool ccpDecryptText(const std::string& text, const std::string& ccp_key,
                    const std::string& ccp_nonce, std::string& op_text);
}  // namespace phi::encryption

#endif /* CCP_HPP */
