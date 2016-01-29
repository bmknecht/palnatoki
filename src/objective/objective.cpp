#include <cassert>
#include "../internal/utility.hpp"
#include <cmath>

#include <palnatoki/include/objective.h>

static const float pif = 3.1415926535898f;


template<class T>
T Ackley(const T *restrict x, unsigned int n) {
    const T a = 20.;
    const T b = 0.2;
    const T c = 2*pif;

    T cosSum = 0.;
    for(size_t i=0; i < n; i++) {
        cosSum += std::cos(c*x[i]);
    }

    return -a*std::exp(-b*std::sqrt(vec::sqrSum(x, n) / (T)n))
           - std::exp(cosSum / (T)n) + a + std::exp(1.f);
}

extern "C" {
    float fAckley(const float *restrict x, unsigned int n) {
        return Ackley(x, n);
    }

    double dAckley(const double *restrict x, unsigned int n) {
        return Ackley(x, n);
    }

    long double ldAckley(const long double *restrict x, unsigned int n) {
        return Ackley(x, n);
    }
}

