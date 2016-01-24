#include "../common/common.h"


int pntSsCentralDerivative2(float x, float fx, pntSsObjective f, float h,
                            float *fp);
int pntSsCentralDerivative1(float x, float fx, pntSsObjective f, float h,
                            float *fp);
int pntSsForwardDerivative1(float x, float fx, pntSsObjective f, float h,
                            float *fp);
int pntSsBackwardDerivative1(float x, float fx, pntSsObjective f, float h,
                             float *fp);

