#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../internal/utility.h"
#include "simulated_annealing.h"


#define FLOAT double
typedef pntDsSAOptional Optional;
typedef pntDsSANeighbour Neighbour;
typedef pntDsSADowngrade Downgrade;
typedef pntDsSAResult Result;
typedef pntDsSAObjective Objective;
#define FUNC_NAME pntDsSimAnn
#include "simulated_annealing.inl"

