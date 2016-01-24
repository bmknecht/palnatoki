#ifndef PALNATOKI_SIMULATED_ANNEALING_H
#define PALNATOKI_SIMULATED_ANNEALING_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

//
//  Neighbour function type definitions
//
typedef int (*pntfSANeighbour)(float *restrict x, int dim, float t,
                               float *restrict x1);
typedef int (*pntdSANeighbour)(double *restrict x, int dim, double t,
                               double *restrict x1);
typedef int (*pntldSANeighbour)(long double *restrict x, int dim,
                                long double t, long double *restrict x1);


//
//  Objective function type definitions
//
typedef int (*pntfSAObjective)(float *restrict x, int n, float *restrict fx);
typedef int (*pntdSAObjective)(double *restrict x, int n,
                               double *restrict fx);
typedef int (*pntldSAObjective)(long double *restrict x, int n,
                                long double *restrict fx);



//
// Optional structure type definitions
//
typedef struct _pntfSAOptional
{
	int (*temperature)(int i, int nmax, float *t);
	int (*downgrade)(float fx, float fxn, float temperature, int *result);
} pntfSAOptional;
extern const pntfSAOptional pntfSAOptionalDefault;


typedef struct _pntdSAOptional
{
	int (*temperature)(int i, int nmax, double *t);
	int (*downgrade)(double fx, double fxn, double temperature,
                     int *result);
} pntdSAOptional;
extern const pntdSAOptional pntdSAOptionalDefault;


typedef struct _pntldSAOptional
{
	int (*temperature)(int i, int nmax, long double *t);
	int (*downgrade)(long double fx, long double fxn,
                     long double temperature, int *result);
} pntldSAOptional;
extern const pntldSAOptional pntldSAOptionalDefault;



//
//  Result structure type definitions
//
typedef struct _pntfSAResult
{
	int iterations;
	float *x;
    int dim;
	float fx;
} pntfSAResult;


typedef struct _pntdSAResult
{
	int iterations;
	double *x;
    int dim;
	double fx;
} pntdSAResult;


typedef struct _pntldSAResult
{
	int iterations;
	long double *x;
    int dim;
	long double fx;
} pntldSAResult;


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
PNT_LIB int pntfSimAnn(float *x,
                       int dim,
                       float fx,
                       pntfSAObjective f,
                       const int nmax,
                       float fmin,
                       pntfSANeighbour neighbour,
                       const pntfSAOptional *optional,
                       pntfSAResult *result);

PNT_LIB int pntdSimAnn(double *x,
                       int dim,
                       double fx,
                       pntdSAObjective f,
                       const int nmax, double fmin,
                       pntdSANeighbour neighbour,
                       const pntdSAOptional *optional,
                       pntdSAResult *result);

PNT_LIB int pntldSimAnn(long double *x,
                         int dim,
                         long double fx,
                         pntldSAObjective f,
                         const int nmax,
                         long double fmin,
                         pntldSANeighbour neighbour,
                         const pntldSAOptional *optional,
                         pntldSAResult *result);


#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_SIMULATED_ANNEALING_H
