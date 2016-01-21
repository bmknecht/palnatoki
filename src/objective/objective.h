#ifndef PALNATOKI_OBJECTIVE_H
#define PALNATOKI_OBJECTIVE_H

#ifdef __cplusplus
extern "C" {
#endif


int ssAuckley(float x, float *fx);
int dsAuckley(double x, double *fx);
int ldsAuckley(long double x, long double *fx);


#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_OBJECTIVE_H
