import numpy as np


def identity(x):
    return x


def replaceVectorByFunction(v, boundary):
    if hasattr(v, '__call__'):
        return v

    def f(x):
        return numpy.array([boundary(v[i], x[i]) for i in range(len(v))])
    return f

