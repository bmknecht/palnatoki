#include "numdiff.h"
#include "../internal/utility.h"


template<typename T, typename F>
int pntSsCentralDerivative2(T x, T fx, F f, T h, T *fp)
{
    if(!fp)
        return PNT_INVALID_PARAMETER;
    if(!f)
        return PNT_INVALID_PARAMETER;
    float f1, f2;
    RETHROW(f(x+h, &f1));
    RETHROW(f(x-h, &f2));
    *fp = (f1 - 2*fx + f2) / (h*h);
    return PNT_SUCCESS;
}


int pntSsCentralDerivative1(float x, float fx, pntSsObjective f, float h,
                            float *fp)
{
    if(!fp)
        return PNT_INVALID_PARAMETER;
    if(!f)
        return PNT_INVALID_PARAMETER;
    float f1, f2;
    RETHROW(f(x+h, &f1));
    RETHROW(f(x-h, &f2));
    *fp = (f1 - f2) / (2*h);
    return PNT_SUCCESS;
}


int pntSsForwardDerivative1(float x, float fx, pntSsObjective f, float h,
                            float *fp)
{
    if(!fp)
        return PNT_INVALID_PARAMETER;
    if(!f)
        return PNT_INVALID_PARAMETER;
    float f1;
    RETHROW(f(x+h, &f1));
    *fp = (f1 - fx) / h;
    return PNT_SUCCESS;
}


int pntSsBackwardDerivative1(float x, float fx, pntSsObjective f, float h,
                             float *fp)
{
    if(!fp)
        return PNT_INVALID_PARAMETER;
    if(!f)
        return PNT_INVALID_PARAMETER;
    float f2;
    RETHROW(f(x-h, &f2));
    *fp = (fx - f2) / h;
    return PNT_SUCCESS;
}
