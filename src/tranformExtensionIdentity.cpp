#include "tranformExtension.h"
#include <cstdint>


// ***********************************************
// ***********************************************
// ***********************************************

TEItentity::TEItentity(std::shared_ptr<std::map<std::string, ParamType>> p) : TranformExtension(p) {
    this->labels_pos = std::make_shared<std::map<std::string, std::uint64_t>>();
    for (auto e : (*this->pos_labels)) {
        (*this->labels_pos)[e.second] = e.first;
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<LinearExtension> TEItentity::operator()(std::shared_ptr<LinearExtension> x) {
    return x;
}

// ***********************************************
// ***********************************************
// ***********************************************

