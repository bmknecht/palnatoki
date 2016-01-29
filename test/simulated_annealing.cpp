#include <fstream>
#include "common.hpp"
#include <palnatoki/include/simulated_annealing.h>


template<typename T>
T Ackley(const T *restrict, unsigned int);


template<typename T>
void AckleyNeighbour(const T *restrict x, unsigned int n, T t,
                     T *restrict x1) {
    assert(x && x1);
    for(unsigned int i=0; i < n; i++) {
        x1[i] = x[i] + t * 5. * (T(rand()) / T(RAND_MAX) - 0.5);
    }
}


template<typename T>
void AckleyNeighbourWide(const T *restrict x, unsigned int n, T t,
                         T *restrict x1) {
    assert(x && x1);
    static unsigned int element = 0;
    x1[element] = x[element] + t * 10. * (T(rand()) / T(RAND_MAX) - 0.5);
    element++;
    if(element >= n) {
        element = 0;
    }

    /*
    T dist = 0;
    T min = x1[0], max = x1[0];
    for(size_t i = 0; i < n; i++) {
        dist += x1[i];
        min = std::min(min, x1[i]);
        max = std::max(max, x1[i]);
    }
    std::ofstream("test_values.txt", std::ios::app) << min << "\n" << max << "\n";
    */
}


template<typename TFloat, typename TResult>
void testResult(TResult result, TFloat *bestX, unsigned int n,
                TFloat fmin, unsigned int maxIterations,
                TFloat (*func)(const TFloat *restrict, unsigned int)) {
    TFloat sqrDiff = 0.f;
    for(unsigned int i=0; i < n; i++) {
        sqrDiff += (bestX[i] - result.x[i]) * (bestX[i] - result.x[i]);
    }
    TEST_OP(std::sqrt(sqrDiff), <, 1e-5f);
    TFloat fx = Ackley(result.x, n);
    TEST_EQ(fx, result.fx);
    TEST_OP(std::sqrt(result.fx), <, 1e-5f);
    TEST_TRUE(fmin >= fx || result.iterations == maxIterations);
}


template<typename TFloat, typename TResult, typename TSimAnn>
void testSimulatedAnnealing(const unsigned int n, TSimAnn simAnn) {
    const unsigned int maxIterations = 10 * n;
    const TFloat fmin = 1e-10;
    TFloat *bestX = new TFloat[n];
    for(unsigned int i=0; i < n; i++) {
        bestX[i] = 0;
    }
    TFloat *data = new TFloat[n];
    TResult result;
    result.x = new TFloat[n];
/*
    // test that simulated annealing aborts for wrong parameters
    TEST_EQ(simAnn(NULL, 0, 0, NULL, 0, 0, AckleyNeighbour, data, &result),
            PNT_INVALID_PARAMETER);
    TEST_EQ(simAnn(NULL, 0, 0, Ackley, 0, 0, NULL, data, &result),
            PNT_INVALID_PARAMETER);
    TEST_EQ(simAnn(NULL, 0, 0, Ackley, 0, 0, AckleyNeighbour, NULL, &result),
            PNT_INVALID_PARAMETER);
    TEST_EQ(simAnn(NULL, 0, 0, Ackley, 0, 0, AckleyNeighbour, data, NULL),
            PNT_INVALID_PARAMETER);
*/
    // test that simulated annealing finds an easy optimum
    TFloat *x = new TFloat[n];
    for(unsigned int i=0; i < n; i++) {
        x[i] = 1;
    }
    TFloat fx = Ackley(x, n);
    /*
    TEST_EQ(simAnn(x, n, fx, Ackley, maxIterations, fmin,
                   AckleyNeighbour, data, &result), PNT_SUCCESS);
    testResult(result, bestX, n, fmin, maxIterations, Ackley);
*/
    // test that simulated annealing finds a more difficult optimum with
    // a neighbour function to help it
    for(unsigned int i=0; i < n; i++) {
        x[i] = 5;
    }
    fx = Ackley(x, n);
    TEST_EQ(simAnn(x, n, fx, Ackley, maxIterations, fmin, AckleyNeighbourWide,
                   data, &result), PNT_SUCCESS);
    testResult(result, bestX, n, fmin, maxIterations, Ackley);

    // cleanup
    delete []bestX;
    delete []x;
    delete []result.x;
    delete []data;
}


void testSimulatedAnnealing() {
    std::cout << "\ntesting simulated annealing (tests may fail due to the "
              << "random nature of the algorithm)\n";
    std::cout << "\n\ttesting float - scalar\n";
    //testSimulatedAnnealing<float, pntfSAResult>(1, pntfSimAnn);
    std::cout << "\n\ttesting float - vector dim 100\n";
    testSimulatedAnnealing<float, pntfSAResult>(100, pntfSimAnn);
}

