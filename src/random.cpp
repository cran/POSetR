//
//  random.cpp
//  POSet
//
//  Created by Alessandro Avellone on 03/03/2020.
//  Copyright © 2020 Alessandro Avellone. All rights reserved.
//

#include <chrono>
#include "random.h"

std::shared_ptr<std::uint64_t> Random::START_SEED = std::make_shared<std::uint64_t>((std::uint64_t) std::chrono::high_resolution_clock::now().time_since_epoch().count());
std::shared_ptr<Random> Random::GENERATORE_SEED_RANDOM = std::make_shared<RandomUni>(Random::START_SEED);

// ***********************************************
// ***********************************************
// ***********************************************

RandomUni::RandomUni(std::shared_ptr<std::uint64_t> s) {
    this->seed = s;
    std::random_device rdev{};
    this->generatore = std::make_shared<std::mt19937>(rdev());
    if (this->seed == nullptr) {
        this->seed = std::make_shared<std::uint64_t>(Random::GENERATORE_SEED_RANDOM->RndNextInt(0, std::numeric_limits<size_t>::max()));
    }
    this->generatore->seed((unsigned int) *seed);
}

// ***********************************************
// ***********************************************
// ***********************************************

double RandomUni::RndNext() const {
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    double risultato = dis(*this->generatore);
    return risultato;
}

// ***********************************************
// ***********************************************
// ***********************************************

size_t RandomUni::RndNextInt(size_t min, size_t max) const {
    std::uniform_int_distribution<size_t> dis(min, max);

    size_t risultato = dis(*this->generatore);
    return risultato;
}

// ***********************************************
// ***********************************************
// ***********************************************
