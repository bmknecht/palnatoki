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
//  typedefs
//
template<typename T>
using Neighbour = void (*)(const T*restrict, unsigned int, T, T*restrict);
template<typename T>
using Objective = T (*)(const T*restrict, unsigned int);


template<typename T>
static inline T pntSALinearTemperature(unsigned int i, unsigned int nmax,
                                       T oldTemperature) {
    return oldTemperature - T(1) / T(nmax);
}


template<typename T>
static inline T pntSAExponentialTemperature(unsigned int i, unsigned int nmax,
                                            T oldTemperature) {
    return 0.95 * oldTemperature;
}


template<typename T>
static inline bool downgrade(T fx, T fx1, T t) {
    T randomValue = T(rand()) / T(RAND_MAX);
    return (std::exp(fx/t) / std::exp(fx1/t)) > randomValue;
}


template<typename T>
static inline void chooseNextX(T *restrict *restrict x, T &fx,
                               T *restrict *restrict x1, T fx1, T *t) {
    assert(x && *x && x1 && *x1 && t);
    if(fx1 < fx) {
        std::swap(*x, *x1);
        fx = fx1;
    }
    else if(downgrade(fx, fx1, *t)) {
        std::swap(*x, *x1);
        fx = fx1;
    }
}


template<typename T>
static void iteration(Objective<T> f, unsigned int i,
                      unsigned int nmax,
                      Neighbour<T> neighbour,
                      T *restrict *restrict x, T *restrict *restrict x1,
                      unsigned int n, T &fx, unsigned int *stuckIterations,
                      float *t) {
    assert(f && neighbour && i < nmax && stuckIterations && t);

}


//
//  main function
//
template<typename TFloat, typename TResult>
void cppSimAnn(TFloat *restrict x, TFloat *restrict x1, unsigned int n,
               TFloat fx, Objective<TFloat> f, unsigned int nmax,
               TFloat fmin, Neighbour<TFloat> neighbour, TResult &result) {
    // parameter check
    assert(x && x1 && f && neighbour);

    // setup
    vec::copy(result.x, x, n);
    result.fx = fx;
    result.iterations = nmax;

    // iteration
    unsigned int stuckIterations = 0;
    float temperature = 1.f;
    for(unsigned int i = 0; i < nmax; i++) {
        *t = temperature<T>(i, nmax, *t);
        neighbour(*x, n, *t, *x1);

        T fx1 = f(*x1, n);
        chooseNextX(x, fx, x1, fx1, t, stuckIterations);

        // new best parameter ?
        if(result.fx > fx) {
            vec::copy(result.x, x, n);
            result.fx = fx;
        }

        // reached goal ?
        if(fx <= fmin) {
            result.iterations = i;
            break;
        }
    }
}


extern "C" {
    int pntfSimAnn(float *restrict x, unsigned int n, float fx,
                   pntfSAObjective f, unsigned int itermax, float fmin,
                   pntfSANeighbour neighbour,
                   void *restrict data, pntfSAResult *result) {
        BEGIN_GUARD_CPP_CODE {
            if(!x || !f || !neighbour || !data || !result) {
                return PNT_INVALID_PARAMETER;
            }

            float *restrict x1 = reinterpret_cast<float *restrict>(data);
            cppSimAnn<float>(x, x1, n, fx, f, itermax, fmin, neighbour,
                             *result);
        } END_GUARD_CPP_CODE;
        return PNT_SUCCESS;
    }
}

