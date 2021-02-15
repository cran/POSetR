#ifndef functionLinearExtension_hpp
#define functionLinearExtension_hpp


#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>      // std::out_of_range
#include <limits>
#include <memory>
#include <algorithm>
#include <tuple>

#include "linearExtension.h"
#include "utilita.h"
#include "paramType.h"
#include "poset.h"

class FunctionLinearExtension
{
private:
    static std::map<std::string, FunctionLinearExtension* (*)(std::shared_ptr<std::map<std::string, void*>> p)> map_type;
protected:
    std::uint64_t calls;
    //POSet* poset__ = nullptr;
    std::shared_ptr<std::vector<std::tuple<std::uint64_t, std::uint64_t, double>>> result_data;
    size_t result_data_used;
    std::shared_ptr<std::map<std::string, ParamType>> parms;
    std::shared_ptr<std::map<std::string, std::uint64_t>> fromValuetoLE;
    std::shared_ptr<std::map<std::uint64_t, std::string>> fromLEtoValue;
    std::shared_ptr<std::map<std::uint64_t, std::uint64_t>> fromLEtoResult;
public:
    static double ERRORE;
    FunctionLinearExtension(std::shared_ptr<std::map<std::string, ParamType>> p) : parms(p)
    {
        this->calls = 0;
        this->result_data = std::make_shared<std::vector<std::tuple<std::uint64_t, std::uint64_t, double>>>();
        this->result_data_used = 0;
        
        //this->fromLEtoValue = std::get<std::shared_ptr<std::map<std::uint64_t, std::string>>>((*this->parms)["FLEKeys"]);
        if (auto* p = std::get_if<std::shared_ptr<std::map<std::uint64_t, std::string>>>(&(*this->parms)["FLEKeys"])) {
            this->fromLEtoValue = *p;
        } else {
          std::string err_str = "FunctionLinearExtension error: FLEKeys";
          throw_line(err_str);
        }
        
        this->fromValuetoLE = std::make_shared<std::map<std::string, std::uint64_t>>();
        this->fromLEtoResult = std::make_shared<std::map<std::uint64_t, std::uint64_t>>();
        std::uint64_t k = 0;
        for( std::map<std::uint64_t, std::string>::iterator it = this->fromLEtoValue->begin(); it != this->fromLEtoValue->end(); ++it ) {
            (*this->fromValuetoLE)[it->second] = it->first;
            (*this->fromLEtoResult)[it->first] = k;
            ++k;
        }
    }
    virtual ~FunctionLinearExtension() {};
    virtual std::string to_string() const {
        std::string result = "";
        result += "Calls: " + std::to_string(this->calls);
        return result;
    }
    
    virtual std::uint64_t at0(std::uint64_t k) const {
        std::tuple<std::uint64_t, std::uint64_t, double>& value = this->result_data->at(k);
        std::uint64_t le_value = std::get<0>(value);
        std::uint64_t r_value = (*this->fromLEtoResult)[le_value];
        return r_value;
    }
    
    virtual std::uint64_t at1(std::uint64_t k) const {
        std::tuple<std::uint64_t, std::uint64_t, double>& value = this->result_data->at(k);
        std::uint64_t le_value = std::get<1>(value);
        std::uint64_t r_value = (*this->fromLEtoResult)[le_value];
        return r_value;
    }
    
    virtual double at2(std::uint64_t k) const {
        std::tuple<std::uint64_t, std::uint64_t, double>& value = this->result_data->at(k);
        return std::get<2>(value);
    }
    
    std::uint64_t resSize() const {
        return this->result_data->size();
    }
    
    virtual void operator()(std::shared_ptr<LinearExtension>) = 0;
    virtual std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> size() const = 0;
};


//************************************
//************************************
//************************************

class FLETest : public FunctionLinearExtension
{
public:
    FLETest(std::shared_ptr<std::map<std::string, ParamType>> p) : FunctionLinearExtension(p) {
        this->result_data->clear();
    }

    std::string to_string() const {
        std::string base_string = FunctionLinearExtension::to_string();
        std::string result = "FLETest:";
        result += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
        return result;
    }

    // ***********************************************
    // ***********************************************
    // ***********************************************

    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> size() const {
        std::shared_ptr<std::vector<std::string>> keys1 = std::make_shared<std::vector<std::string>>();
        std::shared_ptr<std::vector<std::string>> keys2 = std::make_shared<std::vector<std::string>>();
        keys1->push_back("0");
        keys2->push_back("0");
        std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> risultato(keys1, keys2);
        return risultato;
    }

