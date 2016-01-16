#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../simann/simulated_annealing.h"
#include "../objective/objective.h"
#include "common.h"


int auckleyNeighbour(float x, float t, float *x1)
{
    assert(x1);
    float r = 2.f * (float)rand() / (float)RAND_MAX - 1.f;
    *x1 = t * r + x;
    return 0;
}


int testXmin(float *x)
{
    assert(x);
    if(*x < 2.f)
        *x = 2.f;
    return 0;
}


int testXmax(float *x)
{
    assert(x);
    if(*x > -2.f)
        *x = -2.f;
    return 0;
}


int identityNeighbour(float x, float t, float *x1)
{
    assert(x1);
    *x1 = x;
    return 0;
}


int failingNeighbour(float x, float t, float *x1)
{
    assert(x1);
    return 101;
}


int downgradeTrue(float fx, float fxn, float temp, int *result)
{
    assert(result);
    *result = 1;
    return 0;
}


int failingObjective(float x, float *fx)
{
    assert(fx);
    return 101;
}


int increasingObjective(float x, float *fx)
{
    assert(fx);
    static float oldFx = 1e6;
    *fx = oldFx;
    oldFx += 1.f;
    return 0;
}


int failingTemp(int i, int nmax, float *t)
{
    assert(t);
    return 101;
}


int main()
{
    srand(time(0));
    pntSsSAOptional optional;
    pntSsSAOptionalReset(&optional);
    pntSsSAResult result;
    float fx;

    assert(ssAuckley(5.f, &fx) == 0);
    int res = pntSsSimAnn(5.f, fx, ssAuckley, 100000, 0.f, auckleyNeighbour,
                          &optional, &result);
    TEST(res == PNT_SUCCESS);
    TEST(result.fx < fx);
    TEST(fabsf(result.x) <= 5.f);

    res = pntSsSimAnn(5.f, fx, ssAuckley, 1000, fx-1.f, auckleyNeighbour,
                      &optional, &result);
    TEST(res == PNT_SUCCESS);
    TEST(result.iterations == 1000 || result.fx < fx-1.f);

    optional.xmin = testXmin;
    res = pntSsSimAnn(5.f, fx, ssAuckley, 1000, 0.f, auckleyNeighbour,
                      &optional, &result);
    TEST(res == PNT_SUCCESS);
    TEST(result.x >= 2.f);

    pntSsSAOptionalReset(&optional);
    optional.xmax = testXmax;
    res = pntSsSimAnn(-5.f, fx, ssAuckley, 1000, 0.f, auckleyNeighbour,
                      &optional, &result);
    TEST(res == PNT_SUCCESS);
    TEST(result.x >= -2.f);

    pntSsSAOptionalReset(&optional);
    res = pntSsSimAnn(-5.5f, fx, ssAuckley, 1000, 0.f, identityNeighbour,
                      &optional, &result);
    TEST(res == PNT_SUCCESS);
    TEST(result.x == -5.5f);

    res = pntSsSimAnn(-5.5f, fx, ssAuckley, 1000, 0.f, failingNeighbour,
                      &optional, &result);
    TEST(res == 101);

    res = pntSsSimAnn(-5.f, fx, failingObjective, 1000, 0.f,
                      auckleyNeighbour, &optional, &result);
    TEST(res == 101);

    optional.downgrade = downgradeTrue;
    res = pntSsSimAnn(-5.f, fx, increasingObjective, 1000, 0.f,
                      auckleyNeighbour, &optional, &result);
    TEST(result.x == -5.f);
    TEST(result.iterations == 1000);

    pntSsSAOptionalReset(&optional);
    optional.temp = failingTemp;
    res = pntSsSimAnn(-5.f, fx, ssAuckley, 1000, 0.f, auckleyNeighbour,
                      &optional, &result);
    TEST(fabsf(result.x) >= 5.f);
    TEST(result.iterations == 1000);
    TEST(result.fx == fx);
    TEST(res == 101);
}
