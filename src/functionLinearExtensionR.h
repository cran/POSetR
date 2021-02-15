//
//  functionLinearExtensionR.hpp
//  POSet
//
//  Created by Alessandro Avellone on 27/09/2019.
//  Copyright © 2019 Alessandro Avellone. All rights reserved.
//

#ifndef functionLinearExtensionR_hpp
#define functionLinearExtensionR_hpp

#include <vector>
#include <Rcpp.h>

#include "linearExtension.h"
#include "functionLinearExtension.h"

class FLER : public FunctionLinearExtension {
    //************************************
    //************************************
    //************************************
    //** CALL R function
    //************************************
    //************************************
    //************************************
private:
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> parmsSize;
    std::shared_ptr<Rcpp::Function> RFunction;
    std::shared_ptr<Rcpp::NumericVector> outputSize;
public:
    FLER(std::shared_ptr<std::map<std::string, ParamType>> p);
    std::string to_string() const;
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> size() const;
    void operator()(std::shared_ptr<LinearExtension> x);
};

#endif

