#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../internal/utility.h"
#include "simulated_annealing.h"


#define FLOAT long double
typedef pntLDsSAOptional Optional;
typedef pntLDsSANeighbour Neighbour;
typedef pntLDsSADowngrade Downgrade;
typedef pntLDsSAResult Result;
typedef pntLDsSAObjective Objective;
#define FUNC_NAME pntLDsSimAnn
#include "simulated_annealing.inl"

