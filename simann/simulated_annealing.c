#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

#include "../internal/utility.h"
#include "simulated_annealing.h"


int _iteration(pntSsSAObjective f, int i, const int nmax,
               const pntSsSAOptional *optional, pntSsSANeighbour neighbour,
               float *x, float *fx);
int _chooseNextX(float *x, float *fx, float x1, float fx1, float t,
                 pntSsSADowngrade downgrade);
int _nextNeighbourInBounds(float x, float *x1, float t,
                           const pntSsSAOptional *optional,
                           pntSsSANeighbour neighbour);
void _setDefaultParameters(pntSsSAOptional *optional);
int _temperature(int i, int nmax, float *t);
int _downgrade(float fx, float fx1, float t, int *result);


int ssSimAnn(float x, float fx, pntSsSAObjective f, const int nmax,
             float fmin, pntSsSANeighbour neighbour,
             pntSsSAOptional *optional, pntSsSAResult *result)
{
    // parameter check
    if(!result)
        return PNT_INVALID_PARAMETER;
    if(!f)
        return PNT_INVALID_PARAMETER;
    if(!neighbour)
        return PNT_INVALID_PARAMETER;

    // setup
    _setDefaultParameters(optional);
    result->iterations = nmax;

    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, &x, &fx));

        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }

    // save result
    result->x = x;
    result->fx = fx;
    return PNT_SUCCESS;
}


int _iteration(pntSsSAObjective f, int i, const int nmax,
               const pntSsSAOptional *optional, pntSsSANeighbour neighbour,
               float *x, float *fx)
{
    assert(optional);
    assert(neighbour);
    assert(x);
    assert(fx);

    float t, x1;
    RETHROW(optional->temp(i, nmax, &t));
    RETHROW(_nextNeighbourInBounds(*x, &x1, t, optional, neighbour));

    float fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}


int _nextNeighbourInBounds(float x, float *x1, float t,
                           const pntSsSAOptional *optional,
                           pntSsSANeighbour neighbour)
{
    assert(x1);
    RETHROW(neighbour(x, t, x1));
    if(optional->xmin)
        RETHROW(optional->xmin(x1));
    if(optional->xmax)
        RETHROW(optional->xmax(x1));
    return PNT_SUCCESS;
}


void _setDefaultParameters(pntSsSAOptional *optional)
{
    assert(optional);
    if(optional->temp)
        optional->temp = _temperature;
    if(optional->downgrade)
        optional->downgrade = _downgrade;
}


int _temperature(int i, int nmax, float *t)
{
    assert(t);
    *t = 1.f - (float)i / (float)nmax;
    return PNT_SUCCESS;
}


int _downgrade(float fx, float fx1, float t, int *result)
{
    assert(result);
    float randomValue = (float)rand() / (float)(INT_MAX);
    *result = expf(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}


int _chooseNextX(float *x, float *fx, float x1, float fx1, float t,
                 pntSsSADowngrade downgrade)
{
    assert(x);
    assert(fx);
    assert(downgrade);
    if(fx1 < *fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(*fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}

