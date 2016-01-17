#ifndef PALNATOKI_TEST_COMMON_HPP
#define PALNATOKI_TEST_COMMON_HPP


#define TEST(expr)      \
{                       \
    if(expr)            \
        std::cout << ".";    \
    else                \
        std::cout << "\n\"" #expr "\" failed, line " << __LINE__ \
                  << " file " __FILE__"\n";  \
}

int ssFailingTestFunction(float, float*);
int dsFailingTestFunction(double, double*);
int ldsFailingTestFunction(long double, long double*);


#endif // PALNATOKI_TEST_COMMON_HPP
