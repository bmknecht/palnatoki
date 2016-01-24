//
//  Simulated Annealing Implementation
//
#include "../internal/compiler_specifics.hpp"

#include <cassert>
#include <random>

#include "../internal/utility.hpp"
#include <palnatoki/include/simulated_annealing.h>


const pntfSAOptional pntfSAOptionalDefault = { nullptr, nullptr };
const pntdSAOptional pntdSAOptionalDefault = { nullptr, nullptr };
const pntldSAOptional pntldSAOptionalDefault = { nullptr, nullptr };

//
//  typedefs
//
template<typename F>
using Neighbour = int (*)(F*, int, F, F*);
template<typename F>
using Downgrade = int (*)(F, F, F, int*);
template<typename F>
using Objective = int (*)(F*, int, F*);

//
//  local functions
//
template<typename F>
static int temperature(int i, int nmax, F *t) {
    if(!t || nmax == i) {
        return PNT_LOGIC_ERROR;
    }
    *t = 1.f - (F)i / (F)nmax;
    return PNT_SUCCESS;
}


template<typename F>
static int downgrade(F fx, F fx1, F t, int *result) {
    if(!result) {
        return PNT_LOGIC_ERROR;
    }
    F randomValue = (F)rand() / (F)RAND_MAX;
    *result = std::exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}


template<typename F>
static int chooseNextX(Vector<F> &x, F &fx, Vector<F> &&x1, F fx1, F t,
                       Downgrade<F> downgrade) {
    if(!downgrade) {
        return PNT_LOGIC_ERROR;
    }

    if(fx1 < fx) {
        x = std::move(x1);
        fx = fx1;
    }
    else {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result) {
            x = std::move(x1);
            fx = fx1;
        }
    }
    return PNT_SUCCESS;
}


template<typename TFloat, typename TOptional>
static int iteration(Objective<TFloat> f, int i, const int nmax,
                     const TOptional &optional,
                     Neighbour<TFloat> neighbour,
                     Vector<TFloat> &x, TFloat &fx) {
    if(!neighbour || i >= nmax) {
        return PNT_LOGIC_ERROR;
    }

    TFloat t;
    Vector<TFloat> x1;
    x1.resize(size(x));
    RETHROW(optional.temperature(i, nmax, &t));
    RETHROW(neighbour(x.pointer(), size(x), t, x1.pointer()));

    TFloat fx1;
    RETHROW(f(x1.pointer(), size(x1), &fx1));
    RETHROW(chooseNextX(x, fx, std::move(x1), fx1, t, optional.downgrade));
    return PNT_SUCCESS;
}


template<typename T>
void verifyParameters(T &optional) {
    if(!optional.temperature)
        optional.temperature = temperature;
    if(!optional.downgrade)
        optional.downgrade = downgrade;
}


//
//  main function
//
template<typename TFloat, typename TResult, typename TOptional>
int cppSimAnn(Vector<TFloat> &x, TFloat fx,
              Objective<TFloat> f,
              const int nmax, TFloat fmin,
              Neighbour<TFloat> neighbour,
              TOptional &optional, TResult *result) {
    // parameter check
    if(!result || !f || !neighbour || !result)
        return PNT_LOGIC_ERROR;

    // setup
    verifyParameters(optional);
    Vector<TFloat> resultVector;
    resultVector.borrowFrom(result->x, size(x));
    resultVector = x;
    result->fx = fx;
    result->iterations = nmax;

    // iteration
    for(int i = 0; i < nmax; i++) {
        RETHROW(iteration(f, i, nmax, optional, neighbour, x, fx));

        // new best parameter ?
        if(result->fx > fx) {
            resultVector = x;
            result->fx = fx;
        }

        // reached goal ?
        if(fx <= fmin) {
            result->iterations = i;
            break;
        }
    }

    return PNT_SUCCESS;
}


extern "C" {
    int pntfSimAnn(float *xr, int n, float fx, pntfSAObjective f,
                   const int itermax, float fmin, pntfSANeighbour neighbour,
                   const pntfSAOptional *optional, pntfSAResult *result) {
        BEGIN_GUARD_CPP_CODE {
            if(!f || !neighbour || !result) {
                return PNT_INVALID_PARAMETER;
            }

            pntfSAOptional tempOptional(pntfSAOptionalDefault);
            if(optional) {
                tempOptional = *optional;
            }

            Vector<float> x;
            x.borrowFrom(xr, n);
            return cppSimAnn(x, fx, f, itermax, fmin, neighbour, tempOptional,
                             result);
        } END_GUARD_CPP_CODE;
    }


    int pntdSimAnn(double *xr, int n, double fx, pntdSAObjective f,
                   const int itermax, double fmin, pntdSANeighbour neighbour,
                   const pntdSAOptional *optional,
                   pntdSAResult *result) {
        BEGIN_GUARD_CPP_CODE {
            if(!f || !neighbour || !result) {
                return PNT_INVALID_PARAMETER;
            }

            pntdSAOptional tempOptional(pntdSAOptionalDefault);
            if(optional) {
                tempOptional = *optional;
            }
            Vector<double> x;
            x.borrowFrom(xr, n);
            return cppSimAnn(x, fx, f, itermax, fmin, neighbour, tempOptional,
                             result);
        } END_GUARD_CPP_CODE;
    }


    int pntldSimAnn(long double *xr, int n, long double fx,
                    pntldSAObjective f, const int itermax, long double fmin,
                    pntldSANeighbour neighbour,
                    const pntldSAOptional *optional,
                    pntldSAResult *result) {
        BEGIN_GUARD_CPP_CODE {
            if(!f || !neighbour || !result) {
                return PNT_INVALID_PARAMETER;
            }

            pntldSAOptional tempOptional(pntldSAOptionalDefault);
            if(optional) {
                tempOptional = *optional;
            }
            Vector<long double> x;
            x.borrowFrom(xr, n);
            return cppSimAnn(x, fx, f, itermax, fmin, neighbour, tempOptional,
                             result);
        } END_GUARD_CPP_CODE;
    }
}
