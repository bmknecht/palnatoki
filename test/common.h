#define _hypot hypot

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <palnatoki/include/objective.h>


#define TEST_TRUE(expr)             \
{                                   \
    if(!(expr)) {                   \
        printf("\n" #expr " failed: " __FILE__ ": "     \
               "%i.\n", __LINE__);  \
    }                               \
    else {                          \
        printf(".");                \
    }                               \
}

#define TEST_EQ(a, b, t, f)         \
{                                   \
    t a_ = a;                       \
    t b_ = b;                       \
    if(!(a_ == b_)) {               \
        printf("\n" #a " == " #b " failed, "           \
               "got \"" #f "\" and \"" #f "\": "       \
               __FILE__ ": %i\n", a_, b_, __LINE__);   \
    }                               \
    else {                          \
        printf(".");                \
    }                               \
}

#define TEST_OP(a, o, b, t, f)      \
{                                   \
    t a_ = a;                       \
    t b_ = b;                       \
    if(!(a_ o b_)) {                \
        printf("\n" #a #o #b " failed, "               \
               "got \"" #f "\" and \"" #f "\": "       \
               __FILE__ ": %i\n", a_, b_, __LINE__);   \
    }                               \
    else {                          \
        printf(".");                \
    }                               \
}

void testSimulatedAnnealing();

