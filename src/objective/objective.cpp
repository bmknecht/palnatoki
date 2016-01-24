#include <cassert>
#include "../internal/utility.hpp"
#include <cmath>

#include <palnatoki/include/objective.h>

static const float pif = 3.1415926535898f;


template<class T>
int sAckley(Vector<T> &x, T *fx)
{
    assert(fx);
    const T a = 20.;
    const T b = 0.2;
    const T c = 2*pif;

    T cosSum = 0.;
    for(int i=0; i < size(x); i++) {
        cosSum += std::cos(c*x[i]);
    }

    *fx = -a*std::exp(-b*std::sqrt(sqrSum(x) / (T)size(x)))
          - std::exp(cosSum / (T)size(x)) + a + std::exp(1.f);
    return 0;
}

extern "C" {
    int fAckley(float *x, int n, float *fx) {
        Vector<float> xv;
        xv.borrowFrom(x, n);
        return sAckley(xv, fx);
    }

    int dAckley(double *x, int n, double *fx) {
        Vector<double> xv;
        xv.borrowFrom(x, n);
        return sAckley(xv, fx);
    }

    int ldAckley(long double *x, int n, long double *fx) {
        Vector<long double> xv;
        xv.borrowFrom(x, n);
        return sAckley(xv, fx);
    }
}

