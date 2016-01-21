//
//
//  Simulated Annealing Implementation
//
//
//
//  local functions
//
static int _temperature(int i, int nmax, float *t)
{
    if(!t)
        return PNT_LOGIC_ERROR;
    if(nmax == i)
        return PNT_LOGIC_ERROR;
    *t = 1.f - (float)i / (float)nmax;
    return PNT_SUCCESS;
}
static int _downgrade(float fx, float fx1, float t, int *result)
{
    if(!result)
        return PNT_LOGIC_ERROR;
    float randomValue = (float)rand() / (float)RAND_MAX;
    *result = math_exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}
static int _chooseNextX(float *x, float *fx, float x1, float fx1,
                        float t, int (*downgrade)(float, float, float, int*);)
{
    if(!downgrade || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(fx1 < fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}
static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional, int (*neighbour)(float, float, float*),
                      float *x, float *fx)
{
    if(!neighbour || !optional || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(i >= nmax)
        return PNT_LOGIC_ERROR;
    float t;
    float x1;
    RETHROW(optional->temperature(i, nmax, &t));
    RETHROW(neighbour(x, t, &x1));
    float fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}
int _verifyParameters(Optional *optional)
{
    if(!optional->temperature)
        optional->temperature = _temperature;
    if(!optional->downgrade)
        optional->downgrade = _downgrade;
    return PNT_SUCCESS;
}
//
//  main function
//
int cppsSimAnn(float x, float fx, Objective f, const int nmax, float fmin,
               int (*neighbour)(float, float, float*), Optional *optional, Result *result)
{
    // parameter check
    if(!result || !f || !neighbour || !optional)
        return PNT_INVALID_PARAMETER;
    // setup
    _verifyParameters(optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;
    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, x, fx));
        // new best parameter ?
        if(result->fx > fx)
        {
            result->x = x;
            result->fx = fx;
        }
        // reached goal ?
        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }
    return PNT_SUCCESS;
}
//
//
//  Simulated Annealing Implementation
//
//
//
//  local functions
//
static int _temperature(int i, int nmax, double *t)
{
    if(!t)
        return PNT_LOGIC_ERROR;
    if(nmax == i)
        return PNT_LOGIC_ERROR;
    *t = 1.f - (double)i / (double)nmax;
    return PNT_SUCCESS;
}
static int _downgrade(double fx, double fx1, double t, int *result)
{
    if(!result)
        return PNT_LOGIC_ERROR;
    double randomValue = (double)rand() / (double)RAND_MAX;
    *result = math_exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}
static int _chooseNextX(double *x, double *fx, double x1, double fx1,
                        double t, int (*downgrade)(double, double, double, int*);)
{
    if(!downgrade || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(fx1 < fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}
static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional, int (*neighbour)(double, double, double*),
                      double *x, double *fx)
{
    if(!neighbour || !optional || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(i >= nmax)
        return PNT_LOGIC_ERROR;
    double t;
    double x1;
    RETHROW(optional->temperature(i, nmax, &t));
    RETHROW(neighbour(x, t, &x1));
    double fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}
int _verifyParameters(Optional *optional)
{
    if(!optional->temperature)
        optional->temperature = _temperature;
    if(!optional->downgrade)
        optional->downgrade = _downgrade;
    return PNT_SUCCESS;
}
//
//  main function
//
int cppsSimAnn(double x, double fx, Objective f, const int nmax, double fmin,
               int (*neighbour)(double, double, double*), Optional *optional, Result *result)
{
    // parameter check
    if(!result || !f || !neighbour || !optional)
        return PNT_INVALID_PARAMETER;
    // setup
    _verifyParameters(optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;
    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, x, fx));
        // new best parameter ?
        if(result->fx > fx)
        {
            result->x = x;
            result->fx = fx;
        }
        // reached goal ?
        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }
    return PNT_SUCCESS;
}
//
//
//  Simulated Annealing Implementation
//
//
//
//  local functions
//
static int _temperature(int i, int nmax, long double *t)
{
    if(!t)
        return PNT_LOGIC_ERROR;
    if(nmax == i)
        return PNT_LOGIC_ERROR;
    *t = 1.f - (long double)i / (long double)nmax;
    return PNT_SUCCESS;
}
static int _downgrade(long double fx, long double fx1, long double t, int *result)
{
    if(!result)
        return PNT_LOGIC_ERROR;
    long double randomValue = (long double)rand() / (long double)RAND_MAX;
    *result = math_exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}
