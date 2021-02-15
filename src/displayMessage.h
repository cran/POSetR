//
//  displayMessage.hpp
//  POSet
//
//  Created by Alessandro Avellone on 26/09/2019.
//  Copyright © 2019 Alessandro Avellone. All rights reserved.
//

#ifndef displayMessage_hpp
#define displayMessage_hpp

#include <memory>
#include <string>
#include <map>
#include <chrono>
#include <iostream>
#include "paramType.h"

// ***********************************************
// ***********************************************
// ***********************************************

class DisplayMessage {
private:
protected:
    std::uint64_t calls;
    std::shared_ptr<std::map<std::string, ParamType>> parms;
public:
    DisplayMessage(std::shared_ptr<std::map<std::string, ParamType>> p) : parms(p) {
        this->calls = 0;
    }
    virtual ~DisplayMessage() {};
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Display() = 0;
    virtual std::string to_string() const;
};

// ***********************************************
// ***********************************************
// ***********************************************

class DisplayMessageCout : public DisplayMessage {
private:
    static unsigned long OUTPUT_SEC;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_output;
    std::shared_ptr<std::uint64_t> total_number_of_extension;
    std::shared_ptr<std::uint64_t> number_of_extension;
public:
    DisplayMessageCout(std::shared_ptr<std::map<std::string, ParamType>> p);
    virtual void Display();
    virtual void Start();
    virtual void Stop() {};
};

#endif /* displayMessage_hpp */
