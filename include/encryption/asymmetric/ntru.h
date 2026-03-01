/*

 PHI
 2026/02/22

 Phi C++ Project
 include/encryption/asymmetric/ntru.h

 Zevi Berlin

*/

#ifndef NTRU_H
#define NTRU_H


#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>  // memcpy()

#include "oqs/oqs.h"  // common include

//---------> [ Config. Separator ] <---------\\ 

enum alg { NTRU1373 = 1373, NTRU4096 = 4096 };

/*
Check that both algorithms are available to use
*/
static bool ntruInit() {
  OQS_KEM* kem = OQS_KEM_new(OQS_KEM_alg_ntru_hps40961229);
  if (kem == NULL) return false;
  OQS_KEM_free(kem);

  kem = OQS_KEM_new(OQS_KEM_alg_ntru_hrss1373);
  if (kem == NULL) return false;
  OQS_KEM_free(kem);

  return true;
}

/*
Generate public and private keys for NTRUEncrypt

Size: 1373 for hrss_1373 and 4096 for hps_40961229

std::string.data() pointers MUST BE preallocated
 op_public: OQS_KEM_ntru_hrss1373_length_public_key
 + op_private: OQS_KEM_ntru_hrss1373_length_secret_key

 op_public: OQS_KEM_ntru_hps40961229_length_public_key
 + op_private: OQS_KEM_ntru_hps40961229_length_secret_key
*/
void ntruGenKeyPair(int size, char* op_public, char* op_private);

//================={ Header Item Separator }=================\\ 

/*
Generate a ciphertext and shared secret for NTRUEncrypt
 given a public key


Size: 1373 for hrss_1373 and 4096 for hps_40961229

std::string.data() pointers MUST BE preallocated

 op_ciphertext: OQS_KEM_ntru_hrss1373_length_ciphertext
 + op_ss: OQS_KEM_ntru_hrss1373_length_shared_secret

 op_ciphertext: OQS_KEM_ntru_hps40961229_length_ciphertext
 + op_ss: OQS_KEM_ntru_hps40961229_length_shared_secret
*/
void ntruGenData(int size, const char* ntru_pub_key, char* op_ciphertext, char* op_ss);

//================={ Header Item Separator }=================\\ 

/*
Derive a shared secret from an NTRUEncrypt ciphertext
 given a private key

Size: 1373 for hrss_1373 and 4096 for hps_40961229

std::string.data() pointers MUST BE preallocated

 op_ss: OQS_KEM_ntru_hrss1373_length_shared_secret

 op_ss: OQS_KEM_ntru_hps40961229_length_shared_secret
*/
void ntruDeriveSS(int size, const char* ntru_priv_key, const char* ciphertext, char* op_ss);

//---------> [  ] <---------\\ 


#ifdef __cplusplus
}  // extern "C"
#endif


#endif /* NTRU_H */