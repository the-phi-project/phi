/*

 PHI
 2026/02/22

 Phi C++ Project
 src/ntru.c

 Zevi Berlin

*/

#include "encryption/asymmetric/ntru.h"

#include <string.h>

#include "oqs/oqs.h"  // common include

// ive thoroughly inspected liboqs source code and this will never fail to allocate
#define INIT_KEM                                                         \
  OQS_KEM* kem = (size == 1373) ? OQS_KEM_new(OQS_KEM_alg_ntru_hrss1373) \
                                : OQS_KEM_new(OQS_KEM_alg_ntru_hps40961229);

//---------> [ Config. Separator ] <---------\\ 

void ntruGenKeyPair(int size, char* op_public, char* op_private) {
  INIT_KEM

  size_t pub_size = (size == 1373) ? OQS_KEM_ntru_hrss1373_length_public_key
                                   : OQS_KEM_ntru_hps40961229_length_public_key;
  size_t priv_size = (size == 1373) ? OQS_KEM_ntru_hrss1373_length_secret_key
                                    : OQS_KEM_ntru_hps40961229_length_secret_key;

  uint8_t public_key[pub_size];
  uint8_t secret_key[priv_size];

  // only fails when kem is NULL which is never
  OQS_KEM_keypair(kem, public_key, secret_key);

  OQS_KEM_free(kem);

  memcpy(op_public, public_key, pub_size);
  memcpy(op_private, secret_key, priv_size);
}

//------------[ Func. Implementation Separator ]------------\\ 

void ntruGenData(int size, const char* ntru_pub_key, char* op_ciphertext, char* op_ss) {
  INIT_KEM

  size_t ciphertext_size = (size == 1373) ? OQS_KEM_ntru_hrss1373_length_ciphertext
                                          : OQS_KEM_ntru_hps40961229_length_ciphertext;
  // both algorithms have the same shared secret size
  size_t ss_size = OQS_KEM_ntru_hrss1373_length_shared_secret;

  uint8_t ciphertext[ciphertext_size];
  uint8_t shared_secret[ss_size];

  // assuming that char* -> uint8_t* will never create unintended effects
  // only fails when kem is NULL which is never
  OQS_KEM_encaps(kem, ciphertext, shared_secret, (uint8_t*)ntru_pub_key);

  OQS_KEM_free(kem);

  memcpy(op_ciphertext, ciphertext, ciphertext_size);
  memcpy(op_ss, shared_secret, ss_size);
}

//------------[ Func. Implementation Separator ]------------\\ 

void ntruDeriveSS(int size, const char* ntru_priv_key, const char* ciphertext, char* op_ss) {
  INIT_KEM

  // both algorithms have the same shared secret size
  size_t ss_size = OQS_KEM_ntru_hrss1373_length_shared_secret;

  uint8_t shared_secret[ss_size];

  // assuming that char* -> uint8_t* will never create unintended effects
  // only fails when kem is NULL which is never
  OQS_KEM_decaps(kem, shared_secret, (uint8_t*)ciphertext, (uint8_t*)ntru_priv_key);

  OQS_KEM_free(kem);

  memcpy(op_ss, shared_secret, ss_size);
}