static int _chooseNextX(long double *x, long double *fx, long double x1, long double fx1,
                        long double t, int (*downgrade)(long double, long double, long double, int*);)
{
    if(!downgrade || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(fx1 < fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}
static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional, int (*neighbour)(long double, long double, long double*),
                      long double *x, long double *fx)
{
    if(!neighbour || !optional || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(i >= nmax)
        return PNT_LOGIC_ERROR;
    long double t;
    long double x1;
    RETHROW(optional->temperature(i, nmax, &t));
    RETHROW(neighbour(x, t, &x1));
    long double fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}
int _verifyParameters(Optional *optional)
{
    if(!optional->temperature)
        optional->temperature = _temperature;
    if(!optional->downgrade)
        optional->downgrade = _downgrade;
    return PNT_SUCCESS;
}
//
//  main function
//
int cppsSimAnn(long double x, long double fx, Objective f, const int nmax, long double fmin,
               int (*neighbour)(long double, long double, long double*), Optional *optional, Result *result)
{
    // parameter check
    if(!result || !f || !neighbour || !optional)
        return PNT_INVALID_PARAMETER;
    // setup
    _verifyParameters(optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;
    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, x, fx));
        // new best parameter ?
        if(result->fx > fx)
        {
            result->x = x;
            result->fx = fx;
        }
        // reached goal ?
        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }
    return PNT_SUCCESS;
}
//
//
//  Simulated Annealing Implementation
//
//
//
//  local functions
//
static int _temperature(int i, int nmax, float *t)
{
    if(!t)
        return PNT_LOGIC_ERROR;
    if(nmax == i)
        return PNT_LOGIC_ERROR;
    *t = 1.f - (float)i / (float)nmax;
    return PNT_SUCCESS;
}
static int _downgrade(float fx, float fx1, float t, int *result)
{
    if(!result)
        return PNT_LOGIC_ERROR;
    float randomValue = (float)rand() / (float)RAND_MAX;
    *result = math_exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}
static int _chooseNextX(float* *x, float *fx, float* x1, float fx1,
                        float t, int (*downgrade)(float, float, float, int*);)
{
    if(!downgrade || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(fx1 < fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}
static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional, int (*neighbour)(float*, float, float**),
                      float* *x, float *fx)
{
    if(!neighbour || !optional || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(i >= nmax)
        return PNT_LOGIC_ERROR;
    float t;
    float* x1;
    RETHROW(optional->temperature(i, nmax, &t));
    RETHROW(neighbour(x, t, &x1));
    float fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}
int _verifyParameters(Optional *optional)
{
    if(!optional->temperature)
        optional->temperature = _temperature;
    if(!optional->downgrade)
        optional->downgrade = _downgrade;
    return PNT_SUCCESS;
}
//
//  main function
//
int cppsSimAnn(float* x, float fx, Objective f, const int nmax, float fmin,
               int (*neighbour)(float*, float, float**), Optional *optional, Result *result)
{
    // parameter check
    if(!result || !f || !neighbour || !optional)
        return PNT_INVALID_PARAMETER;
    // setup
    _verifyParameters(optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;
    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, x, fx));
        // new best parameter ?
        if(result->fx > fx)
        {
            result->x = x;
            result->fx = fx;
        }
        // reached goal ?
        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }
    return PNT_SUCCESS;
}
//
//
//  Simulated Annealing Implementation
//
//
//
//  local functions
//
static int _temperature(int i, int nmax, double *t)
{
    if(!t)
        return PNT_LOGIC_ERROR;
    if(nmax == i)
        return PNT_LOGIC_ERROR;
    *t = 1.f - (double)i / (double)nmax;
    return PNT_SUCCESS;
}
static int _downgrade(double fx, double fx1, double t, int *result)
{
    if(!result)
        return PNT_LOGIC_ERROR;
    double randomValue = (double)rand() / (double)RAND_MAX;
    *result = math_exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}
