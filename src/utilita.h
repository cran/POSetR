//
//  utilita.h
//  POSet
//
//  Created by Alessandro Avellone on 09/04/2019.
//  Copyright © 2019 Alessandro Avellone. All rights reserved.
//

#ifndef utilita_hpp
#define utilita_hpp

#include <iostream>
#include "evalResult.h"
#include "displayMessage.h"

std::string& FindAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr);
std::string matri_to_string(std::vector<std::vector<double>>&);
void LoadFromFile(std::string file_name, std::vector<std::string>& elementi, std::vector<std::pair<std::string, std::string>>& comparabilita);

void split(const std::string &text, char sep, std::vector<std::string>& tokens);

//************************************
//************************************
//************************************


#endif /* utilita_hpp */
