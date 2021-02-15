#ifndef treeOfIdeals_h
#define treeOfIdeals_h


#include <vector>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include "linearExtension.h"


class TreeOfIdeals {
private:
    /// parent associate each node k of the tree to his parent.
    std::map<std::uint64_t, std::uint64_t> parent;
    /// labels associate each node k of the tree to the label of the edge between k and his parent.
    /// The label of node k is the element of the linear extention obtainde by the set difference from the ideal of Parent(k) and the ideal of k.
    std::map<std::uint64_t, std::uint64_t> labels;
    std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>> children;
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> ImPred;
    std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>> ideals;
    std::uint64_t ROOT;
    std::shared_ptr<LinearExtension> leForConversion;
public:
    /**
    * Tree build the tree of ideal.
    *
    * @param ImPred Container of the immediate predecessors of each element of the poset.
    *           Keys and values are coded using the linear extention, i.e. each element E of the poset is coded by is position (1...n) in the linear extention.
    *
    */
    TreeOfIdeals(std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> ImPred, std::shared_ptr<LinearExtension> le);
    
    std::shared_ptr<LinearExtension> getLEForConversion() const {
        return this->leForConversion;
    }
    
    std::uint64_t Left(std::uint64_t n, std::shared_ptr<std::set<std::uint64_t>> ideal);
    void Right(std::uint64_t n, std::uint64_t r, std::uint64_t root);
    
    std::uint64_t getRoot() {
        return this->ROOT;
    }
    
    std::uint64_t newNode(std::shared_ptr<std::set<std::uint64_t>> ideal) {
        std::uint64_t new_id = this->children.size();
        this->children[new_id] = std::make_shared<std::set<std::uint64_t>>();
        this->ideals[new_id] = std::make_shared<std::set<std::uint64_t>>(ideal->begin(), ideal->end());
        return new_id;
    }
    
    void addChild(std::uint64_t parent, std::uint64_t child, std::uint64_t label) {
        this->parent[child] = parent;
        this->labels[child] = label;
        this->children[parent]->insert(child);
        this->ideals[child]->erase(label);
    }
    
    std::uint64_t getLabel(std::uint64_t n)  {
        if (this->labels.find(n) == this->labels.end())
            throw 1;
        return this->labels[n];
    }
    
    std::shared_ptr<std::set<std::uint64_t>> getIdeal(std::uint64_t n)  {
        if (this->ideals.find(n) == this->ideals.end())
            throw 2;
        return this->ideals[n];
    }
    
    
    std::map<std::uint64_t, std::uint64_t>& getLabels()  {
        return this->labels;
    }
    
    std::uint64_t getParent(std::uint64_t n) {
        return this->parent[n];
    }
    
    std::uint64_t labelSize() {
        return this->labels.size();
    }
    
    std::shared_ptr<std::set<std::uint64_t>> getChildrenOf(std::uint64_t n) {
        return this->children[n];
    }
    
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::map<std::uint64_t, std::uint64_t, std::greater<std::uint64_t>>>>> ChildrenSortedLabel() {
        std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::map<std::uint64_t, std::uint64_t, std::greater<std::uint64_t>>>>> result = std::make_shared<std::map<std::uint64_t, std::shared_ptr<std::map<std::uint64_t, std::uint64_t, std::greater<std::uint64_t>>>>>();
        for (auto key_val : this->children) {
            auto childrenOf = key_val.second;
            std::shared_ptr<std::map<std::uint64_t, std::uint64_t, std::greater<std::uint64_t>>> childrenOfResult = std::make_shared<std::map<std::uint64_t, std::uint64_t, std::greater<std::uint64_t>>>();
            for (auto val : *childrenOf) {
                auto label = getLabel(val);
                (*childrenOfResult)[label] = val;
            }
            (*result)[key_val.first] = childrenOfResult;
        }
        return result;
    }
    
    void toFile(std::ofstream& fp, std::uint64_t n) {
        std::shared_ptr<std::set<std::uint64_t>> children = this->children[n];
        for (auto s : *children) {
            std::uint64_t label = getLabel(s);
            auto label_converted = this->leForConversion->getVal(label - 1);
            fp << std::to_string(n) << "," << std::to_string(s) << "," << std::to_string(label_converted) << std::endl;
            toFile(fp, s);
        }
    }
    
    
};



#endif /* treeOfIdeals_h */
