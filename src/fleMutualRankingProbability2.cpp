#include <vector>
#include <cstdint>

#include "linearExtension.h"
#include "functionLinearExtension.h"

// ***********************************************
// ***********************************************
// ***********************************************

FLEMutualRankingProbability2::FLEMutualRankingProbability2(std::shared_ptr<std::map<std::string, ParamType>> p) : FunctionLinearExtension(p) {
    this->result_data->clear();

    this->keys_map = std::make_shared<std::map<std::string, std::uint64_t>>();
    if (this->parms->find("FLEMRPSelection") != this->parms->end()) {
        if (auto* p = std::get_if<std::shared_ptr<Rcpp::StringMatrix>>(&(*this->parms)["FLEMRPSelection"])) {
            this->incomparabilities = *p;
        } else {
            std::string err_str = "FLEMutualRankingProbability2 error: FLEMRPSelection";
            throw_line(err_str);
        }
        
        this->keys = std::make_shared<std::vector<std::string>>(this->incomparabilities->rows());
        for (int k = 0; k < this->incomparabilities->rows(); ++k) {
            std::string first_str = Rcpp::as<std::string>((*this->incomparabilities)(k, 0));
            std::string second_str = Rcpp::as<std::string>((*this->incomparabilities)(k, 1));
            std::uint64_t first = (*this->fromValuetoLE)[first_str];
            std::uint64_t second = (*this->fromValuetoLE)[second_str];
            this->keys->at(k) = first_str + "<" + second_str;
            std::string keys_map_v = std::to_string(first) + "<" + std::to_string(second);
            (*this->keys_map)[keys_map_v] = k;
            this->result_data->push_back(std::tuple<std::uint64_t, std::uint64_t, double>(first, second, 0));
        }
    }
    else {
        this->incomparabilities = nullptr;
        this->keys = std::make_shared<std::vector<std::string>>(this->fromLEtoValue->size() * this->fromLEtoValue->size());
        std::uint64_t k = 0;
        for (auto it1 : (*this->fromLEtoValue)) {
            for (auto it2 : (*this->fromLEtoValue)) {
                std::uint64_t first = it1.first;
                std::uint64_t second = it2.first;
                this->result_data->push_back(std::tuple<std::uint64_t, std::uint64_t, double>(first, second, 0));
                this->keys->at(k) = it1.second + "<" + it2.second;
                std::string keys_map_v = std::to_string(first) + "<" + std::to_string(second);
                (*this->keys_map)[keys_map_v] = k;
                ++k;
            }
        }
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> FLEMutualRankingProbability2::size() const
{
    std::shared_ptr<std::vector<std::string>> keys2 = std::make_shared<std::vector<std::string>>();
    keys2->push_back("0");
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> risultato(this->keys, keys2);
    return risultato;
}

// ***********************************************
// ***********************************************
// ***********************************************

void FLEMutualRankingProbability2::operator()(std::shared_ptr<LinearExtension> x)
{
    ++this->calls;
    for (size_t k = 0; k < this->result_data->size(); ++k) {
        std::uint64_t first_idx = std::get<0>(this->result_data->at(k));
        std::uint64_t second_idx = std::get<1>(this->result_data->at(k));
        size_t first_pos = x->getPos(first_idx);
        size_t second_pos = x->getPos(second_idx);
        this->result_data->at(k) = std::make_tuple(first_idx, second_idx, first_pos <= second_pos);
    }
    
    return;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t FLEMutualRankingProbability2::at0(std::uint64_t k) const {
    std::tuple<std::uint64_t, std::uint64_t, double>& value = this->result_data->at(k);
    std::uint64_t first = std::get<0>(value);
    std::uint64_t second = std::get<1>(value);
    std::string keys_map_v = std::to_string(first) + "<" + std::to_string(second);
    std::uint64_t r_value = (*this->keys_map)[keys_map_v];
    return r_value;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t FLEMutualRankingProbability2::at1(std::uint64_t k) const {
    return 0;
}

