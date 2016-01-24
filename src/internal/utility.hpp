
#include <palnatoki/include/common.h>
#include "compiler_specifics.hpp"
#include "math.hpp"

#define RETHROW(expr)       \
{                           \
    int res = expr;         \
    if(res != PNT_SUCCESS)  \
        return res;         \
}

#define BEGIN_GUARD_CPP_CODE try
#define END_GUARD_CPP_CODE              \
    catch(std::exception &e) {          \
        return PNT_STD_EXCEPTION;       \
    } catch(...) {                      \
        return PNT_UNKNOWN_ERROR;       \
    }

template<typename T>
T* asPointer(T &t) { return &t; }

template<typename T>
T* asPointer(Vector<T> *t) { return t->asPointer(); }

