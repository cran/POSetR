//
//  LinearExtensionGenerator.h
//  POSet
//
//  Created by Alessandro Avellone on 14/05/2019.
//  Copyright © 2019 Alessandro Avellone. All rights reserved.
//

#ifndef linearExtensionGenerator_hpp
#define linearExtensionGenerator_hpp


#include <iostream>
#include <map>
#include <deque>
#include <chrono>
#include <sstream>
#include <fstream>
#include <cstdint>

#include "paramType.h"
#include "utilita.h"
#include "linearExtension.h"
#include "poset.h"
#include "linearExtentionTreeOfIdeals.h"
#include "linearExtentionLatticeOfIdeals.h"


// ***********************************************
// ***********************************************
// ***********************************************

class LinearExtensionGenerator
{
protected:
    std::shared_ptr<std::map<std::string, ParamType>> parms;
    std::uint64_t max_number_le;
    std::uint64_t current_number_le;
    std::shared_ptr<std::string> output_file_name;
    std::shared_ptr<std::fstream> file_le;
    std::shared_ptr<LinearExtension> currentLinearExtension;
    std::shared_ptr<POSet> poset;
    bool started;
public:
    LinearExtensionGenerator(std::shared_ptr<std::map<std::string, ParamType>> p);
    virtual ~LinearExtensionGenerator();
    virtual std::string to_string() const {return "";};
    virtual void start() = 0;
    virtual std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> get() = 0;
    virtual void next() = 0;
    virtual bool hasNext() = 0;
    std::uint64_t numberOfLE() const;
    std::uint64_t currentNumberOfLE() const;
    virtual void to_file();
};

// ***********************************************
// ***********************************************
// ***********************************************

class LEGBubleyDyer : public LinearExtensionGenerator
{
private:
    static std::uint64_t PRECISION;
    //using DATASTORE = std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<std::unordered_set<std::string>>>>;
    
    std::uint64_t precision;
    std::shared_ptr<Random> rnd;
    bool toUpdate;
    bool isSwitched;
    std::uint64_t positionToUpdate;

public:
    LEGBubleyDyer(std::shared_ptr<std::map<std::string, ParamType>> p);
    virtual ~LEGBubleyDyer() {}
    virtual std::string to_string() const;
    virtual void start();
    virtual void next();
    virtual bool hasNext() {
        return this->current_number_le < this->max_number_le;
    }
    virtual void to_file();

    virtual std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> get();
private:
    std::uint64_t numberOfIterations();
    std::uint64_t getSetOneElement(std::set<std::uint64_t>& setOne);
};


// ***********************************************
// ***********************************************
// ***********************************************

class LEGAllLE : public LinearExtensionGenerator
{
private:
    using EST_TYPE = std::vector<std::uint64_t>;
    using STATUS_TYPE = std::vector<bool>;
    using EST_STACK_TYPE = std::list<std::shared_ptr<EST_TYPE>>;
    using STATUS_STACK_TYPE = std::list<std::shared_ptr<STATUS_TYPE>>;

    EST_STACK_TYPE extension_stack;
    STATUS_STACK_TYPE status_stack;
    STATUS_TYPE currentStatus;
    std::uint64_t extension_stack_size;
    bool more_extensions;
public:
    LEGAllLE(std::shared_ptr<std::map<std::string, ParamType>> p);
    virtual ~LEGAllLE() {}
    virtual std::string to_string() const;
    virtual void start();
    virtual void next();
    virtual bool hasNext();
    virtual std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> get();
private:
    bool IsPossibleToSwitch(std::uint64_t p1, std::uint64_t p2);
};

// ***********************************************
// ***********************************************
// ***********************************************

class LEGByTreeOfIdeals : public LinearExtensionGenerator
{
private:
    std::uint64_t extension_stack_size;
    bool more_extensions;
    std::shared_ptr<LatticeOfIdeals>  latticeOfIdeals;
    std::shared_ptr<std::vector<std::uint64_t>> latticeOfIdealsCrossing;
    std::shared_ptr<std::vector<bool>> moreCrossing;
public:
    LEGByTreeOfIdeals(std::shared_ptr<std::map<std::string, ParamType>> p);
    virtual ~LEGByTreeOfIdeals() {}
    virtual std::string to_string() const;
    virtual void start();
    virtual void next();
    virtual bool hasNext();
    virtual std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> get();
private:
    bool IsPossibleToSwitch(std::uint64_t p1, std::uint64_t p2);
};



#endif /* linearExtensionGenerator_hpp */
