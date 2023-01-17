#include <vector>
#include <cstdint>

#include "linearExtension.h"
#include "functionLinearExtension.h"

// ***********************************************
// ***********************************************
// ***********************************************

FLEMutualRankingProbability::FLEMutualRankingProbability(std::shared_ptr<std::map<std::string, ParamType>> p) : FunctionLinearExtension(p) {
    if (this->parms->find("FLEMRPSelection") != this->parms->end()) {
        if (auto* p = std::get_if<std::shared_ptr<Rcpp::StringMatrix>>(&(*this->parms)["FLEMRPSelection"])) {
            this->incomparabilities = *p;
        } else {
            std::string err_str = "FLEMutualRankingProbability error: FLEMRPSelection";
            throw_line(err_str);
        }
    }
    else
        this->incomparabilities = nullptr;
    
    
    this->result_data->clear();
    if (this->incomparabilities != nullptr) {
        for (int k = 0; k < this->incomparabilities->rows(); ++k) {
            std::string first_str = Rcpp::as<std::string>((*this->incomparabilities)(k, 0));
            std::string second_str = Rcpp::as<std::string>((*this->incomparabilities)(k, 1));

            std::uint64_t first = (*this->fromValuetoLE)[first_str];
            std::uint64_t second = (*this->fromValuetoLE)[second_str];
            this->result_data->push_back(std::tuple<std::uint64_t, std::uint64_t, double>(first, second, 0));
        }
    } else {
        for (auto it1 : (*this->fromLEtoValue)) {
            for (auto it2 : (*this->fromLEtoValue)) {
                std::uint64_t first = it1.first;
                std::uint64_t second = it2.first;
                this->result_data->push_back(std::tuple<std::uint64_t, std::uint64_t, double>(first, second, 0));
            }
        }
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> FLEMutualRankingProbability::size() const
{
    std::shared_ptr<std::vector<std::string>> keys = std::make_shared<std::vector<std::string>>();
    for( std::map<std::uint64_t, std::string>::iterator it = this->fromLEtoValue->begin(); it != this->fromLEtoValue->end(); ++it ) {
        keys->push_back(it->second);
    }
    
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> risultato(keys, keys);
    return risultato;
}

// ***********************************************
// ***********************************************
// ***********************************************

void FLEMutualRankingProbability::operator()(std::shared_ptr<LinearExtension> x)
{
    ++this->calls;
    for (size_t k = 0; k < this->result_data->size(); ++k)
    {
        std::uint64_t first_idx = std::get<0>(this->result_data->at(k));
        std::uint64_t second_idx = std::get<1>(this->result_data->at(k));
        size_t first_pos = x->getPos(first_idx);
        size_t second_pos = x->getPos(second_idx);
        this->result_data->at(k) = std::make_tuple(first_idx, second_idx, first_pos <= second_pos);
    }
    
    return;
}


