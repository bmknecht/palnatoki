#ifndef PALNATOKI_COMMON_H
#define PALNATOKI_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


typedef int (*pntSsObjective)(float x, float *fx);
typedef int (*pntDsObjective)(double x, double *fx);
typedef int (*pntLDsObjective)(long double x, long double *fx);


enum PNT_RESULT
{
    PNT_SUCCESS = 0,
    PNT_INVALID_PARAMETER,
    PNT_LOGIC_ERROR
};


#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_COMMON_H
