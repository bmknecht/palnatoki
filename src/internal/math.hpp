#ifndef PALNATOKI_INTERNAL_MATH_HPP
#define PALNATOKI_INTERNAL_MATH_HPP


/** This file is part of the Palnatoki optimization library. For licensing
 *  information refer to the LICENSE file that is included in the project.
 *
 *  This file in particular contains math functions like vector products and
 *  similar. Some of the functions will be wrappers for LAPACK / BLAS routines.
 */

#include <algorithm>
#include <cstring>
#include <utility>


namespace vec {
    // Returns the sum of the squares of the elements of the given vector.
    template<typename T>
    inline T sqrSum(const T *v, size_t n) {
        auto addSqr = [](T &accumulator, T element) {
            return accumulator + element*element;
        };
        return std::accumulate(v, v + n, T(0), addSqr);
    }


    template<typename T>
    inline void copy(T *restrict lhs, const T *restrict rhs, size_t n) {
        std::copy(rhs, rhs + n, lhs);
    }
}

#endif // PALNATOKI_INTERNAL_MATH_HPP
