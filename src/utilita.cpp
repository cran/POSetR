//
//  utilita.cpp
//  POSet
//
//  Created by Alessandro Avellone on 09/04/2019.
//  Copyright © 2019 Alessandro Avellone. All rights reserved.
//

#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <mutex>

#include "utilita.h"

std::mutex semaphore_cout;
extern const char DELIMETER;

// ***********************************************
// ***********************************************
// ***********************************************

std::string& FindAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    
    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos = data.find(toSearch, pos + replaceStr.size());
    }
    return data;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::string matri_to_string(std::vector<std::vector<double>>& v)
{
    std::string r = "";
    for (size_t k = 0; k <  v.size(); ++k)
    {
        bool first = true;
        for (size_t h = 0; h < v.at(k).size(); ++h)
        {
            if (first)
            {
                r += std::to_string(v.at(k).at(h));
                first = false;
            }
            else
            {
                r += DELIMETER + std::to_string(v.at(k).at(h));
            }
        }
        r += "\n";
    }
    
    return r;
}

// ***********************************************
// ***********************************************
// ***********************************************

void LoadFromFile(std::string file_name,
                  std::vector<std::string>& elementi,
                  std::vector<std::pair<std::string, std::string>>& comparabilita)
{
    std::ifstream fp(file_name);
    if (!fp.good()) {
        throw std::invalid_argument{"File not found: " + std::string(file_name)};
    }
    
    std::string line;
    
    elementi.clear();
    comparabilita.clear();
    bool comparabilita_started = false;
    long line_number = 0;
    while (getline(fp, line))
    {
        ++line_number;
        std::vector<std::string> tokens;
        split(line, DELIMETER, tokens);
        if (tokens.size() == 1)
        {
            if (comparabilita_started == true)
            {
                throw std::invalid_argument{"Wrong file: " + std::to_string(line_number)};
            }
            elementi.push_back(tokens.at(0));

        } else if (tokens.size() == 2)
        {
            comparabilita_started = true;
            comparabilita.push_back(std::pair<std::string, std::string>(tokens.at(0), tokens.at(1)));
        } else
        {
            throw std::invalid_argument{"Wrong file: " + std::to_string(line_number)};
        }

    }
    
    
}


//************************************
//************************************
//************************************

void split(const std::string &text, char sep, std::vector<std::string>& tokens)
{
    tokens.clear();
    size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}

//************************************
//************************************
//************************************
