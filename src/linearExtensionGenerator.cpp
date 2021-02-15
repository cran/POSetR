//
//  LinearExtensionGenerator.cpp
//  POSet
//
//  Created by Alessandro Avellone on 14/05/2019.
//  Copyright © 2019 Alessandro Avellone. All rights reserved.
//

#include "linearExtensionGenerator.h"

// ***********************************************
// ***********************************************
// ***********************************************

LinearExtensionGenerator::LinearExtensionGenerator(std::shared_ptr<std::map<std::string, ParamType>> p) : parms(p)
{
    this->file_le = nullptr;
    this->max_number_le = 0;
    this->current_number_le = 0;
    if ((*this->parms).find("LinearExtensionGeneratorOutputFile") != (*this->parms).end()) {
        //this->output_file_name = std::get<std::shared_ptr<std::string>>((*this->parms)["LinearExtensionGeneratorOutputFile"]);
        if (auto* p = std::get_if<std::shared_ptr<std::string>>(&(*this->parms)["LinearExtensionGeneratorOutputFile"])) {
            this->output_file_name = *p;
        } else {
          std::string err_str = "LinearExtensionGenerator error: LinearExtensionGeneratorOutputFile";
          throw_line(err_str);
        }
        
        if (this->output_file_name->empty()) {
            std::string start_time = std::to_string(std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count());
            (*this->output_file_name) = start_time + "_LEG" + ".txt";
        }
        this->file_le = std::make_shared<std::fstream>();
        this->file_le->open(*this->output_file_name, std::ios::out);
    }
    else {
        this->output_file_name = nullptr;
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

LinearExtensionGenerator::~LinearExtensionGenerator() {
    if (this->file_le != nullptr && this->file_le->is_open())
        this->file_le->close();
}

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t LinearExtensionGenerator::numberOfLE() const
{
    return this->max_number_le;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t LinearExtensionGenerator::currentNumberOfLE() const
{
    return this->current_number_le;
}

// ***********************************************
// ***********************************************
// ***********************************************

void LinearExtensionGenerator::to_file() {
    if (this->file_le != nullptr && this->file_le->is_open())
    {
        std::string str_le = "";
        bool first = true;
        for (std::uint64_t k = 0; k < this->currentLinearExtension->size(); ++k) {
            std::string nome_etichetta = this->poset->GetElement(this->currentLinearExtension->getVal(k));
            if (first) {
                str_le = "" + nome_etichetta;
                first = false;
            }
            else {
                str_le += DELIMETER + nome_etichetta;
            }
        }
        (*this->file_le)  << str_le;
        (*this->file_le)  << std::endl;
    }
}

// ***********************************************
// ***********************************************
// ***********************************************
