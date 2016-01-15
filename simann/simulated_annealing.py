import math
import random

import numpy

from .. import utility


def simulatedAnnealing(x0, f, nmax,
                       xmin=utility.identity,
                       xmax=utility.identity,
                       fmin=0,
                       temperature=_temperature,
                       newNeighbour=_randomVector,
                       downgrade=_downgrade):
    """ Computes the minimum of an objective function via the standard
    simulated annealing algorithm.

    Args:
        x0: starting parameter(s) for the objective function
        f: the objective function to minimize
        nmax: maximum number of iterations (equals function evaluations + 1)
        xmin, xmax: functions to keep parameter(s) in bounds, alternatively
            vectors of minimum / maximum values
        fmin: desired objective function value
        temperature: callable which chooses the next temperature
            (standard: linear)
        newNeighbour: callable which chooses the next parameter set
            (standard: random vector with length equal to the temperature)
        downgrade: callable which chooses whether to downgrade the current
            solution to a less optimal one
            (standard: inverse exponential to objective value reduction)

    Returns:
        tuple consisting of 3 values: 

    """

    xmin = utility.replaceVectorByFunction(xmin, min)
    xmax = utility.replaceVectorByFunction(xmax, max)
    x = x0
    fx = f(x)
    for i in range(nmax):
        t = temperatureFunc(i, nmax)
        x1 = xmin(xmax(newNeighbourFunc(x, t)))
        fx1 = f(x1)
        x, fx = _chooseNextX(x, fx, x1, fx1, t, _downgrade)
        if fx <= fmin:
            return x, fx, i
    return x, fx, i


def _temperature(i, nmax):
    return 1 - i / nmax


def _randomVector(x, t):
    r = np.array(len(x))
    return x + t / numpy.linalg.norm(r) * r


def _downgrade(fx, fx1, t):
    return math.exp(-(fx1 - fx) / t) > random.random()


def _chooseNextX(x, fx, x1, fx1, t, downgrade):
    if fx1 < fx:
        return x1, fx1
    else:
        if downgrade(fx, fx1, t):
            return x1, fx1
    return x, fx

