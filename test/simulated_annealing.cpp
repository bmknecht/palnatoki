#ifdef __MINGW32__
#define _hypot hypot
#endif

#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>

#include "common.hpp"
#include "../objective/objective.h"
#include "../simann/simulated_annealing.cpp"


template<typename F>
int auckleyNeighbour(F x, F t, F *x1)
{
    assert(x1);
    F r = 2.f * (F)rand() / (F)RAND_MAX - 1.f;
    *x1 = t * r + x;
    return 0;
}


template<typename F>
int failingNeighbour(F x, F t, F *x1)
{
    assert(x1);
    return 101;
}


template<typename F>
int failingDowngrade(F fx, F fxn, F temp, int *result)
{
    assert(result);
    return 101;
}


template<typename F>
int failingObjective(F x, F *fx)
{
    assert(fx);
    return 101;
}


template<typename F>
int failingTemperature(int i, int nmax, F *t)
{
    assert(t);
    return 101;
}


void testFunctionalityS()
{
    pntSsSAOptional optional = {nullptr, nullptr};
    pntSsSAResult result;
    float fx;
    assert(ssAuckley(5.f, &fx) == 0);
    TEST(pntSsSimAnn(5.f, fx, ssAuckley, 100000, 0.f, auckleyNeighbour,
                     &optional, &result) == PNT_SUCCESS);
    TEST(result.fx < fx);
    TEST(fabsf(result.x) <= 5.f);

    TEST(pntSsSimAnn(5.f, fx, ssAuckley, 100000, 0.f, failingNeighbour,
                      &optional, &result) == 101);

    optional.downgrade = failingDowngrade;
    TEST(pntSsSimAnn(5.f, fx, ssAuckley, 100000, 0.f, auckleyNeighbour,
                      &optional, &result) == 101);

    optional.downgrade = nullptr;
    optional.temperature = failingTemperature;
    TEST(pntSsSimAnn(5.f, fx, ssAuckley, 100000, 0.f, auckleyNeighbour,
                      &optional, &result) == 101);

    optional.temperature = nullptr;
    TEST(pntSsSimAnn(5.f, fx, failingObjective, 100000, 0.f, auckleyNeighbour,
                      &optional, &result) == 101);
}


int main()
{
    srand(time(0));
    testFunctionalityS();
    std::cout << "tests done" << std::endl;
}
