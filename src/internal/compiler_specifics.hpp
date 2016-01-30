#ifndef PALNATOKI_COMPILER_SPECIFICS_HPP
#define PALNATOKI_COMPILER_SPECIFICS_HPP

/** This file is part of the Palnatoki optimization library. For licensing
 *  information refer to the LICENSE file that is included in the project.
 *
 *  This file in particular contains compiler specific defines.
 */

// On my system MINGW fails to define _hypot but needs it in a std-header.
#ifdef __MINGW32__
#define _hypot hypot
#endif

#endif // PALNATOKI_COMPILER_SPECIFICS_HPP
