#ifndef PALNATOKI_SIMULATED_ANNEALING_H
#define PALNATOKI_SIMULATED_ANNEALING_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


//
//  Neighbour function type definitions
//
typedef void (*pntfSANeighbour)(const float *restrict x, unsigned int dim,
                                float t, float *restrict x1);


//
//  Objective function type definitions
//
typedef float (*pntfSAObjective)(const float *restrict x, unsigned int n);


//
//  Result structure type definitions
//
typedef struct _pntfSAResult
{
	unsigned int iterations;
	float *restrict x;
	float fx;
} pntfSAResult;


/** Computes the minimum of an objective function via the simulated
 *  annealing algorithm.  
 *
 *  Args:
 *  	x0: starting parameter(s) for the objective function
 *  	f: the objective function to minimize
 *  	nmax: maximum number of iterations (equals function evaluations + 1)
 *  	xmin, xmax: functions to keep parameter(s) in bounds, alternatively
 *  	vectors of minimum / maximum values
 *  	fmin: desired objective function value
 *  	temperature: callable which chooses the next temperature
 *  		(standard: linear)
 *  	newNeighbour: callable which chooses the next parameter set
 *  		(standard: random vector with length equal to the temperature)
 *  	downgrade: callable which chooses whether to downgrade the current
 *  		solution to a less optimal one (standard: inverse exponential to
 *  		objective value reduction) 
 *
 *  Returns:
 *  	tuple consisting of 3 values: 
 */
PNT_LIB int pntfSimAnn(float *restrict x,
                       unsigned int dim,
                       float fx,
                       pntfSAObjective f,
                       unsigned int nmax,
                       float fmin,
                       pntfSANeighbour neighbour,
                       void *restrict additionalData,
                       pntfSAResult *result);


#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_SIMULATED_ANNEALING_H
