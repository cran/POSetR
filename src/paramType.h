

#ifndef paramType_h
#define paramType_h

#include <variant>
#include <iterator>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <cstdint>

#include <Rcpp.h>
#include "myException.h"

class POSet;
class LinearExtensionGenerator;
class FunctionLinearExtension;
class Random;
class TranformExtension;

//************************************
//************************************
//************************************

#define throw_line(arg) throw my_exception(arg, __FILE__, __LINE__);



typedef std::variant<
std::shared_ptr<std::map<std::uint64_t, std::string>>,
std::shared_ptr<std::vector<std::uint64_t>>,

std::shared_ptr<Rcpp::Function>,
std::shared_ptr<Rcpp::NumericVector>,
std::shared_ptr<Rcpp::StringMatrix>,
std::shared_ptr<Rcpp::List>,

std::shared_ptr<POSet>,
std::shared_ptr<std::vector<std::shared_ptr<FunctionLinearExtension>>>,
std::shared_ptr<LinearExtensionGenerator>,
std::shared_ptr<TranformExtension>,

std::shared_ptr<std::string>,
std::shared_ptr<std::uint64_t>,

std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>>,

std::uint64_t
> ParamType;


/*
class ParamType {
private:
    enum Type {
        NONE,
        BOOL,
        UI64,
        I64,
        DOUBLE,
        SPUINT64,
        SPSTRING,
        SPVSTRING,
        SPVUINT64,
        SPVPAIRUINT64UINT64,
        SPVBOOL,
        SPVDOUBLE,
        SPVVDOUBLE,
        SPVVString,
        SPRANDOM,
        SPuint64_t,
        SPfstream,
        SPPOSet,
        SPFLE,
        SPLEG,
        SPTE,
        SPRcppNumericVector,
        SPRcppStringMatrix,
        SPRcppList,
        SPRcppFunction,
        SPMapElements,
    };
    bool value_boolean;
    std::uint64_t value_ui64;
    std::int64_t value_i64;
    double value_double;
    std::shared_ptr<std::string> value_sp_string;
    std::shared_ptr<std::uint64_t> value_sp_uint64_t;
    std::shared_ptr<std::fstream> value_sp_fstream;
    std::shared_ptr<Rcpp::NumericVector> sp_rcpp_numeric_vector;
    std::shared_ptr<Rcpp::StringMatrix> sp_rcpp_string_matrix;
    std::shared_ptr<Rcpp::List> sp_rcpp_list;
    std::shared_ptr<Rcpp::Function> sp_rcpp_function;
    std::shared_ptr<Random> value_sp_random;
    std::shared_ptr<POSet> value_sp_poset;
    std::shared_ptr<FunctionLinearExtension> value_sp_fle;
    std::shared_ptr<LinearExtensionGenerator> value_sp_leg;
    std::shared_ptr<TranformExtension> value_sp_te;

    std::shared_ptr<std::vector<bool>> value_sp_v_bool;
    std::shared_ptr<std::vector<double>> value_sp_v_double;
    std::shared_ptr<std::vector<std::string>> value_sp_v_string;
    std::shared_ptr<std::vector<std::uint64_t>> value_sp_v_uint64_t;

    std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> value_sp_v_pair_uint64_uint64;

    std::shared_ptr<std::vector<std::vector<double>>> value_sp_vv_double;
    std::shared_ptr<std::vector<std::vector<std::string>>> value_sp_vv_string;
    
    std::shared_ptr<std::map<std::uint64_t, std::string>> value_sp_vv_map_elements;
    std::shared_ptr<std::map<std::uint64_t, std::uint64_t>> value_sp_uu_map_elements;

    Type field;
public:
    ParamType() {this->field = NONE;}
    std::string to_string() const {
        std::string result = "";
        switch (this->field) {
            case BOOL:
                result = std::to_string(value_boolean);
                break;
            case UI64:
                result = std::to_string(value_ui64);
                break;
            case I64:
                result = std::to_string(value_i64);
                break;
            case DOUBLE:
                result = std::to_string(value_double);
                break;
            case SPSTRING:
                result = *value_sp_string;
                break;
            case SPVSTRING: {
                std::ostringstream oss;
                if (!value_sp_v_string->empty())
                {
                    std::copy(value_sp_v_string->begin(), value_sp_v_string->end() -1, std::ostream_iterator<std::string>(oss, ", "));
                    oss << value_sp_v_string->back();
                }
                result = oss.str();
                break;
            }
            case SPVDOUBLE:{
                std::ostringstream oss;
                if (!value_sp_v_double->empty())
                {
                    std::copy(value_sp_v_double->begin(), value_sp_v_double->end() -1, std::ostream_iterator<double>(oss, ", "));
                    oss << value_sp_v_double->back();
                }
                result = oss.str();
                break;
            }
            default:
                break;
        }
        return result;
    }
    
    virtual ParamType& operator=(bool f) {
        this->value_boolean = f;
        this->field = BOOL;
        return *this;
    }
    
    virtual ParamType& operator=(std::uint64_t f) {
        this->value_ui64 = f;
        this->field = UI64;
        return *this;
    }
    
    virtual ParamType& operator=(std::int64_t f) {
        this->value_i64 = f;
        this->field = I64;
        return *this;
    }
    
    virtual ParamType& operator=(double f) {
        this->value_double = f;
        this->field = DOUBLE;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::string> f) {
        this->value_sp_string = f;
        this->field = SPSTRING;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::vector<std::string>> f) {
        this->value_sp_v_string = f;
        this->field = SPVSTRING;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::vector<std::uint64_t>> f) {
        this->value_sp_v_uint64_t = f;
        this->field = SPVUINT64;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> f) {
        this->value_sp_v_pair_uint64_uint64 = f;
        this->field = SPVPAIRUINT64UINT64;
        return *this;
    }
    
    
    
    virtual ParamType& operator=(const std::shared_ptr<std::vector<std::vector<double>>> f) {
        this->value_sp_vv_double = f;
        this->field = SPVVDOUBLE;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::vector<std::vector<std::string>>> f) {
        this->value_sp_vv_string = f;
        this->field = SPVVString;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::map<std::uint64_t, std::string>> f) {
        this->value_sp_vv_map_elements = f;
        this->field = SPMapElements;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::map<std::uint64_t, std::uint64_t>> f) {
        this->value_sp_uu_map_elements = f;
        this->field = SPMapElements;
        return *this;
    }
    
    
    virtual ParamType& operator=(const std::shared_ptr<std::vector<bool>> f) {
        this->value_sp_v_bool = f;
        this->field = SPVBOOL;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::vector<double>> f) {
        this->value_sp_v_double = f;
        this->field = SPVDOUBLE;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<Random> f) {
        this->value_sp_random = f;
        this->field = SPRANDOM;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<POSet> f) {
        this->value_sp_poset = f;
        this->field = SPPOSet;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<FunctionLinearExtension> f) {
        this->value_sp_fle = f;
        this->field = SPFLE;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<LinearExtensionGenerator> f) {
        this->value_sp_leg = f;
        this->field = SPLEG;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<TranformExtension> f) {
        this->value_sp_te = f;
        this->field = SPTE;
        return *this;
    }
    

    virtual ParamType& operator=(const std::shared_ptr<std::fstream> f) {
        this->value_sp_fstream = f;
        this->field = SPfstream;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<std::uint64_t> f) {
        this->value_sp_uint64_t = f;
        this->field = SPuint64_t;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<Rcpp::NumericVector>& f) {
        this->sp_rcpp_numeric_vector = f;
        this->field = SPRcppNumericVector;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<Rcpp::StringMatrix>& f) {
        this->sp_rcpp_string_matrix = f;
        this->field = SPRcppStringMatrix;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<Rcpp::List>& f) {
        this->sp_rcpp_list = f;
        this->field = SPRcppList;
        return *this;
    }
    
    virtual ParamType& operator=(const std::shared_ptr<Rcpp::Function>& f) {
        this->sp_rcpp_function = f;
        this->field = SPRcppFunction;
        return *this;
    }
    
    bool getBool() const {
        return value_boolean;
    }
    
    std::uint64_t getUI64() const {
        return value_ui64;
    }
    
    std::int64_t getI64() const {
        return value_i64;
    }
    
    double getDouble() const {
        return value_double;
    }
    
    std::shared_ptr<std::string> getSPString() const {
        return value_sp_string;
    }
    
    const std::shared_ptr<std::vector<std::string>> getSPVString() const {
        return value_sp_v_string;
    }
    
    const std::shared_ptr<std::vector<std::uint64_t>> getSPVUINT64() const {
        return value_sp_v_uint64_t;
    }
    
    const std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> getSPVPairUINT64UINT64() const {
        return value_sp_v_pair_uint64_uint64;
    }
    
    const std::shared_ptr<std::vector<bool>> getSPVBool() const {
        return value_sp_v_bool;
    }
    
    const std::shared_ptr<std::vector<double>> getSPVDouble() const {
        return value_sp_v_double;
    }
    
    const std::shared_ptr<std::vector<std::vector<double>>> getSPVVDouble() const {
        return value_sp_vv_double;
    }
    
    const std::shared_ptr<std::vector<std::vector<std::string>>> getSPVVString() const {
        return value_sp_vv_string;
    }
    
    const std::shared_ptr<std::map<std::uint64_t, std::string>> getSPMapElements() const {
        return value_sp_vv_map_elements;
    }
    
    const std::shared_ptr<std::map<std::uint64_t, std::uint64_t>> getSPMapUUElements() const {
        return value_sp_uu_map_elements;
    }
    
    const std::shared_ptr<Random> getSPRandom() const {
        return value_sp_random;
    }
    
    const std::shared_ptr<POSet> getSPPOSet() const {
        return value_sp_poset;
    }
    
    const std::shared_ptr<FunctionLinearExtension> getSPFLE() const {
        return value_sp_fle;
    }
    
    const std::shared_ptr<LinearExtensionGenerator> getSPLEG() const {
        return value_sp_leg;
    }
    
    const std::shared_ptr<TranformExtension> getSPTE() const {
        return value_sp_te;
    }
    
    const std::shared_ptr<std::fstream> getSPfstream() const {
        return value_sp_fstream;
    }
    
    const std::shared_ptr<std::uint64_t> getSPuint64_t() const {
        return value_sp_uint64_t;
    }
    
    const std::shared_ptr<Rcpp::NumericVector>& getSPRcppNumericVector() const {
        return sp_rcpp_numeric_vector;
    }
    
    const std::shared_ptr<Rcpp::StringMatrix>& getSPRcppStringMatrix() const {
        return sp_rcpp_string_matrix;
    }
    
    const std::shared_ptr<Rcpp::List>& getSPRcppList() const {
        return sp_rcpp_list;
    }
    
    const std::shared_ptr<Rcpp::Function>& getSPRcppFunction() const {
        return sp_rcpp_function;
    }
};
*/


#endif /* paramType_h */
