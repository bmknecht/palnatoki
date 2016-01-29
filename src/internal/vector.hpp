#ifndef PALNATOKI_INTERNAL_VECTOR_HPP
#define PALNATOKI_INTERNAL_VECTOR_HPP

#include <algorithm>
#include <cstring>
#include <utility>


namespace vec {
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


#endif // PALNATOKI_INTERNAL_VECTOR_HPP
