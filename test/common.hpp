#define _hypot hypot

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>


#include <palnatoki/include/objective.h>


#define TEST_TRUE(expr)             \
{                                   \
    if(!(expr)) {                   \
        std::cout << "\n" #expr     \
                  << " failed: "    \
                  << __FILE__ ": "  \
                  << __LINE__       \
                  << ".\n";         \
    }                               \
    else {                          \
        std::cout << ".";           \
    }                               \
}


#define TEST_OP(a, o, b)            \
{                                   \
    auto a_ = a;                    \
    auto b_ = b;                    \
    if(!(a_ o b_)) {                \
        std::cout << "\n" #a #o #b " failed, "               \
               "got \"" << a_ << "\" and \"" << b_ << "\": " \
               __FILE__ ": " << __LINE__ << "\n";            \
    }                               \
    else {                          \
        std::cout << ".";           \
    }                               \
}


#define TEST_EQ(a, b) TEST_OP(a, ==, b)

void testSimulatedAnnealing();