    // ***********************************************
    // ***********************************************
    // ***********************************************

    void operator()(std::shared_ptr<LinearExtension> x) {
        ++this->calls;
        std::string val = "";
        for (size_t k = 0; k < x->size(); ++k) {
            std::string el = this->fromLEtoValue->at(x->getVal(k));
            val += el + ";";
        }
        return;
    }
};

//************************************
//************************************
//************************************

class FLEMutualRankingProbability : public FunctionLinearExtension
{
    //************************************
    //************************************
    //************************************
    //** \{(i, j, 1) \mid i <_{LE} j\}_{(i,j)\in ICP} \bigcup \{(i, j, 0) \mid i \not<_{LE} j\}_{(i,j)\in ICP}
    //** LE = linear extension
    //** ICP = Incoparability set in Poset
    //************************************
    //************************************
    //************************************
private:
    std::shared_ptr<Rcpp::StringMatrix> incomparabilities;
public:
    FLEMutualRankingProbability(std::shared_ptr<std::map<std::string, ParamType>> p);
    std::string to_string() const {
        std::string base_string = FunctionLinearExtension::to_string();
        std::string result = "FLEMutualRankingProbability:";
        result += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
        return result;
    }
    virtual void operator()(std::shared_ptr<LinearExtension> x);
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> size() const;
};

//************************************
//************************************
//************************************

class FLEMutualRankingProbability2 : public FunctionLinearExtension
{
    //************************************
    //************************************
    //************************************
    //** \{(i, j, 1) \mid i <_{LE} j\}_{(i,j)\in ICP} \bigcup \{(i, j, 0) \mid i \not<_{LE} j\}_{(i,j)\in ICP}
    //** LE = linear extension
    //** ICP = Incoparability set in Poset
    //************************************
    //************************************
    //************************************
private:
    std::shared_ptr<Rcpp::StringMatrix> incomparabilities;
    std::shared_ptr<std::vector<std::string>> keys;
    std::shared_ptr<std::map<std::string, std::uint64_t>> keys_map;
public:
    FLEMutualRankingProbability2(std::shared_ptr<std::map<std::string, ParamType>> p);
    std::string to_string() const {
        std::string base_string = FunctionLinearExtension::to_string();
        std::string result = "FLEMutualRankingProbability2:";
        result += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
        return result;
    }
    virtual void operator()(std::shared_ptr<LinearExtension> x);
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> size() const;
    virtual std::uint64_t at0(std::uint64_t k) const;
    virtual std::uint64_t at1(std::uint64_t k) const;
};


//************************************
//************************************
//************************************

class FLEAverageHeight : public FunctionLinearExtension
{
    //************************************
    //************************************
    //************************************
    //** \{(i, j, 1) \mid i <_{LE} j\}_{(i,j)\in ICP} \bigcup \{(i, j, 0) \mid i \not<_{LE} j\}_{(i,j)\in ICP}
    //** LE = linear extension
    //** ICP = Incoparability set in Poset
    //************************************
    //************************************
    //************************************
private:
public:
    FLEAverageHeight(std::shared_ptr<std::map<std::string, ParamType>> p);
    std::string to_string() const
    {
        std::string base_string = FunctionLinearExtension::to_string();
        std::string result = "FLEAverageHeight:";
        result += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
        return result;
    }

    virtual void operator()(std::shared_ptr<LinearExtension> x);
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> size()  const;
};

//************************************
//************************************
//************************************

class FLESeparation : public FunctionLinearExtension
{
    //************************************
    //************************************
    //************************************
    //** \{(i, j, k) \mid i <_{LE} j and k = distance(j, i) \}_{(i,j)\in ICP} \bigcup \{(i, j, 0) \mid i \not<_{LE} j\}_{(i,j)\in ICP}
    //** LE = linear extension
    //** ICP = Incoparability set in Poset
    //************************************
    //************************************
    //************************************
private:
    std::shared_ptr<Rcpp::StringMatrix> incomparabilities;
public:
    FLESeparation(std::shared_ptr<std::map<std::string, ParamType>> p);
    std::string to_string() const {
        std::string base_string = FunctionLinearExtension::to_string();
        std::string result = "FLESeparation:";
        result += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
        return result;
    }
    virtual void operator()(std::shared_ptr<LinearExtension> x);
    std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> size() const;
};
#endif /* functionLinearExtension_hpp */
