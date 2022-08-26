//
// Created by santi on 26/08/2022.
//
#include <ctime>
#include "random.h"


/**
     * Constructor
     * @param seed  see value
     */
Random::Random(long seed) {
    _seed = seed;
}

/**
 * Random returns a pseudo-random real number uniformly distributed
 * between 0.0 and 1.0.
 * @return
 */
double Random::rand() {
    const long Q = MODULUS / MULTIPLIER;
    const long R = MODULUS % MULTIPLIER;
    long t;

    t = MULTIPLIER * (_seed % Q) - R * (_seed / Q);
    if (t > 0)
        _seed = t;
    else
        _seed = t + MODULUS;
    return ((double) _seed / MODULUS);
}

/**
 * Random integer in an interval
 * @param minimum
 * @param maximum
 * @return
 */
long Random::randInt(long minimum, long maximum) {
    double random_d = rand();  // random between 0 and 1
    long range = maximum - minimum;
    return minimum + long(random_d * double(range));

}

/**
 * Use this function to set the state of the current random number
 * generator stream according to the following conventions:
 *    if x > 0 then x is the state (unless too large)
 *    if x < 0 then the state is obtained from the system clock
 *    if x = 0 then the state is to be supplied interactively
 * @param x
 */
void Random::setSeed(long x)  {
    if (x > 0)
        x = x % MODULUS;                       /* correct if x is too large  */
    if (x < 0)
        x = ((unsigned long) time((time_t *) NULL)) % MODULUS;
    if (x == 0)
        // x must be a positive number
        x = 1;

    _seed = x;
}

long Random::getSeed() const {return _seed;}

void Random::setTimeBasedSeed() {
    std::time_t result = std::time(nullptr);
    _seed = result % MODULUS;
}

Random::Random() {
    setTimeBasedSeed();
}