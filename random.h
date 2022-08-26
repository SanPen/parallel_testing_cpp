//
// Created by santi on 26/08/2022.
//

#ifndef NEWTON_RANDOM_H
#define NEWTON_RANDOM_H


class Random {

    const static long MODULUS = 2147483647; // DON'T CHANGE THIS VALUE
    const static long MULTIPLIER = 48271;   // DON'T CHANGE THIS VALUE
    long _seed;                             // current state of the stream

public:

    Random();

    explicit Random(long seed);

    double rand();

    long randInt(long minimum, long maximum) ;

    void setSeed(long x) ;

    [[nodiscard]] long getSeed() const;

    void setTimeBasedSeed();
};


#endif //NEWTON_RANDOM_H
