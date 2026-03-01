/*

 PHI
 2026/02/22

 Phi C++ Project
 include/encryption/asymmetric/ntru.h

 Zevi Berlin

*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NTRU_H
#define NTRU_H

#include <string.h>

#include "oqs/oqs.h"  // common include

//---------> [ Config. Separator ] <---------\\ 

/*
Generate public and private keys for NTRUEncrypt

std::string.data() pointers MUST BE preallocated

Size: 1373 for hrss_1373 and 4096 for hsp_40961229
*/
bool ntruGenKeyPair(int size, char* op_public, char* op_private);

//---------> [  ] <---------\\ 

#endif /* NTRU_H */

#ifdef __cplusplus
}  // extern "C"
#endif