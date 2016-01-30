#ifndef PALNATOKI_UTILITY_HPP
#define PALNATOKI_UTILITY_HPP

/** This file is part of the Palnatoki optimization library. For licensing
 *  information refer to the LICENSE file that is included in the project.
 *
 *  This file in particular contains utility/various functions and definitions.
 */

#include <palnatoki/include/common.h>
#include "compiler_specifics.hpp"
#include "math.hpp"


#define BEGIN_GUARD_CPP_CODE try
#define END_GUARD_CPP_CODE              \
    catch(int i) {                      \
        return i;                       \
    } catch(std::exception &e) {        \
        return PNT_STD_EXCEPTION;       \
    } catch(...) {                      \
        return PNT_UNKNOWN_ERROR;       \
    }

#endif // PALNATOKI_UTILITY_HPP
