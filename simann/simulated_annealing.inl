static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional, Neighbour neighbour, FLOAT *x,
                      FLOAT *fx);
static int _chooseNextX(FLOAT *x, FLOAT *fx, FLOAT x1, FLOAT fx1, FLOAT t,
                        Downgrade downgrade);
static int _nextNeighbourInBounds(FLOAT x, FLOAT *x1, FLOAT t,
                                  const Optional *optional,
                                  Neighbour neighbour);
static void _setDefaultParameters(Optional *optional);
static int _temperature(int i, int nmax, FLOAT *t);
static int _downgrade(FLOAT fx, FLOAT fx1, FLOAT t, int *result);


int FUNC_NAME(FLOAT x, FLOAT fx, Objective f, const int nmax, FLOAT fmin,
              Neighbour neighbour, Optional *optional, Result *result)
{
    // parameter check
    if(!result)
        return PNT_INVALID_PARAMETER;
    if(!f)
        return PNT_INVALID_PARAMETER;
    if(!neighbour)
        return PNT_INVALID_PARAMETER;

    // setup
    _setDefaultParameters(optional);
    result->x = x;
    result->fx = fx;
    result->iterations = nmax;

    // iteration
    for(int i = 0; i < nmax; i++)
    {
        RETHROW(_iteration(f, i, nmax, optional, neighbour, &x, &fx));
        if(result->fx > fx)
        {
            result->x = x;
            result->fx = fx;
        }

        if(fx <= fmin)
        {
            result->iterations = i;
            break;
        }
    }

    // save result
    return PNT_SUCCESS;
}


static int _iteration(Objective f, int i, const int nmax,
                      const Optional *optional, Neighbour neighbour, FLOAT *x,
                      FLOAT *fx)
{
    assert(optional);
    assert(neighbour);
    assert(x);
    assert(fx);

    FLOAT t, x1;
    RETHROW(optional->temp(i, nmax, &t));
    RETHROW(_nextNeighbourInBounds(*x, &x1, t, optional, neighbour));

    FLOAT fx1;
    RETHROW(f(x1, &fx1));
    RETHROW(_chooseNextX(x, fx, x1, fx1, t, optional->downgrade));
    return PNT_SUCCESS;
}


static int _nextNeighbourInBounds(FLOAT x, FLOAT *x1, FLOAT t,
                                  const Optional *optional,
                                  Neighbour neighbour)
{
    assert(x1);
    RETHROW(neighbour(x, t, x1));
    if(optional->xmin)
        RETHROW(optional->xmin(x1));
    if(optional->xmax)
        RETHROW(optional->xmax(x1));
    return PNT_SUCCESS;
}


static void _setDefaultParameters(Optional *optional)
{
    assert(optional);
    if(!optional->temp)
        optional->temp = _temperature;
    if(!optional->downgrade)
        optional->downgrade = _downgrade;
}


static int _temperature(int i, int nmax, FLOAT *t)
{
    assert(t);
    *t = 1.f - (FLOAT)i / (FLOAT)nmax;
    return PNT_SUCCESS;
}


static int _downgrade(FLOAT fx, FLOAT fx1, FLOAT t, int *result)
{
    assert(result);
    FLOAT randomValue = (FLOAT)rand() / (FLOAT)RAND_MAX;
    *result = expf(-(fx1 - fx) / t) > randomValue;
    return PNT_SUCCESS;
}


static int _chooseNextX(FLOAT *x, FLOAT *fx, FLOAT x1, FLOAT fx1, FLOAT t,
                        Downgrade downgrade)
{
    assert(x);
    assert(fx);
    assert(downgrade);
    if(fx1 < *fx)
    {
        *x = x1;
        *fx = fx1;
    }
    else
    {
        int result;
        RETHROW(downgrade(*fx, fx1, t, &result));
        if(result)
        {
            *x = x1;
            *fx = fx1;
        }
    }
    return PNT_SUCCESS;
}

