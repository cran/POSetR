//
//  linearExtension.h
//  POSet
//
//  Created by Alessandro Avellone on 11/04/2019.
//  Copyright © 2019 Alessandro Avellone. All rights reserved.
//

#ifndef linearExtension_hpp
#define linearExtension_hpp

#include <vector>
#include <map>
#include <iostream>
#include <cstdint>
extern const char DELIMETER;

class LinearExtension
{
private:
    std::vector<std::uint64_t> by_position;
    std::map<std::uint64_t, std::uint64_t> by_element;  // key is poset element; value is position of the key element in the extension
public:
    LinearExtension(size_t s) {
        this->by_position.resize(s);
    }
    
    // ***********************************************
    // ***********************************************
    // ***********************************************

    std::string to_string() {
        std::string result = "";
        bool first = true;
        for (std::uint64_t k = 0; k < this->by_position.size(); ++k) {
            if (first) {
                result = "" + std::to_string(this->by_position[k]);
                first = false;
            }
            else {
                result += DELIMETER + std::to_string(this->by_position[k]);
            }
        }
        return result;
    }
    
    // ***********************************************
    // ***********************************************
    // ***********************************************

    size_t size() const {
        return this->by_position.size();
    }
    
    // ***********************************************
    // ***********************************************
    // ***********************************************

    std::uint64_t getVal(size_t idx) const {
        return this->by_position.at(idx);
    }
    
    // ***********************************************
    // ***********************************************
    // ***********************************************

    size_t getPos(std::uint64_t e) const {
        return this->by_element.at(e);
    }
    
    // ***********************************************
    // ***********************************************
    // ***********************************************

    void set(std::uint64_t idx, std::uint64_t val) {
        this->by_position[idx] = val;
        this->by_element[val] = idx;
    }
    
    // ***********************************************
    // ***********************************************
    // ***********************************************

};


#endif /* linearExtension_hpp */
