#include <vector>

#include "linearExtension.h"
#include "functionLinearExtension.h"

// ***********************************************
// ***********************************************
// ***********************************************

FLEAverageHeight::FLEAverageHeight(std::shared_ptr<std::map<std::string, ParamType>> p) : FunctionLinearExtension(p) {
    this->result_data->clear();
    for( std::map<std::uint64_t, std::string>::iterator it = this->fromLEtoValue->begin(); it != this->fromLEtoValue->end(); ++it ) {
        this->result_data->push_back(std::tuple<std::uint64_t, std::uint64_t, double>(it->first , 0, 0.0));
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> FLEAverageHeight::size()  const
{
    std::shared_ptr<std::vector<std::string>> keys1 = std::make_shared<std::vector<std::string>>();
    for( std::map<std::uint64_t, std::string>::iterator it = this->fromLEtoValue->begin(); it != this->fromLEtoValue->end(); ++it ) {
        keys1->push_back(it->second);
    }
    std::shared_ptr<std::vector<std::string>> keys2 = std::make_shared<std::vector<std::string>>();
    keys2->push_back("0");
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> risultato(keys1, keys2);
    return risultato;
}

// ***********************************************
// ***********************************************
// ***********************************************

void FLEAverageHeight::operator()(std::shared_ptr<LinearExtension> x) {
    ++this->calls;
    for (size_t k = 0; k < this->result_data->size(); ++k) {
        size_t value = std::get<0>(this->result_data->at(k));
        size_t pos = x->getPos(value);
        this->result_data->at(k) = std::make_tuple(value, 0, pos + 1);
    }
    
    return;
}


