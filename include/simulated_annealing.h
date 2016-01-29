#ifndef PALNATOKI_SIMULATED_ANNEALING_H
#define PALNATOKI_SIMULATED_ANNEALING_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


//
//  Result structure type definitions
//
typedef struct _pntfSAResult
{
	unsigned int iterations;
	float *restrict x;
	float fx;
} pntfSAResult;


//
//  Parameter structure
//
typedef struct _pntfSAParameters {
    // non-optional
    float *restrict x;
    unsigned int n;
    float fx;
    float (*f)(const float *restrict x, unsigned int n);
    unsigned int itermax;
    float fmin;
    void (*neighbour)(const float *restrict x, unsigned int n,
                      float t, float *restrict xnew);
    void *restrict work;

    // optional
    float (*temperature)(unsigned int i, unsigned int itermax,
                         float oldTemperature);
    int (*downgrade)(float fx, float fxnew, float temperature);
} pntfSAParameters;


//
// main functions
//
PNT_LIB int pntfSimAnn(pntfSAParameters *parameters,
                       pntfSAResult *result);


#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_SIMULATED_ANNEALING_H
