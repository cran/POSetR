#include <chrono>
#include <fstream>
#include "evalResult.h"

extern const char DELIMETER;

// ***********************************************
// ***********************************************
// ***********************************************

EvalResult::EvalResult() {}

// ***********************************************
// ***********************************************
// ***********************************************

void EvalResult::push_back(std::shared_ptr<Rcpp::NumericMatrix> val) {
    this->dati.push_back(val);
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<Rcpp::NumericMatrix> EvalResult::at(size_t k)
{
    return this->dati.at(k);
}

// ***********************************************
// ***********************************************
// ***********************************************

size_t EvalResult::size() const
{
    return this->dati.size();
}

// ***********************************************
// ***********************************************
// ***********************************************

std::string EvalResult::to_string() const {
    return "";
}

// ***********************************************
// ***********************************************
// ***********************************************

void EvalResult::to_file(std::string out_file_name)
{
    
    if (out_file_name != "")
    {
        std::string START_TIME = std::to_string(std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count());
        
        size_t file_index = 1;
        for (auto matrix : this->dati)
        {
        
            std::string file_name = START_TIME + "_" + std::to_string(file_index) + "_" + out_file_name;
            // check if exists
            std::ifstream infile(file_name);
            if (infile.good())
            {
                infile.close();
                std::string err_str = "Error: out file exists (" + std::string(file_name) + ")!";
                throw std::invalid_argument(err_str);
            }
            std::fstream file_dinamica(file_name, std::ios::out);
        
            for (int row = 0; row < matrix->rows(); ++row) {
                for (int col = 0; col < matrix->cols(); ++col) {
                    file_dinamica <<  matrix->at(row, col);
                    if (col != matrix->cols() - 1) {
                        file_dinamica << DELIMETER;
                    }
                }
                if (row != matrix->rows() - 1) {
                    file_dinamica << "\n";
                }
            }
            
            file_dinamica.close();

        }
        
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

