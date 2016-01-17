#ifdef __MINGW32__
#define _hypot hypot
#endif

#include <cmath>
#include <random>

#include "../internal/utility.hpp"
#include "simulated_annealing.h"


template<typename F>
using Neighbour = int (*)(F, F, F*);
template<typename F>
using Downgrade = int (*)(F, F, F, int*);


template<typename F>
static int _temperature(int i, int nmax, F *t)
{
    if(!t)
        return PNT_LOGIC_ERROR;
    if(nmax == i)
        return PNT_LOGIC_ERROR;
    *t = 1.f - (F)i / (F)nmax;
    return PNT_SUCCESS;
}


template<typename F>
static int _downgrade(F fx, F fx1, F t, int *result)
{
    if(!result)
        return PNT_LOGIC_ERROR;
    F randomValue = F(rand()) / F(RAND_MAX);
    *result = std::exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}


template<typename F>
static int _chooseNextX(F &x, F &fx, F x1, F fx1, F t,
                        Downgrade<F> downgrade)
{
    if(!downgrade)
        return PNT_LOGIC_ERROR;
    if(fx1 < fx)
    {
        x = x1;
        fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result)
        {
            x = x1;
            fx = fx1;
        }
    }
    return PNT_SUCCESS;
}


template<typename F, typename O>
static int _iteration(sObjective<F> f, int i, const int nmax,
                      const O &optional, Neighbour<F> neighbour,
                      F &x, F &fx)
{
    if(!neighbour)
        return PNT_LOGIC_ERROR;
    if(i >= nmax)
        return PNT_LOGIC_ERROR;

    F t, x1;
    RETHROW(optional.temperature(i, nmax, &t));
    RETHROW(neighbour(x, t, &x1));

    F fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional.downgrade));
    return PNT_SUCCESS;
}


template<typename O>
int _verifyParameters(O &optional)
{
    if(!optional.temperature)
        optional.temperature = _temperature;
    if(!optional.downgrade)
        optional.downgrade = _downgrade;
    return PNT_SUCCESS;
}


template<typename F, typename O, typename R>
int cppsSimAnn(F x, F fx, sObjective<F> f, const int nmax, F fmin,
               Neighbour<F> neighbour, O *optional, R *result)
{
    // parameter check
    if(!result)
        return PNT_INVALID_PARAMETER;
    if(!f)
        return PNT_INVALID_PARAMETER;
    if(!neighbour)
        return PNT_INVALID_PARAMETER;
    if(!optional)
        return PNT_LOGIC_ERROR;

    // setup
    _verifyParameters(*optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;

    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, *optional, neighbour, x, fx));

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


extern "C"
{
    int pntSsSimAnn(float x, float fx, pntSsObjective f,
                    const int nmax, float fmin,
                    pntSsSANeighbour neighbour,
                    pntSsSAOptional *optional, pntSsSAResult *result)
    {
        pntSsSAOptional backup;
        if(!optional)
            optional = &backup;
        return cppsSimAnn(x, fx, f, nmax, fmin, neighbour, optional,
                          result);
    }


    int pntDsSimAnn(double x, double fx, pntDsObjective f,
                    const int nmax, double fmin,
                    pntDsSANeighbour neighbour,
                    pntDsSAOptional *optional, pntDsSAResult *result)
    {
        pntDsSAOptional backup;
        if(!optional)
            optional = &backup;
        return cppsSimAnn(x, fx, f, nmax, fmin, neighbour, optional,
                          result);
    }


    int pntLDsSimAnn(long double x, long double fx, pntLDsObjective f,
                     const int nmax, long double fmin,
                     pntLDsSANeighbour neighbour,
                     pntLDsSAOptional *optional,
                     pntLDsSAResult *result)
    {
        pntLDsSAOptional backup;
        if(!optional)
            optional = &backup;
        return cppsSimAnn(x, fx, f, nmax, fmin, neighbour, optional,
                          result);
    }
}

