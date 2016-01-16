#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../internal/utility.h"
#include "simulated_annealing.h"


#define FLOAT float
typedef pntSsSAOptional Optional;
typedef pntSsSANeighbour Neighbour;
typedef pntSsSADowngrade Downgrade;
typedef pntSsSAResult Result;
typedef pntSsSAObjective Objective;
#define FUNC_NAME pntSsSimAnn
#include "simulated_annealing.inl"


void pntSsSAOptionalReset(pntSsSAOptional *optional)
{
    optional->xmin = NULL;
    optional->xmax = NULL;
    optional->temp = NULL;
    optional->neighbour = NULL;
    optional->downgrade = NULL;
}

