/*

 PHI
 2026/02/22

 Phi C++ Project
 src/ntru.c

 Zevi Berlin

*/

#include "encryption/asymmetric/ntru.h"

#include <stdlib.h>

#include "oqs/oqs.h"  // common include

//---------> [ Config. Separator ] <---------\\ 

void ntru_testing(int a) {
  OQS_KEM* kem = OQS_KEM_new(OQS_KEM_alg_ntru_hrss1373);

  if (kem == NULL) {
    fprintf(stderr, "ERROR: KEM 1 algorithm not enabled\n");
    return 1;
  }

  OQS_KEM_free(kem);

  kem = OQS_KEM_new(OQS_KEM_alg_ntru_hps40961229);

  if (kem == NULL) {
    fprintf(stderr, "ERROR: KEM 2 algorithm not enabled\n");
    return 1;
  }

  OQS_KEM_free(kem);
}