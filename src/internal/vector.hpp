#ifndef PALNATOKI_INTERNAL_VECTOR_HPP
#define PALNATOKI_INTERNAL_VECTOR_HPP

#include <cstring>
#include <utility>


template<typename F>
class Vector
{
public:
    Vector() {
        array = nullptr;
        n = 0;
    }

    Vector(const Vector &rhs) {
        array = nullptr;
        n = 0;
        copyFrom(rhs);
    }

    Vector(Vector &&rhs) {
        array = nullptr;
        n = 0;
        moveFrom(rhs);
    }

    ~Vector() {
        destroy();
    }

    Vector& operator=(const Vector &rhs) {
        return copyFrom(rhs);
    }

    Vector& operator=(Vector &&rhs) {
        return moveFrom(std::move(rhs));
    }

    inline F* pointer() {
        return array;
    }

    inline const F* pointer() const {
        return array;
    }

    inline size_t getSize() const {
        return n;
    }

    void resize(size_t newSize) {
        if(newSize == n) {
            return;
        }

        F *oldPtr = array;
        array = new F[newSize];
        std::memcpy(array, oldPtr, n * sizeof(F));
        delete []oldPtr;
        n = newSize;
    }

    Vector& borrowFrom(const Vector &rhs) {
        moveFrom(rhs);
        ownsArray = false;
        return *this;
    }

    Vector& borrowFrom(F *array, size_t n) {
        moveFrom(array, n);
        ownsArray = false;
        return *this;
    }

    Vector& copyFrom(const Vector &rhs) {
        destroy();

        if(rhs.n != 0) {
            array = new F[rhs.n];
            memcpy(array, rhs.array, n * sizeof(F));
        }
        else {
            array = nullptr;
        }
        n = rhs.n;
        ownsArray = true;
        return *this;
    }

    Vector& moveFrom(Vector &&rhs) {
        std::swap(array, rhs.array);
        std::swap(n, rhs.n);
        std::swap(ownsArray, rhs.ownsArray);
        return *this;
    }

    Vector& moveFrom(F *array, size_t n) {
        destroy();
        this->array = array;
        this->n = n;
        ownsArray = true;
        return *this;
    }

    void destroy() {
        if(ownsArray) {
            delete []array;
        }
        array = nullptr;
        n = 0;
    }

    inline F& operator[](size_t i) {
        assert(i < n);
        return array[i];
    }

    inline const F& operator[](size_t i) const {
        assert(i < n);
        return array[i];
    }


private:
    F *array;
    size_t n;
    bool ownsArray;
};


template<typename T>
inline T sqrSum(const Vector<T> &v) {
    auto n = size(v);
    T s = 0;
    for(auto i=0; i < n; i++) {
        s += v[i]*v[i];
    }
    return s;
}


template<typename T>
inline size_t size(const Vector<T> &v) {
    return v.getSize();
}


template<typename T>
inline void copy(Vector<T> &dest, const Vector<T> &src) {
    dest.copyFrom(src);
}


#endif // PALNATOKI_INTERNAL_VECTOR_HPP
