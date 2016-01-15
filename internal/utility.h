#include "../common/common.h"


#define RETHROW(expr)       \
{                           \
    int res = expr;         \
    if(res != PNT_SUCCESS)  \
        return res;         \
}

