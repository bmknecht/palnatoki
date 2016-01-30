#ifndef PALNATOKI_SIMULATED_ANNEALING_H
#define PALNATOKI_SIMULATED_ANNEALING_H


/** This file is part of the Palnatoki optimization library. For licensing
 *  information refer to the LICENSE file that is included in the project.
 *
 *  This file in particular contains the function and structure definitions
 *  for the Simulated Annealing implementation.
 *
 *  Nomenclature:
 *  'cost function': The function to be optimized.
 *  'argument': A vector of parameters for the optimization function (may
 *      be one-dimensional).
 *  'solution': The best (known) argument.
 *  'cost': The cost function value for a given argument.
 */
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Result structure type definitions.
 *  These contain the results of the optimization process.
 *  Note that the variables may contain arbitrary values if pnt*SimAnn
 *  returns an error.
 */
typedef struct _pntfSAResult
{
	unsigned int iterations;    // Number of iterations done
	float *restrict solution;
	float solutionCost;
} pntfSAResult;


/** Parameter structure definitions.
 *  These contain optional and non-optional parameters to the
 *  simulated annealing algorithm function(s).
 */
typedef struct _pntfSAParameters {
    //
    // non-optional parameters
    //

    // Starting argument (size 'dimension').
    float *restrict startingArgument;

    // Dimension/Size of the arguments.
    unsigned int dimension;

    // Cost of the argument 'startingArgument'
    // i.e. cost = costFunction(startingArgument, dimension)
    float cost;

    /** Cost function. This must return the cost of a given argument
     *  'argument' of size 'dimension'.
     */
    float (*costFunction)(const float *restrict argument,
                          unsigned int dimension);

    /** Iterations limit. If this is reached, the optimization process
     *  will be terminated. Note that in this case the functions will still
     *  return PNT_SUCCESS.
     */
    unsigned int itermax;

    /** Minimum cost at which the optimization process is considered a success
     *  and therefore terminated. This happens even if the maximum number of
     *  iterations is not reached.
     */
    float acceptableCost;

    /** A neighbour function that takes the last accepted argument
     *  (simulated annealing may discard arguments provided by this 
     *  function), its dimension, the current temperature, and an argument that
     *  is to be set by this function.  The parameter 'nextArgument' may be
     *  uninitialized the first time this function is called. Furthermore it
     *  must provide a valid argument, i.e. it must be inside the valid
     *  parameter domain (in-bounds).
     *  This structure member is non-optional because a good neighbour function
     *  is problem dependent, therefore there is no implementation provided for
     *  you.
     */
    void (*neighbour)(const float *restrict currentArgument,
                      unsigned int dimension, float currentTemperature,
                      float *restrict nextArgument);

    /** A working array of size 'dimension'. This lets the algorithm run without
     *  any heap/memory allocations.
     */
    void *restrict work;


    //
    // optional parameters
    //

    /** A temperature function that takes the current iteration,
     *  the maximum number of iterations and the current temperature
     *  and returns the next temperature. At the beginning the
     *  'currentTemperature' will be set to 1.
     *  This parameter may be NULL. If it is NULL, a linear temperature
     *  decrease will be used and the temperature will never be reset (so called
     *  reannealing).
     */
    float (*temperature)(unsigned int iteration, unsigned int itermax,
                         float currentTemperature);

    /** Downgrade determines whether an argument, that increases
     *  the current arguments cost, is accepted as the next argument.
     *  It takes the cost of the current argument ('cost'), the argument
     *  candidates cost ('costnew') and the current temperature.
     *  The temperature will contain values between 1 (at the beginning of the
     *  optimization process) and 0 (at the end).
     *  It must return non-0 if the candidate is accepted, else 0.
     *  This parameter may be NULL. If it is, the new argument will be
     *  accepted with a probability of exp(-(costnew - cost) / temperature).
     */
    int (*downgrade)(float cost, float costnew, float temperature);
} pntfSAParameters;


/** Simulated Annealing algorithm functions.
 *  All of the following functions perform a Simulated Annealing optimization
 *  with differing precision.
 *
 *  For a description of the parmeters refer to the description of the
 *  structures above. Neither of the parameters may be NULL. The values of the
 *  variables of the structures may be changed by the function, whether it
 *  returns an error or not. You can therefore not rely on their values'
 *  content.
 *
 *  This function will return PNT_SUCCESS on success or one of the enum values
 *  of PNT_RESULT (see common.h). In particular the function will only return
 *  an error if a parameter is invalid or an unexpected error
 *  occurred. In the later case the values of the pnt*SAResult structure
 *  are undefined.
 */
PNT_LIB int pntfSimAnn(pntfSAParameters *parameters,
                       pntfSAResult *result);


#ifdef __cplusplus
}
#endif

#endif // PALNATOKI_SIMULATED_ANNEALING_H
