#include <stdlib.h>

static unsigned long next = 1;

/* *
 * rand - returns a pseudo-random integer
 *
 * The rand() function return a value in the range [0, RAND_MAX].
 * */
int
rand(void) {
    next = (next * 0xDEECE66D + 0xB) & ((1 << 17) - 1);
    unsigned long long result = (next >> 12);
    return result & RAND_MAX;
}

/* *
 * srand - seed the random number generator with the given number
 * @seed:   the required seed number
 * */
void
srand(unsigned int seed) {
    next = seed;
}

