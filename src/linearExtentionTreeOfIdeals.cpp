
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include "linearExtentionTreeOfIdeals.h"

/**
* Left create the leftmost subtree containning all the ideals that not include n.
*
* @param n position of the elementi in the linear extention.
* @return id of the tree.
*/
std::uint64_t TreeOfIdeals::Left(std::uint64_t n, std::shared_ptr<std::set<std::uint64_t>> ideal) {
    std::uint64_t root = this->newNode(ideal);
    if (n == 0)
        return root;

    std::shared_ptr<std::set<std::uint64_t>> sub_ideal = std::make_shared<std::set<std::uint64_t>>(ideal->begin(), ideal->end());
    sub_ideal->erase(n);
    std::uint64_t r = Left(n - 1, sub_ideal);
    Right(n, r, root);
    this->addChild(root, r, n);
    
    return root;
}

/**
* Right create all ideals containing the element whos position in linear extention is n.
*
* The ideals are created by traversing the tree rooted by r. The linear extention position strat from 1
*
* @param n position of the elementi in the linear extention.
* @param r id of the subtree.
* @param root id of the tree to be modified.
*/
void TreeOfIdeals::Right(std::uint64_t n, std::uint64_t r, std::uint64_t root) {
    std::shared_ptr<std::set<std::uint64_t>> children = this->getChildrenOf(r);
    for (auto s : *children) {
        std::uint64_t label_s = this->getLabel(s);
        if (ImPred->at(n)->find(label_s) != ImPred->at(n)->end()) {
            continue;
        }
        std::shared_ptr<std::set<std::uint64_t>> ideal = this->ideals[root];
        std::uint64_t t = this->newNode(ideal);
        this->addChild(root, t, label_s);
        Right(n, s, t);
    }
}

TreeOfIdeals::TreeOfIdeals(std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> imPred, std::shared_ptr<LinearExtension> le) :ImPred(imPred), leForConversion(le) {
    std::uint64_t n = this->ImPred->size();
    std::vector<std::uint64_t> idxs (n);
    std::iota(idxs.begin(), idxs.end(), 1);
    std::shared_ptr<std::set<std::uint64_t>> ideal = std::make_shared<std::set<std::uint64_t>>(idxs.begin(), idxs.end());

    this->ROOT = Left(n, ideal);
}

