#include "common.h"
#include <palnatoki/include/simulated_annealing.h>


#define EXTERN_FUNCTION_ERROR_CODE 101


//
//
//      FLOAT TESTS
//
//
int fTemperature(int i, int nmax, float *t) {
    if(!t) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *t = 1.f - (float)i / (float)nmax;
    return 0;
}


int fAckleyNeighbour(float *x, int n, float t, float *x1) {
    if(!x1) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    for(int i=0; i < n; i++) {
        x1[i] = x[i] + t * 5.f * ((float)rand() / (float)RAND_MAX - 0.5);
    }
    return 0;
}


int fAckleyNeighbourWide(float *x, int n, float t, float *x1) {
    if(!x1) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    for(int i=0; i < n; i++) {
        x1[i] = x[i] + t * 20.f * ((float)rand() / (float)RAND_MAX - 0.5);
    }
    return 0;
}


int fDowngrade(float fx, float fxnew, float temperature, int *result) {
    if(!result) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *result = 0;
    return rand()%2;
}


void ftestResult(pntfSAResult result, float *bestX, int n, float fmin,
                 int maxIterations, int (*func)(float*, int, float*)) {
    float sqrDiff = 0.f;
    for(int i=0; i < n; i++) {
        sqrDiff += (bestX[i] - result.x[i]) * (bestX[i] - result.x[i]);
    }
    TEST_OP(sqrtf(sqrDiff), <, 1e-5f, float, %f);
    float fx;
    assert(fAckley(result.x, n, &fx) == 0);
    TEST_EQ(fx, result.fx, float, %f);
    TEST_TRUE(fmin >= fx || result.iterations == maxIterations);
}


void testfSimulatedAnnealing(const int n) {
    const int maxIterations = 5000;
    const float fmin = 1e-10f;
    float *bestX = malloc(n * sizeof(float));
    assert(bestX);
    float fx;
    pntfSAResult result;

    // test that simulated annealing aborts for wrong parameters
    TEST_EQ(pntfSimAnn(NULL, 0, 0, NULL, 0, 0, fAckleyNeighbour, NULL,
                       &result), PNT_INVALID_PARAMETER, int, %i);
    TEST_EQ(pntfSimAnn(NULL, 0, 0, fAckley, 0, 0, NULL, NULL,
                       &result), PNT_INVALID_PARAMETER, int, %i);
    TEST_EQ(pntfSimAnn(NULL, 0, 0, fAckley, 0, 0, fAckleyNeighbour, NULL,
                       NULL), PNT_INVALID_PARAMETER, int, %i);

    // test that simulated annealing finds an easy optimum
    float *x = malloc(n * sizeof(float));
    assert(x);
    assert(fAckley(x, n, &fx) == 0);
    TEST_EQ(pntfSimAnn(x, n, fx, fAckley, maxIterations, fmin,
                       fAckleyNeighbour, NULL, &result),
            PNT_SUCCESS, int, %i);
    ftestResult(result, bestX, n, fmin, maxIterations, fAckley);

    // test that simulated annealing finds a more difficult optimum with
    // a neighbour function to help it
    fx = 5.f;
    assert(fAckley(x, n, &fx) == 0);
    TEST_EQ(pntfSimAnn(x, n, fx, fAckley, maxIterations, fmin,
                       fAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    ftestResult(result, bestX, n, fmin, maxIterations, fAckley);

    // test that simulated annealing can use a custom temperature function
    // (may crash)
    assert(fAckley(x, n, &fx) == 0);
    pntfSAOptional optional;
    memset(&optional, 0, sizeof(optional));
    optional.temperature = &fTemperature;
    TEST_EQ(pntfSimAnn(x, n, fx, fAckley, maxIterations, fmin,
                       fAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    ftestResult(result, bestX, n, fmin, maxIterations, fAckley);

    // test that simulated annealing can use a custom downgrade function
    // (may crash)
    fx = 5.f;
    assert(fAckley(x, n, &fx) == 0);
    optional.temperature = NULL;
    optional.downgrade = &fDowngrade;
    TEST_EQ(pntfSimAnn(x, n, fx, fAckley, maxIterations, fmin,
                       fAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    ftestResult(result, bestX, n, fmin, maxIterations, fAckley);

    free(bestX);
    free(x);
}


void testSimulatedAnnealing() {
    printf("\ntesting simulated annealing (tests may fail due to the random "
           "nature of the algorithm)\n");
    printf("\n\ttesting float - scalar\n");
    testfSimulatedAnnealing(100);
    printf("\n\ttesting double - scalar\n");
    //testdSimulatedAnnealing();
    printf("\n\ttesting long double - scalar\n");
    //testldSimulatedAnnealing();
}

