#include <assert.h>
#include <math.h>

#include "objective.h"

static const float pif = 3.1415926535898f;


int ssAuckley(float x, float *fx)
{
    assert(fx);
    const float a = 20.f;
    const float b = 0.2f;
    const float c = 2*pif;
    *fx = -a*expf(-b*fabsf(x)) - expf(cosf(c*x)) + a + expf(1.f);
    return 0;
}

int dsAuckley(double x, double *fx);
int ldsAuckley(long double x, long double *fx);

