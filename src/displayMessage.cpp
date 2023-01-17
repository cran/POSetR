#include "displayMessage.h"
#include <cstdint>


unsigned long DisplayMessageCout::OUTPUT_SEC = 10;

// ***********************************************
// ***********************************************
// ***********************************************

std::string DisplayMessage::to_string() const {
    std::string result = "";
    result += "Calls: " + std::to_string(this->calls);
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

DisplayMessageCout::DisplayMessageCout(std::shared_ptr<std::map<std::string, ParamType>> p) : DisplayMessage(p) {
    this->total_number_of_extension = nullptr;
    if ((*this->parms).find("TotalNumberExtension") != (*this->parms).end()) {
        //this->total_number_of_extension = std::get<std::shared_ptr<std::uint64_t>>((*this->parms)["TotalNumberExtension"]);
        if (auto* p = std::get_if<std::shared_ptr<std::uint64_t>>(&(*this->parms)["TotalNumberExtension"])) {
            this->total_number_of_extension = *p;
        } else {
            std::string err_str = "DisplayMessageCout error: TotalNumberExtension";
            throw_line(err_str);
        }
    }
    this->number_of_extension = nullptr;
    if ((*this->parms).find("NumberExtension") != (*this->parms).end()) {
        //this->number_of_extension = std::get<std::shared_ptr<std::uint64_t>>((*this->parms)["NumberExtension"]);
        if (auto* p = std::get_if<std::shared_ptr<std::uint64_t>>(&(*this->parms)["NumberExtension"])) {
            this->number_of_extension = *p;
        } else {
            std::string err_str = "DisplayMessageCout error: NumberExtension";
            throw_line(err_str);
        }
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

void DisplayMessageCout::Display() {
    if ((*this->total_number_of_extension) == 0) return;
    auto inteval = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - last_output);
    if ((last_output == start) || ((unsigned long) inteval.count()) > OUTPUT_SEC) {
        long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
        Rcpp::Rcout << (elapsed / 1000) << ",";
        Rcpp::Rcout << (elapsed % 1000) << "sec. " << std::endl;
        Rcpp::Rcout << "Linear extension analized " << (*this->number_of_extension) << " of " << (*this->total_number_of_extension) << std::endl;
        last_output = std::chrono::high_resolution_clock::now();
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

void DisplayMessageCout::Start() {
    this->start = std::chrono::high_resolution_clock::now();
    this->last_output = start;
}

// ***********************************************
// ***********************************************
// ***********************************************
