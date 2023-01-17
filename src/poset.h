#ifndef poset_hpp
#define poset_hpp

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <deque>
#include <map>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <Rcpp.h>
#include <cstdint>

#include "displayMessage.h"
#include "matrice.h"
#include "random.h"
#include "evalResult.h"
#include "paramType.h"
#include "linearExtension.h"
#include "linearExtentionTreeOfIdeals.h"
#include "linearExtentionLatticeOfIdeals.h"

class FunctionLinearExtension;

//***********************************************//
//***********************************************//
//***********************************************//

class POSet
{
public:
    using RELATEDSTORE = std::set<std::uint64_t>;
    using DATASTORE = std::map<std::uint64_t, std::shared_ptr<RELATEDSTORE>>;
    using STARTINGSTORE = std::map<std::string, std::uint64_t>;
private:
    static std::uint64_t ELEMENT_CODE;
    DATASTORE elementi;
    std::shared_ptr<std::map<std::uint64_t, std::string>> starting_element_presentation;
    STARTINGSTORE starting_element_position;
    std::shared_ptr<Random> rnd;
    std::shared_ptr<TreeOfIdeals>  treeOfIdeals;
    std::shared_ptr<LatticeOfIdeals>  latticeOfIdeals;
public:
    // costruttori
    POSet(std::vector<std::string>&, std::vector<std::pair<std::string, std::string>>&);
    //**********
    ~POSet();
    std::string to_string() const;
    size_t size() const;
    std::shared_ptr<Rcpp::LogicalMatrix> IncidenceMatrix() const;
    std::shared_ptr<Rcpp::LogicalMatrix> CoverMatrix() const;
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> coveredBy(std::shared_ptr<std::vector<std::uint64_t>>) const;
    std::shared_ptr<std::set<std::uint64_t>> coveredBy(std::uint64_t) const;
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> imPred() const;
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> imPred(std::shared_ptr<std::vector<std::uint64_t>>) const;
    std::shared_ptr<std::set<std::uint64_t>> imPred(std::uint64_t) const;
    std::shared_ptr<std::map<std::uint64_t, std::string>> Elements();
    std::shared_ptr<Rcpp::StringVector> ElementsKeys() const;
    std::shared_ptr<LinearExtension> FirstLE() const;
    std::uint64_t StartPosition(std::string);
    std::shared_ptr<std::map<std::uint64_t, std::uint64_t>> UpSetSizes();
    std::shared_ptr<DATASTORE> DownSet() const;
    std::shared_ptr<std::set<std::uint64_t>> DownSet(std::set<std::uint64_t>&);
    std::shared_ptr<std::set<std::uint64_t>> UpSet(std::set<std::uint64_t>&);
    bool GreaterThan(std::uint64_t, std::uint64_t);
    void evaluation(std::shared_ptr<std::map<std::string, ParamType>>, std::shared_ptr<EvalResult>, std::shared_ptr<std::uint64_t>, std::shared_ptr<std::uint64_t>, std::shared_ptr<bool>, std::shared_ptr<DisplayMessage>);
    std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> incomparabilities();
    std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> comparabilities();
    std::shared_ptr<LatticeOfIdeals>  getLatticeOfIdeals();
    std::shared_ptr<TreeOfIdeals>  getTreeOfIdeals();

    std::shared_ptr<POSet> clone();
    std::shared_ptr<std::vector<std::uint64_t>> maximals();
    void remove(std::shared_ptr<std::vector<std::string>>);


    std::uint64_t GetElementIdx(std::string);
    std::string GetElement(std::uint64_t);
    
    static void UpdateForFirst(std::shared_ptr<POSet::DATASTORE> downSets, std::set<std::uint64_t>& setOne, std::uint64_t min_el);
private:
    void remove(std::shared_ptr<std::vector<std::uint64_t>>);
    bool AddToDatastore(std::uint64_t);
    std::uint64_t TransitiveClosure(std::uint64_t, std::uint64_t);
    bool CheckAntisymmetric(std::uint64_t, std::uint64_t);
    void AverageUpdate(std::shared_ptr<Matrice>, std::shared_ptr<FunctionLinearExtension>, std::uint64_t);
};




#endif /* poset_hpp */
