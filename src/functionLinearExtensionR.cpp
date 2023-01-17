#include "functionLinearExtensionR.h"
#include <cstdint>

// ***********************************************
// ***********************************************
// ***********************************************


FLER::FLER(std::shared_ptr<std::map<std::string, ParamType>> p) : FunctionLinearExtension(p) {
    outputSize = nullptr;
    if (auto* p = std::get_if<std::shared_ptr<Rcpp::NumericVector>>(&(*this->parms)["FunctionLinearExtensionSize"])) {
        outputSize = *p;
    } else {
        std::string err_str = "FLER error: FunctionLinearExtensionSize";
        throw_line(err_str);
    }
    
    std::shared_ptr<std::vector<std::string>> keys1 = std::make_shared<std::vector<std::string>>();
    for (std::uint64_t k = 0; k < (*outputSize)[0]; ++k)
        keys1->push_back(std::to_string(k));

    std::shared_ptr<std::vector<std::string>> keys2 = std::make_shared<std::vector<std::string>>();
    for (std::uint64_t k = 0; k < (*outputSize)[1]; ++k)
        keys2->push_back(std::to_string(k));

    this->parmsSize.first = keys1;
    this->parmsSize.second = keys2;
    
        if (auto* p = std::get_if<std::shared_ptr<Rcpp::Function>>(&(*this->parms)["FunctionLinearExtension"])) {
        this->RFunction = *p;
    } else {
        std::string err_str = "FLER error: FunctionLinearExtension";
        throw_line(err_str);
    }
    
    for (std::uint64_t pos_first = 0; pos_first < this->parmsSize.first->size(); ++pos_first) {
        for (std::uint64_t pos_second = 0; pos_second < this->parmsSize.second->size(); ++pos_second) {
            this->result_data->push_back(std::tuple<std::uint64_t, std::uint64_t, double>(pos_first , pos_second, 0.0));
        }
    }
}


// ***********************************************
// ***********************************************
// ***********************************************

std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> FLER::size() const {
    return this->parmsSize;
}


// ***********************************************
// ***********************************************
// ***********************************************

std::string FLER::to_string() const {
    std::string base_string = FunctionLinearExtension::to_string();
    std::string result = "FLER:";
    result += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

void FLER::operator()(std::shared_ptr<LinearExtension> x) {
    ++this->calls;
    Rcpp::StringVector x_arg(x->size());

    for (size_t k = 0; k < x->size(); ++k) {
        std::string el = this->fromLEtoValue->at(x->getVal(k));
        x_arg(k) = el;
    }

    Rcpp::NumericMatrix result = (*this->RFunction)(x_arg);
    
    for (size_t k = 0; k < this->result_data->size(); ++k) {
        size_t first_idx = std::get<0>(this->result_data->at(k));
        size_t second_idx = std::get<1>(this->result_data->at(k));
        double val = result(first_idx, second_idx);
        this->result_data->at(k) = std::make_tuple(first_idx, second_idx, val);
    }
    return;
}
