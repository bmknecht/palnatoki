#include <fstream>
#include "common.hpp"
#include <palnatoki/include/objective.h>
#include <palnatoki/include/simulated_annealing.h>
#include <palnatoki/src/simann/simulated_annealing.cpp>


template<typename T>
struct ParametersStruct {};

template<>
struct ParametersStruct<float> {
    typedef pntfSAParameters type;
};


template<typename T>
struct ResultStruct {};

template<>
struct ResultStruct<float> {
    typedef pntfSAResult type;
};


template<typename T>
struct SimAnnStruct {};

template<>
struct SimAnnStruct<float> {
    static int func(pntfSAParameters *p, pntfSAResult *r) {
        return pntfSimAnn(p, r);
    }
};


template<typename T>
T ackley(const T*restrict, unsigned int);

template<>
float ackley<float>(const float *restrict x, unsigned int n) {
    return fAckley(x, n);
}


template<typename T>
static void testLinearTemperature() {
    TEST_OP(std::abs(linearTemperature<T>(0, 100, 1.f) - 0.99), <, 1e-8);
    TEST_OP(std::abs(linearTemperature<T>(1, 100, 1.f) - 0.99), <, 1e-8);
    TEST_OP(std::abs(linearTemperature<T>(0, 100, 0.5f) - 0.49), <, 1e-8);
    TEST_OP(std::abs(linearTemperature<T>(1, 100, 0.5f) - 0.49), <, 1e-8);
    TEST_OP(std::abs(linearTemperature<T>(0, 100, 0.01f) - 0), <, 1e-8);
    TEST_OP(std::abs(linearTemperature<T>(1, 100, 0.01f) - 0), <, 1e-8);
}


template<typename T>
static void testDowngrade() {
    // random function therefore we test for probability
    unsigned int succeeded = 0;
    for(unsigned int i = 0; i < 1e6; i++) {
        T a = std::log(0.5) + 1;
        // downgrade succeedes 50% of the time with the chosen parameters
        if(downgrade<T>(a, 1, 1)) {
            succeeded++;
        }
    }

    TEST_OP(std::abs(succeeded - 1e6 / 2), <, 2000);
}


template<typename T>
static int downgradeNever(T, T, T) {
    return 0;
}


template<typename T>
static int downgradeAlways(T, T, T) {
    return 1;
}


template<typename T>
static void testChooseNextX() {
    float x = -1.f;
    float *xp = &x;
    float x1 = 1.f;
    float *x1p = &x1;

    // should always pick new value (-1.f)
    TEST_EQ(chooseNextX(&xp, 1.f, &x1p, -1.f, 1.f, downgradeNever), -1.f);
    TEST_EQ(xp, &x1);
    TEST_EQ(x1p, &x);
    TEST_EQ(chooseNextX(&xp, 10.f, &x1p, -1.f, 1.f, downgradeNever), -1.f);
    TEST_EQ(xp, &x);
    TEST_EQ(x1p, &x1);

    // should pick new value
    TEST_EQ(chooseNextX(&xp, 1.f, &x1p, 2.f, 1.f, downgradeAlways), 2.f);
    TEST_EQ(xp, &x1);
    TEST_EQ(x1p, &x);

    // should not pick new value
    TEST_EQ(chooseNextX(&xp, 1.f, &x1p, 2.f, 1.f, downgradeNever), 1.f);
    TEST_EQ(xp, &x1);
    TEST_EQ(x1p, &x);
}


template<typename T>
static void testVerifyParameters() {
    typedef typename ParametersStruct<T>::type PS;
    PS parameters;
    typedef typename ResultStruct<T>::type RS;
    RS result;
    TEST_EQ((verifyParameters<PS, RS>(nullptr, &result)), false);
    TEST_EQ((verifyParameters<PS, RS>(&parameters, nullptr)), false);

    std::memset(&parameters, 1, sizeof(parameters));
    std::memset(&result, 1, sizeof(result));
    TEST_EQ(verifyParameters(&parameters, &result), true);
    
    result.x = nullptr;
    TEST_EQ(verifyParameters(&parameters, &result), false);
    result.x = reinterpret_cast<T*>(1);

    parameters.x = nullptr;
    TEST_EQ(verifyParameters(&parameters, &result), false);
    parameters.x = reinterpret_cast<T*>(1);

    parameters.f = nullptr;
    TEST_EQ(verifyParameters(&parameters, &result), false);
    parameters.f = reinterpret_cast<T(*)(const T*, unsigned int)>(1);

    parameters.neighbour = nullptr;
    TEST_EQ(verifyParameters(&parameters, &result), false);
    parameters.neighbour = reinterpret_cast<void (*)(const T*,
                                                     unsigned int, T,
                                                     T*)>(1);

    parameters.work = nullptr;
    TEST_EQ(verifyParameters(&parameters, &result), false);
    parameters.work = reinterpret_cast<T*>(1);

    parameters.temperature = nullptr;
    TEST_EQ(verifyParameters(&parameters, &result), true);
    parameters.downgrade = nullptr;
    TEST_EQ(verifyParameters(&parameters, &result), true);
    parameters.temperature = reinterpret_cast<T(*)(unsigned int,
                                                   unsigned int, T)>(1);
    TEST_EQ(verifyParameters(&parameters, &result), true);
}


