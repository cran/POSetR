#ifndef EvalResult_hpp
#define EvalResult_hpp

#include <memory>
#include <list>
#include <vector>
#include <string>
#include <Rcpp.h>

// ***********************************************
// ***********************************************
// ***********************************************

class EvalResult {
private:
    std::vector<std::shared_ptr<Rcpp::NumericMatrix>> dati;
public:
    EvalResult();
    void push_back(std::shared_ptr<Rcpp::NumericMatrix>);
    std::shared_ptr<Rcpp::NumericMatrix> at(size_t k);
    size_t size() const;
    std::string to_string() const;
    void to_file(std::string);
};

// ***********************************************
// ***********************************************
// ***********************************************

#endif /* EvalResult_hpp */
