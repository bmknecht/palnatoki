
#include <palnatoki/include/common.h>
#include "compiler_specifics.hpp"
#include "math.hpp"

#define RETHROW(expr)       \
{                           \
    int res = expr;         \
    if(res != PNT_SUCCESS)  \
        throw res;          \
}

#define BEGIN_GUARD_CPP_CODE try
#define END_GUARD_CPP_CODE              \
    catch(int i) {                      \
        return i;                       \
    } catch(std::exception &e) {        \
        return PNT_STD_EXCEPTION;       \
    } catch(...) {                      \
        return PNT_UNKNOWN_ERROR;       \
    }

