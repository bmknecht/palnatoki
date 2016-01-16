#ifndef PALNATOKI_SIMULATED_ANNEALING_H
#define PALNATOKI_SIMULATED_ANNEALING_H

#include "../common/common.h"


typedef int (*pntSsSATemp)(int i, int nmax, float *t);
typedef int (*pntDsSATemp)(int i, int nmax, double *t);
typedef int (*pntLDsSATemp)(int i, int nmax, long double *t);

typedef int (*pntSsSANeighbour)(float x, float t, float *x1);
typedef int (*pntDsSANeighbour)(double x, double t, double *x1);
typedef int (*pntLDsSANeighbour)(long double x, long double t,
                                 long double *x1);

typedef int (*pntSsSADowngrade)(float fx, float fxn, float temp, int *result);
typedef int (*pntDsSADowngrade)(double fx, double fxn, double temp,
                                int *result);
typedef int (*pntLDsSADowngrade)(long double fx, long double fxn,
                                 long double temp, int *result);


typedef struct _pntSsSAOptional
{
	pntSsBoundary xmin;
	pntSsBoundary xmax;
	pntSsSATemp temp;
	pntSsSANeighbour neighbour;
	pntSsSADowngrade downgrade;
} pntSsSAOptional;
void pntSsSAOptionalReset(pntSsSAOptional*);


typedef struct _pntSsSAResult
{
	int iterations;
	float x;
	float fx;
} pntSsSAResult;


typedef struct _pntDsSAOptional
{
	pntDsBoundary xmin;
	pntDsBoundary xmax;
	pntDsSATemp temp;
	pntDsSANeighbour neighbour;
	pntDsSADowngrade downgrade;
} pntDsSAOptional;
const pntDsSAOptional pntDsSAOptionalStd;


typedef struct _pntDsSAResult
{
	int iterations;
	double x;
	double fx;
} pntDsSAResult;


typedef struct _pntLDsSAOptional
{
	pntLDsBoundary xmin;
	pntLDsBoundary xmax;
	pntLDsSATemp temp;
	pntLDsSANeighbour neighbour;
	pntLDsSADowngrade downgrade;
} pntLDsSAOptional;
const pntLDsSAOptional pntLDsSAOptionalStd;


typedef struct _pntLDsSAResult
{
	int iterations;
	long double x;
	long double fx;
} pntLDsSAResult;


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
int pntSsSimAnn(float x, float fx, pntSsSAObjective f, const int nmax,
                float fmin, pntSsSANeighbour neighbour,
                pntSsSAOptional *optional, pntSsSAResult *result);
int pntDsSimAnn(double x, double fx, pntDsSAObjective f, const int nmax,
                double fmin, pntDsSANeighbour neighbour,
                pntDsSAOptional *optional, pntDsSAResult *result);
int pntLDsSimAnn(long double x, long double fx, pntLDsSAObjective f,
                 const int nmax, long double fmin, pntLDsSANeighbour neighbour,
                 pntLDsSAOptional *optional, pntLDsSAResult *result);


#endif // PALNATOKI_SIMULATED_ANNEALING_H