static bool neighbourTestCalled = true;
static unsigned int neighbourTestPassedN = -1;
static const float *neighbourTestPassedX = nullptr;
static const float *neighbourTestPassedXNew = nullptr;
template<typename T>
void neighbourTest(const T *restrict x, unsigned int n, T,
                   T *restrict xnew) {
    neighbourTestCalled = true;
    neighbourTestPassedN = n;
    neighbourTestPassedX = x;
    neighbourTestPassedXNew = xnew;
}


static bool temperatureTestCalled = false;
static unsigned int temperatureTestPassedIterMax = -1;
static unsigned int temperatureTestPassedI = -1;
template<typename T>
T temperatureTest(unsigned int i, unsigned int itermax, T oldTemperature) {
    temperatureTestCalled = true;
    temperatureTestPassedIterMax = itermax;
    temperatureTestPassedI = i;
    return 0;
}


static bool downgradeTestCalled = false;
template<typename T>
static int downgradeTest(T fx, T fxnew, T t) {
    downgradeTestCalled = true;
    return 1;
}


template<typename T>
static void testParameterIntegration(unsigned int n) {
    typename ParametersStruct<T>::type parameters;
    parameters.x = new T[n];
    std::memset(parameters.x, 0, n * sizeof(T));
    auto savex = parameters.x;
    parameters.f = ackley;
    parameters.n = n;
    parameters.fx = ackley(parameters.x, n);
    parameters.itermax = 1;
    parameters.fmin = -1.;
    parameters.neighbour = neighbourTest;
    parameters.work = new T[n];
    std::memset(parameters.work, 0, n * sizeof(T));
    auto savework = parameters.work;
    parameters.temperature = temperatureTest;
    parameters.downgrade = downgradeTest;

    typename ResultStruct<T>::type result;
    result.x = new T[n];
    auto saveresultx = result.x;
    result.iterations = 0;

    neighbourTestCalled = false;
    temperatureTestCalled = false;
    downgradeTestCalled = false;

    TEST_EQ(SimAnnStruct<T>::func(&parameters, &result), PNT_SUCCESS);
    TEST_EQ(result.iterations, 1);
    TEST_EQ(neighbourTestCalled, true);
    TEST_EQ(neighbourTestPassedN, n);
    if(neighbourTestPassedX == parameters.x) {
        TEST_EQ(neighbourTestPassedXNew, parameters.work);
        TEST_EQ(savex, parameters.x);
        TEST_EQ(savework, parameters.work);
    }
    else {
        TEST_EQ(neighbourTestPassedX, parameters.work);
        TEST_EQ(neighbourTestPassedXNew, parameters.x);
        TEST_EQ(savex, parameters.work);
        TEST_EQ(savework, parameters.x);
    }
    TEST_EQ(temperatureTestCalled, true);
    TEST_EQ(temperatureTestPassedIterMax, 1);
    TEST_EQ(temperatureTestPassedI, 0);
    TEST_EQ(downgradeTestCalled, true);
    TEST_EQ(saveresultx, result.x);
    delete []result.x;
    delete []reinterpret_cast<T*>(parameters.work);
    delete []parameters.x;
}


template<typename T>
static void ackleyNeighbour(const T *restrict x, unsigned int n, T t,
                            T *restrict x1) {
    assert(x && x1);
    static int i = 0;
    std::memcpy(x1, x, n * sizeof(T));
    x1[i] = std::max(T(-15), std::min(x[i] + t * T(5.) *
                                      (T(rand()) / T(RAND_MAX) - T(0.5)),
                                      T(15.)));
    i++;
    if(i >= n) {
        i = 0;
    }
}


template<typename T>
static void testSimulatedAnnealing(const unsigned int n) {
    const unsigned int maxIterations = 10000 * n;
    const T fmin = 1e-10;
    typename ParametersStruct<T>::type parameters;
    std::memset(&parameters, 0, sizeof(parameters));
    parameters.x = new T[n];
    for(unsigned int i=0; i < n; i++) {
        parameters.x[i] = 15.f;
    }
    parameters.f = ackley;
    parameters.n = n;
    parameters.fx = ackley(parameters.x, n);
    parameters.itermax = maxIterations;
    parameters.fmin = 1e-5f;
    parameters.neighbour = ackleyNeighbour;
    parameters.work = new T[n];
    std::memset(parameters.work, 0, n * sizeof(T));

    typename ResultStruct<T>::type result;
    result.x = new T[n];
    TEST_EQ(SimAnnStruct<T>::func(&parameters, &result), PNT_SUCCESS);

    T sqrDiff = 0.f;
    for(unsigned int i=0; i < n; i++) {
        sqrDiff += result.x[i] * result.x[i];
    }
    TEST_OP(std::sqrt(sqrDiff), <, 0.1f);
    T fx = ackley(result.x, n);
    TEST_EQ(fx, result.fx);
    TEST_OP(result.fx, <, 0.1f);
    TEST_TRUE(fmin >= fx || result.iterations == maxIterations);

    delete []result.x;
    delete []reinterpret_cast<T*>(parameters.work);
    delete []parameters.x;
}


void testSimulatedAnnealing() {
    std::cout << "\ntesting simulated annealing (tests may fail due to the "
              << "random nature of the algorithm)\n";
    testLinearTemperature<float>();
    testDowngrade<float>();
    testChooseNextX<float>();
    testVerifyParameters<float>();
    std::cout << "\n\ttesting float - scalar\n";
    testParameterIntegration<float>(1);
    testSimulatedAnnealing<float>(1);
    std::cout << "\n\ttesting float - vector dim 20\n";
    testParameterIntegration<float>(20);
    testSimulatedAnnealing<float>(20);
}

