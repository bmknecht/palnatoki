//
//
//  Simulated Annealing Implementation
//
//
#include "../internal/types.h"
#include "../internal/math.h"

#define Neighbour(name) int (*name)(param, scalar, param*)
#define Downgrade(name) int (*name)(scalar, scalar, scalar, int*)

//
//  local functions
//
static int _temperature(int i, int nmax, scalar *t)
{
    if(!t)
        return PNT_LOGIC_ERROR;
    if(nmax == i)
        return PNT_LOGIC_ERROR;
    *t = 1.f - (scalar)i / (scalar)nmax;
    return PNT_SUCCESS;
}


static int _downgrade(scalar fx, scalar fx1, scalar t,
                      int *result)
{
    if(!result)
        return PNT_LOGIC_ERROR;
    scalar randomValue = (scalar)rand() / (scalar)RAND_MAX;
    *result = math_exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}


static int _chooseNextX(param *x, scalar *fx, param x1,
                        scalar fx1, scalar t,
                        Downgrade(downgrade))
{
    if(!downgrade || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(fx1 < fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}


static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional,
                      Neighbour(neighbour),
                      param *x, scalar *fx)
{
    if(!neighbour || !optional || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(i >= nmax)
        return PNT_LOGIC_ERROR;

    scalar t;
    param x1;
    RETHROW(optional->temperature(i, nmax, &t));
    RETHROW(neighbour(x, t, &x1));

    scalar fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}


int _verifyParameters(Optional *optional)
{
    if(!optional->temperature)
        optional->temperature = _temperature;
    if(!optional->downgrade)
        optional->downgrade = _downgrade;
    return PNT_SUCCESS;
}


//
//  main function
//
int cppsSimAnn(param x, scalar fx, Objective f,
               const int nmax, scalar fmin,
               Neighbour(neighbour),
               Optional *optional,
               Result *result)
{
    // parameter check
    if(!result || !f || !neighbour || !optional)
        return PNT_INVALID_PARAMETER;

    // setup
    _verifyParameters(optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;

    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, x, fx));

        // new best parameter ?
        if(result->fx > fx)
        {
            result->x = x;
            result->fx = fx;
        }

        // reached goal ?
        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }

    return PNT_SUCCESS;
}

