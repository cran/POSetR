
#ifndef coveringGraph_hpp
#define coveringGraph_hpp

#include <iostream>
#include <map>
#include <functional>
#include <algorithm>
#include <vector>
#include <list>
#include <tuple>
#include <iterator>
#include <cstdint>

#include "linearExtentionTreeOfIdeals.h"

class LatticeOfIdeals {
private:
    std::map<std::uint64_t, std::shared_ptr<std::vector<std::uint64_t>>> impred;
    std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>, std::greater<std::uint64_t>> sort;
    std::shared_ptr<TreeOfIdeals> treeOfIdeals;
public:
    LatticeOfIdeals (std::shared_ptr<TreeOfIdeals> treeOfIdeals, std::uint64_t root);
    std::shared_ptr<std::vector<std::uint64_t>> getFromPath(std::shared_ptr<std::vector<std::uint64_t>>, std::shared_ptr<std::vector<bool>>);
    
    void ComputeSort();
    
    void toFile(std::ofstream& fp) {
        for (auto a : this->impred) {
            for (auto b : *(a.second)) {
                std::vector<std::uint64_t> diff;
                std::shared_ptr<std::set<std::uint64_t>> ideal1 = treeOfIdeals->getIdeal(a.first);
                std::shared_ptr<std::set<std::uint64_t>> ideal2 = treeOfIdeals->getIdeal(b);
                std::set_difference(ideal1->begin(), ideal1->end(), ideal2->begin(), ideal2->end(), std::inserter(diff, diff.begin()));
                
                std::uint64_t label = diff.front();
                auto label_converted = this->treeOfIdeals->getLEForConversion()->getVal(label - 1);
                fp << std::to_string(b) << "," << std::to_string(a.first) << "," << std::to_string(label_converted) << std::endl;
            }
        }
    }
    
    std::shared_ptr<std::list<std::tuple<std::uint64_t, std::uint64_t, std::uint64_t>>> toList() {
        std::shared_ptr<std::list<std::tuple<std::uint64_t, std::uint64_t, std::uint64_t>>> result;
        result = std::make_shared<std::list<std::tuple<std::uint64_t, std::uint64_t, std::uint64_t>>>();
        for (auto a : this->impred) {
            for (auto b : *(a.second)) {
                std::vector<std::uint64_t> diff;
                std::shared_ptr<std::set<std::uint64_t>> ideal1 = treeOfIdeals->getIdeal(a.first);
                std::shared_ptr<std::set<std::uint64_t>> ideal2 = treeOfIdeals->getIdeal(b);
                std::set_difference(ideal1->begin(), ideal1->end(), ideal2->begin(), ideal2->end(), std::inserter(diff, diff.begin()));
                
                std::uint64_t label = diff.front();
                auto label_converted = this->treeOfIdeals->getLEForConversion()->getVal(label - 1);
                result->push_back(std::make_tuple(b, a.first, label_converted));
            }
        }
        return result;
    }
    
};






#endif 
