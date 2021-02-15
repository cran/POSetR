#ifndef RMod_hpp
#define RMod_hpp
#include <Rcpp.h>
#include <stdio.h>
#include <cstdint>
#include "poset.h"

// [[Rcpp::plugins(cpp17)]]

class POSetR
{
private:
    std::shared_ptr<POSet>  poset;
    
    enum class TranformExtensionType {
        TETItentity,
        TETLexicographical,
    };
    std::map<std::string, TranformExtensionType> tranformExtensionMapType {
        { "Identity", TranformExtensionType::TETItentity },
        { "Lexicographical", TranformExtensionType::TETLexicographical },
    };
    
    enum class FunctionLinearType {
        FLETMutualRankingProbability,
        FLETMutualRankingProbability2,
        FLETAverageHeight,
        FLETSeparation,
    };
    
    std::map<std::string, FunctionLinearType> functionLinearMapType {
        { "MutualRankingProbability", FunctionLinearType::FLETMutualRankingProbability },
        { "MutualRankingProbability2", FunctionLinearType::FLETMutualRankingProbability2 },
        { "AverageHeight", FunctionLinearType::FLETAverageHeight },
        { "Separation", FunctionLinearType::FLETSeparation },
    };
    
    enum class LinearExtensionGeneratorType {
        LEGTBubleyDyer,
        LEGThmnsAllLE,
        LEGTAllLE,
    };

    std::map<std::string, LinearExtensionGeneratorType> linearExtensionGeneratorMapType{
        { "BubleyDyer", LinearExtensionGeneratorType::LEGTBubleyDyer },
        { "hmnsAllLE", LinearExtensionGeneratorType::LEGThmnsAllLE },
        { "AllLE", LinearExtensionGeneratorType::LEGTAllLE },
    };
    
public:
    POSetR(Rcpp::StringVector elements1, Rcpp::StringMatrix comparabilities1, Rcpp::StringVector elements2, Rcpp::StringMatrix comparabilities2, Rcpp::String separator);
    POSetR(Rcpp::StringVector elements, Rcpp::StringMatrix comparabilities);
    POSetR(Rcpp::StringVector elements);
    ~POSetR() {};
    Rcpp::LogicalMatrix IncidenceMatrix() const;
    Rcpp::LogicalMatrix CoverMatrix() const;
    Rcpp::List coveredBy(Rcpp::StringVector) const;
    Rcpp::List evaluation(Rcpp::List);
    Rcpp::StringMatrix incomparabilities() const;
    Rcpp::StringMatrix comparabilities() const;
    Rcpp::StringVector Elements() const;
    Rcpp::StringVector firstLE() const;
    Rcpp::StringVector downSet(Rcpp::StringVector) const;
    Rcpp::StringVector upSet(Rcpp::StringVector) const;
    Rcpp::List latticeOfIdeals() const;
    void show() const;
private:
    std::shared_ptr<TranformExtension> evaluationTE(Rcpp::List args);
    std::shared_ptr<std::vector<std::shared_ptr<FunctionLinearExtension>>> evaluationFLE(Rcpp::List args, std::shared_ptr<std::map<std::uint64_t, std::string>> fleKeys);
    std::shared_ptr<LinearExtensionGenerator> evaluationLEG(Rcpp::List args);
    Rcpp::List BuildResult(std::shared_ptr<EvalResult> evaluationResult);
};


#endif /* RMod_hpp */
