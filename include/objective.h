#ifndef PALNATOKI_OBJECTIVE_H
#define PALNATOKI_OBJECTIVE_H

/** This file is part of the Palnatoki optimization library. For licensing
 *  information refer to the LICENSE file that is included in the project.
 *
 *  This file in particular contains various objective functions that are
 *  internally used for testing purposes and therefore not prefixed with 'pnt'.
 *  You may, however, use them too, just note that these functions
 *  are not rigorously tested.
 */

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Ackleys function.
 *  Has one global minimum at the 0-vector, and a lot of local minima
 *  at various points. See also: http://www.sfu.ca/~ssurjano/ackley.html.
 *  @param x Argument of size n.
 *  @param n Dimension/Size of x.
 *  @return Returns function value at x.
 */
PNT_LIB float fAckley(const float *restrict x, unsigned int n);
PNT_LIB double dAckley(const double *restrict x, unsigned int n);
PNT_LIB long double ldAckley(const long double *restrict x, unsigned int n);

#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_OBJECTIVE_H
