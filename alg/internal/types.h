#ifdef SCALAR

#ifdef FLOAT
#define scalar float
#elif defined DOUBLE
#define scalar double
#elif defined LONG_DOUBLE
#define scalar long double
#else
#error "Missing type definition."
#endif
#define param scalar

#elif defined VECTOR

#ifdef FLOAT
#define scalar float
#elif defined DOUBLE
#define scalar double
#elif defined LONG_DOUBLE
#define scalar long double
#else
#error "Missing type definition."
#endif
#define param scalar*

#else
#error "Missing parameter dimension definition."
#endif
