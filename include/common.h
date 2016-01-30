#ifndef PALNATOKI_COMMON_H
#define PALNATOKI_COMMON_H


/** This file is part of the Palnatoki optimization library. For licensing
 *  information refer to the LICENSE file that is included in the project.
 *
 *  This file in particular defines some necessary values for the Palnatoki
 *  API, also the return/error values of most of the functions.
 */

#ifdef __cplusplus
extern "C" {

// C -> C++ restrict keyword
#ifndef restrict
#define restrict __restrict
#endif

#endif


// Windows DLL export defines
#ifdef WIN32
    #ifdef PNT_EXPORT_LIBRARY
        #define PNT_LIB __declspec(dllexport)
    #elif defined PNT_IMPORT_LIBRARY
        #define PNT_LIB __declspec(dllimport)
    #else
        #define PNT_LIB
    #endif
#else
#define PNT_LIB
#endif


#ifndef PNT_LIB
    #define PNT_LIB
#endif


/** Error values returned by all functions prefixed with 'pnt'. All other
 *  functions document their behaviour individually (usually its 0 on success,
 *  else error).
 */
enum PNT_RESULT
{
    PNT_SUCCESS = 0,
    PNT_INVALID_PARAMETER,
    // The following error should obviously never be returned
    PNT_STD_EXCEPTION
};


#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_COMMON_H
