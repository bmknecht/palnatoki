#ifndef PALNATOKI_COMMON_H
#define PALNATOKI_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef WIN32
    #ifdef PNT_EXPORT_LIBRARY
        #define PNT_LIB __declspec(dllexport)
    #elif defined PNT_IMPORT_LIBRARY
        #define PNT_LIB __declspec(dllimport)
    #endif
#endif

#ifndef PNT_LIB
    #define PNT_LIB
#endif


enum PNT_RESULT
{
    PNT_SUCCESS = 0,
    PNT_INVALID_PARAMETER,
    PNT_LOGIC_ERROR,
    PNT_STD_EXCEPTION,
    PNT_UNKNOWN_ERROR   // Please file a bug report.
};


#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_COMMON_H
