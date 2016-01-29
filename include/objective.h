#ifndef PALNATOKI_OBJECTIVE_H
#define PALNATOKI_OBJECTIVE_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


PNT_LIB float fAckley(const float *restrict x, unsigned int n);
PNT_LIB double dAckley(const double *restrict x, unsigned int n);
PNT_LIB long double ldAckley(const long double *restrict x, unsigned int n);

#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_OBJECTIVE_H
