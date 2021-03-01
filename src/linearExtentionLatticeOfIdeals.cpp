#include <functional>
#include <map>
#include "linearExtentionLatticeOfIdeals.h"

// ***********************************************
// ***********************************************
// ***********************************************

LatticeOfIdeals::LatticeOfIdeals (std::shared_ptr<TreeOfIdeals> tr, std::uint64_t root) : treeOfIdeals(tr) {
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::map<std::uint64_t, std::uint64_t, std::greater<std::uint64_t>>>>> childrenSortedLabel = this->treeOfIdeals->ChildrenSortedLabel();

    this->impred[root] = std::make_shared<std::vector<std::uint64_t>>();
    
    for(auto it = childrenSortedLabel->at(root)->begin(); it != childrenSortedLabel->at(root)->end(); ++it) {
      this->impred[root]->push_back(it->second);
    }
    ComputeSort();
    for (auto key_val : this->sort) {
        auto sort_k = key_val.second;
        for (auto i:*(sort_k)) {
            this->impred[i] = std::make_shared<std::vector<std::uint64_t>>();
            std::uint64_t pi = this->treeOfIdeals->getParent(i);
            
            std::uint64_t h = 0;
            std::uint64_t j_primo = this->impred[pi]->at(h);
            while (j_primo != i) {
                std::uint64_t j = childrenSortedLabel->at(j_primo)->begin()->second;
                this->impred[i]->push_back(j);
                j_primo = this->impred[pi]->at(++h);
            }
            for(auto it = childrenSortedLabel->at(i)->begin(); it != childrenSortedLabel->at(i)->end(); ++it) {
                this->impred[i]->push_back(it->second);
            }
        }
        for (auto i: *(sort_k)) {
          std::uint64_t pi = this->treeOfIdeals->getParent(i);
          auto at_pi = childrenSortedLabel->at(pi);
          for (auto it = at_pi->begin(); it != at_pi->end(); ++it) {
            if (it->second == i) {
              at_pi->erase(it);
              break;
            }
          }
        }
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

void LatticeOfIdeals::ComputeSort () {
    for (auto key_value : this->treeOfIdeals->getLabels()) {
        if (this->sort.find(key_value.second) == this->sort.end()) {
            this->sort[key_value.second] = std::make_shared<std::set<std::uint64_t>>();
        }
        this->sort[key_value.second]->insert(key_value.first);
    }
}

// ***********************************************
// ***********************************************
// ***********************************************


std::shared_ptr<std::vector<std::uint64_t>> LatticeOfIdeals::getFromPath(std::shared_ptr<std::vector<std::uint64_t>> path, std::shared_ptr<std::vector<bool>> more) {
    
    std::shared_ptr<std::vector<std::uint64_t>> result = std::make_shared<std::vector<std::uint64_t>>(path->size());
    
    std::uint64_t val_start = 0;
    for (std::uint64_t k = 0; k < path->size(); ++k) {
        auto set_start = this->impred.at(val_start);
        std::uint64_t val_end = set_start->at(path->at(k));
        if (path->at(k) + 1 < set_start->size()) {
            more->at(k) = true;
        } else {
            more->at(k) = false;
        }
        
        std::vector<std::uint64_t> diff;
        std::shared_ptr<std::set<std::uint64_t>> ideal1 = treeOfIdeals->getIdeal(val_start);
        std::shared_ptr<std::set<std::uint64_t>> ideal2 = treeOfIdeals->getIdeal(val_end);
        std::set_difference(ideal1->begin(), ideal1->end(), ideal2->begin(), ideal2->end(), std::inserter(diff, diff.begin()));
        std::uint64_t label = diff.front();
        auto label_converted = this->treeOfIdeals->getLEForConversion()->getVal(label - 1);
        result->at(path->size() - k - 1) = label_converted;
        val_start = val_end;
    }
    return result;
}
