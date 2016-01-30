/** This file is part of the Palnatoki optimization library. For licensing
 *  information refer to the LICENSE file that is included in the project.
 *
 *  This file in particular contains the implementation of the Simulated
 *  Annealing algorithm.
 */
#include "../internal/compiler_specifics.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <random>

#include "../internal/utility.hpp"
#include <palnatoki/include/simulated_annealing.h>


//
// standard implementations of optional functions
//

/** linear decreasing temperature, starting at 1 (old temperature) and
 * evenly decreasing to 0
 */
template<typename T>
static inline T linearTemperature(unsigned int i, unsigned int itermax,
                                  T oldTemperature) {
    // we could compute the new temperature directly by (1 - i / itermax)
    // but that way we could not reset the temperature (maybe added in the
    // future)
    return std::max(oldTemperature - T(1) / T(itermax), T(0));
}


/** returns true if the new (worse) parameters should be accepted
 */
template<typename T>
static inline int downgrade(T fx, T fxnew, T t) {
    T randomValue = T(rand()) / T(RAND_MAX);
    return std::exp((fx - fxnew)/t) > randomValue;
}


//
//  local functions
//

/**
 * swaps x and xnew if either fxnew is lower than fx or if @ref downgrade
 * returns true.
 * @return The function value associated with the parameters that are
 *  stored in x is returned.
 */
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


/** In particular ensures that all pointers in both the parameter and the
 *  result structure are non-null.
 *  @return True if all values in the parameters and the result structure
 *   are correct, else false
 */
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

/** Templated C++ function of the C functions that are called by the user.
 *  This enables a more compact implementation.
 */
template<typename TFloat, typename TParameters, typename TResult>
void cppSimAnn(TParameters *parameters, TResult *result) {
    if(!verifyParameters(parameters, result)) {
        throw PNT_INVALID_PARAMETER;
    }

    // save much needed values for readability
    const unsigned int n = parameters->n;
    const unsigned int itermax = parameters->itermax;
    TFloat *restrict &x = parameters->x;
    TFloat *restrict &xnew = reinterpret_cast<TFloat*restrict&>(
                                parameters->work);
    TFloat &fx = parameters->fx;

    // setup results values
    vec::copy(result->x, x, n);
    result->fx = fx;
    result->iterations = 0;

    // set optional parameters
    auto temperature = (parameters->temperature ? parameters->temperature :
                        ::linearTemperature<TFloat>);
    auto downgrade = (parameters->downgrade ? parameters->downgrade : 
                      ::downgrade<TFloat>);

    float t = 1.f;
    for(unsigned int i = 0; i < itermax; i++) {
        // compute and evaluate new parameters
        t = temperature(i, itermax, t);
        parameters->neighbour(x, n, t, xnew);
        TFloat fxnew = parameters->f(xnew, n);
        fx = chooseNextX(&x, fx, &xnew, fxnew, t, downgrade);

        // new parameter is best parameter?
        if(result->fx > fx) {
            vec::copy(result->x, x, n);
            result->fx = fx;
        }

        // reached goal ?
        if(fx <= parameters->fmin) {
            result->iterations = i;
            break;
        }
    }
    result->iterations = itermax;
}


//
// C wrappers for C++ functions
//
extern "C" {
    int pntfSimAnn(pntfSAParameters *parameters,
                   pntfSAResult *result) {
        BEGIN_GUARD_CPP_CODE {
            cppSimAnn<float>(*parameters, *result);
            return PNT_SUCCESS;
        } END_GUARD_CPP_CODE;
    }
}

