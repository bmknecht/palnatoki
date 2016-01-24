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


int fsAckleyNeighbour(float *x, int n, float t, float *x1) {
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
                 int maxIterations, int (*func)(float, float*)) {
    float sqrDiff = 0.f;
    for(int i=0; i < n; i++) {
        sqrMetric += (bestX[i] - result.x[i]) * (bestX[i] - result.x[i]);
    }
    TEST_OP(sqrtf(sqrDiff), <, 1e-5f, float, %f);
    float ffx;
    assert(fAckley(result.x, &ffx) == 0);
    TEST_EQ(ffx, result.fx, float, %f);
    TEST_TRUE(fmin >= ffx || result.iterations == maxIterations);
}


void testfSimulatedAnnealing(const int n) {
    const int maxIterations = 5000;
    const float fmin = 1e-10f;
    float *bestX = malloc(n * sizeof(float));
    assert(bestX);
    float ffx;
    pntfSAResult result;

    // test that simulated annealing aborts for wrong parameters
    TEST_EQ(pntfSimAnn(0, 0, NULL, 0, 0, fAckleyNeighbour, NULL,
                       &result), PNT_INVALID_PARAMETER, int, %i);
    TEST_EQ(pntfSimAnn(0, 0, fAckley, 0, 0, NULL, NULL,
                       &result), PNT_INVALID_PARAMETER, int, %i);
    TEST_EQ(pntfSimAnn(0, 0, fAckley, 0, 0, fsAckleyNeighbour, NULL,
                        NULL), PNT_INVALID_PARAMETER, int, %i);

    // test that simulated annealing finds an easy optimum
    float fsx = 1.f;
    assert(fsAckley(fsx, &fsfx) == 0);
    TEST_EQ(pntfSimAnn(fsx, fsfx, fsAckley, maxIterations, fmin,
                        fsAckleyNeighbour, NULL, &result), PNT_SUCCESS,
            int, %i);
    fstestResult(result, bestX, fmin, maxIterations, fsAckley);

    // test that simulated annealing finds a more difficult optimum with
    // a neighbour function to help it
    fsx = 5.f;
    assert(fsAckley(fsx, &fsfx) == 0);
    TEST_EQ(pntfSimAnn(fsx, fsfx, fsAckley, maxIterations, fmin,
                        fsAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    fstestResult(result, bestX, fmin, maxIterations, fsAckley);

    // test that simulated annealing can use a custom temperature function
    // (may crash)
    fsx = 5.f;
    assert(fsAckley(fsx, &fsfx) == 0);
    pntfSAOptional optional;
    memset(&optional, 0, sizeof(optional));
    optional.temperature = &fsTemperature;
    TEST_EQ(pntfSimAnn(fsx, fsfx, fsAckley, maxIterations, fmin,
                        fsAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    fstestResult(result, bestX, fmin, maxIterations, fsAckley);

    // test that simulated annealing can use a custom downgrade function
    // (may crash)
    fsx = 5.f;
    assert(fsAckley(fsx, &fsfx) == 0);
    optional.temperature = NULL;
    optional.downgrade = &fsDowngrade;
    TEST_EQ(pntfSimAnn(fsx, fsfx, fsAckley, maxIterations, fmin,
                        fsAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    fstestResult(result, bestX, fmin, maxIterations, fsAckley);
}

//
//
//      DOUBLE TESTS
//
//
int dsTemperature(int i, int nmax, double *t) {
    if(!t) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *t = 1.f - (double)i / (double)nmax;
    return 0;
}


int dsAckleyNeighbour(double x, double t, double *x1) {
    if(!x1) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *x1 = x + t * 5.f * ((double)rand() / (double)RAND_MAX - 0.5);
    return 0;
}


int dsAckleyNeighbourWide(double x, double t, double *x1) {
    if(!x1) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *x1 = x + t * 20.f * ((double)rand() / (double)RAND_MAX - 0.5);
    return 0;
}


int dsDowngrade(double fx, double fxnew, double temperature, int *result) {
    if(!result) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *result = 0;
    return rand()%2;
}


void dstestResult(pntdSAResult result, double bestX, double fmin,
                  int maxIterations, int (*func)(double, double*)) {
    TEST_OP(fabsf(result.x - bestX), <, 1e-5, double, %f);
    double dsfx;
    assert(dsAckley(result.x, &dsfx) == 0);
    TEST_EQ(dsfx, result.fx, double, %f);
    TEST_TRUE(fmin >= dsfx || result.iterations == maxIterations);
}


void testdSimulatedAnnealing() {
    const int maxIterations = 5000;
    const double fmin = 1e-10;
    const double bestX = 0;
    double dsfx;
    pntdSAResult result;

    // test that simulated annealing aborts for wrong parameters
    TEST_EQ(pntdSimAnn(0, 0, NULL, 0, 0, dsAckleyNeighbour, NULL,
                        &result), PNT_INVALID_PARAMETER, int, %i);
    TEST_EQ(pntdSimAnn(0, 0, dsAckley, 0, 0, NULL, NULL,
                        &result), PNT_INVALID_PARAMETER, int, %i);
    TEST_EQ(pntdSimAnn(0, 0, dsAckley, 0, 0, dsAckleyNeighbour, NULL,
                        NULL), PNT_INVALID_PARAMETER, int, %i);

    // test that simulated annealing finds an easy optimum
    double dsx = 1.f;
    assert(dsAckley(dsx, &dsfx) == 0);
    TEST_EQ(pntdSimAnn(dsx, dsfx, dsAckley, maxIterations, fmin,
                        dsAckleyNeighbour, NULL, &result), PNT_SUCCESS,
            int, %i);
    dstestResult(result, bestX, fmin, maxIterations, dsAckley);

    // test that simulated annealing finds a more difficult optimum with
    // a neighbour function to help it
    dsx = 5.f;
    assert(dsAckley(dsx, &dsfx) == 0);
    TEST_EQ(pntdSimAnn(dsx, dsfx, dsAckley, maxIterations, fmin,
                        dsAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    dstestResult(result, bestX, fmin, maxIterations, dsAckley);

    // test that simulated annealing can use a custom temperature function
    // (may crash)
    dsx = 5.f;
    assert(dsAckley(dsx, &dsfx) == 0);
    pntdSAOptional optional;
    memset(&optional, 0, sizeof(optional));
    optional.temperature = &dsTemperature;
    TEST_EQ(pntdSimAnn(dsx, dsfx, dsAckley, maxIterations, fmin,
                        dsAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    dstestResult(result, bestX, fmin, maxIterations, dsAckley);

    // test that simulated annealing can use a custom downgrade function
    // (may crash)
    dsx = 5.f;
    assert(dsAckley(dsx, &dsfx) == 0);
    optional.temperature = NULL;
    optional.downgrade = &dsDowngrade;
    TEST_EQ(pntdSimAnn(dsx, dsfx, dsAckley, maxIterations, fmin,
                        dsAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    dstestResult(result, bestX, fmin, maxIterations, dsAckley);
}


//
//      LONG DOUBLE
//

int ldTemperature(int i, int nmax, long double *t) {
    if(!t) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *t = 1.f - (long double)i / (long double)nmax;
    return 0;
}


int ldAckleyNeighbour(long double x, long double t, long double *x1) {
    if(!x1) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *x1 = x + t * 5.f * ((long double)rand() / (long double)RAND_MAX - 0.5);
    return 0;
}


int ldAckleyNeighbourWide(long double x, long double t, long double *x1) {
    if(!x1) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *x1 = x + t * 20.f * ((long double)rand() / (long double)RAND_MAX - 0.5);
    return 0;
}


int ldDowngrade(long double fx, long double fxnew, long double temperature, int *result) {
    if(!result) {
        return EXTERN_FUNCTION_ERROR_CODE;
    }

    *result = 0;
    return rand()%2;
}


void ldtestResult(pntldSAResult result, long double bestX, long double fmin,
                  int maxIterations, int (*func)(long double, long double*)) {
    TEST_OP(fabsf(result.x - bestX), <, 1e-2f, long double, %f);
    long double ldfx;
    assert(ldAckley(result.x, &ldfx) == 0);
    TEST_EQ(ldfx, result.fx, long double, %f);
    TEST_TRUE(fmin >= ldfx || result.iterations == maxIterations);
}


void testldSimulatedAnnealing() {
    const int maxIterations = 5000;
    const long double fmin = 1e-10f;
    const long double bestX = 0;
    long double ldfx;
    pntldSAResult result;

    // test that simulated annealing aborts for wrong parameters
    TEST_EQ(pntldSimAnn(0, 0, NULL, 0, 0, ldAckleyNeighbour, NULL,
                        &result), PNT_INVALID_PARAMETER, int, %i);
    TEST_EQ(pntldSimAnn(0, 0, ldAckley, 0, 0, NULL, NULL,
                        &result), PNT_INVALID_PARAMETER, int, %i);
    TEST_EQ(pntldSimAnn(0, 0, ldAckley, 0, 0, ldAckleyNeighbour, NULL,
                        NULL), PNT_INVALID_PARAMETER, int, %i);

    // test that simulated annealing finds an easy optimum
    long double ldx = 1.f;
    assert(ldAckley(ldx, &ldfx) == 0);
    TEST_EQ(pntldSimAnn(ldx, ldfx, ldAckley, maxIterations, fmin,
                        ldAckleyNeighbour, NULL, &result), PNT_SUCCESS,
            int, %i);
    ldtestResult(result, bestX, fmin, maxIterations, ldAckley);

    // test that simulated annealing finds a more difficult optimum with
    // a neighbour function to help it
    ldx = 5.f;
    assert(ldAckley(ldx, &ldfx) == 0);
    TEST_EQ(pntldSimAnn(ldx, ldfx, ldAckley, maxIterations, fmin,
                        ldAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    ldtestResult(result, bestX, fmin, maxIterations, ldAckley);

    // test that simulated annealing can use a custom temperature function
    // (may crash)
    ldx = 5.f;
    assert(ldAckley(ldx, &ldfx) == 0);
    pntldSAOptional optional;
    memset(&optional, 0, sizeof(optional));
    optional.temperature = &ldTemperature;
    TEST_EQ(pntldSimAnn(ldx, ldfx, ldAckley, maxIterations, fmin,
                        ldAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    ldtestResult(result, bestX, fmin, maxIterations, ldAckley);

    // test that simulated annealing can use a custom downgrade function
    // (may crash)
    ldx = 5.f;
    assert(ldAckley(ldx, &ldfx) == 0);
    optional.temperature = NULL;
    optional.downgrade = &ldDowngrade;
    TEST_EQ(pntldSimAnn(ldx, ldfx, ldAckley, maxIterations, fmin,
                        ldAckleyNeighbourWide, NULL, &result), PNT_SUCCESS,
            int, %i);
    ldtestResult(result, bestX, fmin, maxIterations, ldAckley);
}


void testSimulatedAnnealing() {
    printf("\ntesting simulated annealing (tests may fail due to the random "
           "nature of the algorithm)\n");
    printf("\n\ttesting float - scalar\n");
    testfSimulatedAnnealing();
    printf("\n\ttesting double - scalar\n");
    testdSimulatedAnnealing();
    printf("\n\ttesting long double - scalar\n");
    testldSimulatedAnnealing();
}

