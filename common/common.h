#ifndef PALNATOKI_COMMON_H
#define PALNATOKI_COMMON_H

#include <stdbool.h>


typedef int (*pntSsBoundary)(float*);
typedef int (*pntDsBoundary)(double*);
typedef int (*pntLDsBoundary)(long double*);


typedef int (*pntSsObjective)(float x, float *fx);
typedef int (*pntDsObjective)(double x, double *fx);
typedef int (*pntLDsObjective)(long double x, long double *fx);


#define PNT_SUCCESS (0)
#define PNT_INVALID_PARAMETER (1)


#endif // PALNATOKI_COMMON_H
