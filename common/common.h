#ifndef PALNATOKI_COMMON_H
#define PALNATOKI_COMMON_H

#include <stdbool.h>


typedef int (*pntSsBoundary)(float*);
typedef int (*pntDsBoundary)(double*);
typedef int (*pntLDsBoundary)(long double*);

#define PNT_SUCCESS (0)
#define PNT_INVALID_PARAMETER (1)
#define PNT_EXTERN_ERROR (2)

#endif // PALNATOKI_COMMON_H