static int _chooseNextX(double* *x, double *fx, double* x1, double fx1,
                        double t, int (*downgrade)(double, double, double, int*);)
{
    if(!downgrade || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(fx1 < fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}
static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional, int (*neighbour)(double*, double, double**),
                      double* *x, double *fx)
{
    if(!neighbour || !optional || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(i >= nmax)
        return PNT_LOGIC_ERROR;
    double t;
    double* x1;
    RETHROW(optional->temperature(i, nmax, &t));
    RETHROW(neighbour(x, t, &x1));
    double fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}
int _verifyParameters(Optional *optional)
{
    if(!optional->temperature)
        optional->temperature = _temperature;
    if(!optional->downgrade)
        optional->downgrade = _downgrade;
    return PNT_SUCCESS;
}
//
//  main function
//
int cppsSimAnn(double* x, double fx, Objective f, const int nmax, double fmin,
               int (*neighbour)(double*, double, double**), Optional *optional, Result *result)
{
    // parameter check
    if(!result || !f || !neighbour || !optional)
        return PNT_INVALID_PARAMETER;
    // setup
    _verifyParameters(optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;
    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, x, fx));
        // new best parameter ?
        if(result->fx > fx)
        {
            result->x = x;
            result->fx = fx;
        }
        // reached goal ?
        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }
    return PNT_SUCCESS;
}
//
//
//  Simulated Annealing Implementation
//
//
//
//  local functions
//
static int _temperature(int i, int nmax, long double *t)
{
    if(!t)
        return PNT_LOGIC_ERROR;
    if(nmax == i)
        return PNT_LOGIC_ERROR;
    *t = 1.f - (long double)i / (long double)nmax;
    return PNT_SUCCESS;
}
static int _downgrade(long double fx, long double fx1, long double t, int *result)
{
    if(!result)
        return PNT_LOGIC_ERROR;
    long double randomValue = (long double)rand() / (long double)RAND_MAX;
    *result = math_exp(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}
static int _chooseNextX(long double* *x, long double *fx, long double* x1, long double fx1,
                        long double t, int (*downgrade)(long double, long double, long double, int*);)
{
    if(!downgrade || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(fx1 < fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}
static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional, int (*neighbour)(long double*, long double, long double**),
                      long double* *x, long double *fx)
{
    if(!neighbour || !optional || !x || !fx)
        return PNT_LOGIC_ERROR;
    if(i >= nmax)
        return PNT_LOGIC_ERROR;
    long double t;
    long double* x1;
    RETHROW(optional->temperature(i, nmax, &t));
    RETHROW(neighbour(x, t, &x1));
    long double fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}
int _verifyParameters(Optional *optional)
{
    if(!optional->temperature)
        optional->temperature = _temperature;
    if(!optional->downgrade)
        optional->downgrade = _downgrade;
    return PNT_SUCCESS;
}
//
//  main function
//
int cppsSimAnn(long double* x, long double fx, Objective f, const int nmax, long double fmin,
               int (*neighbour)(long double*, long double, long double**), Optional *optional, Result *result)
{
    // parameter check
    if(!result || !f || !neighbour || !optional)
        return PNT_INVALID_PARAMETER;
    // setup
    _verifyParameters(optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;
    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, x, fx));
        // new best parameter ?
        if(result->fx > fx)
        {
            result->x = x;
            result->fx = fx;
        }
        // reached goal ?
        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }
    return PNT_SUCCESS;
}
