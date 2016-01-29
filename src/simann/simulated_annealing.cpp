//
//  Simulated Annealing Implementation
//
#include "../internal/compiler_specifics.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <random>

#include "../internal/utility.hpp"
#include <palnatoki/include/simulated_annealing.h>


//
// standard functions
//
template<typename T>
static inline T linearTemperature(unsigned int i, unsigned int itermax,
                                  T oldTemperature) {
    return std::max(oldTemperature - T(1) / T(itermax), T(0));
}


template<typename T>
static inline int downgrade(T fx, T fxnew, T t) {
    T randomValue = T(rand()) / T(RAND_MAX);
    return std::exp((fx - fxnew)/t) > randomValue;
}


//
//  local functions
//
template<typename T>
static inline T chooseNextX(T *restrict *restrict x, T fx,
                            T *restrict *restrict xnew, T fxnew, T t,
                            int (*downgrade)(T, T, T)) {
    assert(x && *x && xnew && *xnew);
    if(fxnew < fx) {
        std::swap(*x, *xnew);
        return fxnew;
    }
    else if(downgrade(fx, fxnew, t)) {
        std::swap(*x, *xnew);
        return fxnew;
    }
    return fx;
}


template<typename TParameters, typename TResult>
static bool verifyParameters(TParameters *parameters, TResult *result) {
    if(!parameters || !result) {
        return false;
    }

    if(!parameters->x || parameters->n <= 0 || !parameters->neighbour ||
       !parameters->f || !parameters->work) {
        return false;
    }

    if(!result->x) {
        return false;
    }
    return true;
}




//
//  main function
//
template<typename TFloat, typename TParameters, typename TResult>
void cppSimAnn(TParameters &parameters, TResult &result) {
    assert(verifyParameters(&parameters, &result));
    const unsigned int n = parameters.n;
    const unsigned int itermax = parameters.itermax;
    TFloat *restrict &x = parameters.x;
    TFloat *restrict &xnew = reinterpret_cast<TFloat*restrict&>(parameters.work);
    TFloat &fx = parameters.fx;

    // setup
    vec::copy(result.x, x, n);
    result.fx = fx;
    result.iterations = itermax;

    // iteration
    float t = 1.f;
    auto temperature = (parameters.temperature ? parameters.temperature :
                        ::linearTemperature<TFloat>);
    auto downgrade = (parameters.downgrade ? parameters.downgrade : 
                      ::downgrade<TFloat>);
    for(unsigned int i = 0; i < itermax; i++) {
        // compute and evaluate new parameters
        t = temperature(i, itermax, t);
        parameters.neighbour(x, n, t, xnew);
        TFloat fxnew = parameters.f(xnew, n);
        fx = chooseNextX(&x, fx, &xnew, fxnew, t, downgrade);

        // new best parameter ?
        if(result.fx > fx) {
            vec::copy(result.x, x, n);
            result.fx = fx;
        }

        // reached goal ?
        if(fx <= parameters.fmin) {
            result.iterations = i;
            break;
        }
    }
}


extern "C" {
    int pntfSimAnn(pntfSAParameters *parameters,
                   pntfSAResult *result) {
        BEGIN_GUARD_CPP_CODE {
            if(!verifyParameters(parameters, result)) {
                return PNT_INVALID_PARAMETER;
            }

            cppSimAnn<float>(*parameters, *result);
        } END_GUARD_CPP_CODE;
        return PNT_SUCCESS;
    }
}

