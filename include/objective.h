#ifndef PALNATOKI_OBJECTIVE_H
#define PALNATOKI_OBJECTIVE_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


PNT_LIB int fAckley(float *x, int n, float *fx);
PNT_LIB int dAckley(double *x, int n, double *fx);
PNT_LIB int ldAckley(long double *x, int n, long double *fx);

#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_OBJECTIVE_H
