#ifndef PALNATOKI_OBJECTIVE_H
#define PALNATOKI_OBJECTIVE_H

#ifdef __cplusplus
extern "C" {
#endif


int fAckley(float *x, int n, float *fx);
int dAckley(double *x, int n, double *fx);
int ldAckley(long double *x, int n, long double *fx);

#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_OBJECTIVE_H
