#define TEST(expr)      \
{                       \
    if(expr)            \
        printf(".");    \
    else                \
        printf("\n\"" #expr "\" failed, line %i file " \
               __FILE__"\n", __LINE__);  \
}

int ssFailingTestFunction(float, float*);
int dsFailingTestFunction(double, double*);
int ldsFailingTestFunction(long double, long double*);

