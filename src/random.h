//
//  random.h
//  pso
//
//  Created by Alessandro Avellone on 10/08/16.
//  Copyright © 2016 Alessandro Avellone. All rights reserved.
//

#ifndef random_hpp
#define random_hpp

#include <stdio.h>
#include <chrono>
#include <memory>
#include <iostream>
#include <random>
#include <limits>


// ***********************************************
// ***********************************************
// ***********************************************

class Random {
public:
    std::shared_ptr<std::uint64_t> seed = nullptr;
    virtual double RndNext() const = 0;
    virtual size_t RndNextInt(size_t, size_t) const = 0;
    virtual ~Random() {};
protected:
    static std::shared_ptr<std::uint64_t> START_SEED;
    static std::shared_ptr<Random> GENERATORE_SEED_RANDOM;
};

// ***********************************************
// ***********************************************
// ***********************************************

class RandomUni: public Random {
private:
    std::shared_ptr<std::mt19937>  generatore;
public:
    RandomUni(std::shared_ptr<std::uint64_t> s = nullptr);
    ~RandomUni() {}
    double RndNext() const;
    size_t RndNextInt(size_t min, size_t max) const;
};

#endif /* random_hpp */
