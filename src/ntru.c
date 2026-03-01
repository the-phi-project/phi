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

//---------> [ Config. Separator ] <---------\\ 

bool ntruGenKeyPair(int size, char* op_public, char* op_private) {
  OQS_KEM* kem = NULL;
  if (size == 1373) {
    kem = OQS_KEM_new(OQS_KEM_alg_ntru_hrss1373);
  } else {  // size == 4096
    kem = OQS_KEM_new(OQS_KEM_alg_ntru_hps40961229);
  }

  if (kem == NULL) {
    return false;
  }

  size_t pub_size = (size == 1373) ? OQS_KEM_ntru_hrss1373_length_public_key
                                   : OQS_KEM_ntru_hps40961229_length_public_key;
  size_t priv_size = (size == 1373) ? OQS_KEM_ntru_hrss1373_length_secret_key
                                    : OQS_KEM_ntru_hps40961229_length_secret_key;

  uint8_t public_key[pub_size];
  uint8_t secret_key[priv_size];

  OQS_STATUS scs = OQS_KEM_keypair(kem, public_key, secret_key);

  OQS_KEM_free(kem);  // early free

  if (scs != OQS_SUCCESS) {
    return false;
  }

  memcpy(op_public, public_key, pub_size);
  memcpy(op_private, secret_key, priv_size);

  return true;
}