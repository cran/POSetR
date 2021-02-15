//
//  linearExtensionByTreeOfIdeal.cpp
//  POSet
//
//  Created by Alessandro Avellone on 01/10/2020.
//  Copyright © 2020 Alessandro Avellone. All rights reserved.
//

#include "linearExtensionGenerator.h"
#include "linearExtentionTreeOfIdeals.h"
#include "linearExtentionLatticeOfIdeals.h"


// ***********************************************
// ***********************************************
// ***********************************************

LEGByTreeOfIdeals::LEGByTreeOfIdeals(std::shared_ptr<std::map<std::string, ParamType>> p) : LinearExtensionGenerator(p) {
    if (auto* p = std::get_if<std::shared_ptr<POSet>>(&(*this->parms)["POSet"])) {
        this->poset = *p;
    } else {
        std::string err_str = "LEGByTreeOfIdeals error: POSet";
        throw_line(err_str);
    }
    this->started = false;
    
    this->latticeOfIdeals = this->poset->getLatticeOfIdeals();
    this->latticeOfIdealsCrossing = std::make_shared<std::vector<std::uint64_t>>(this->poset->size(), 0);
    this->moreCrossing = std::make_shared<std::vector<bool>>(this->poset->size());
    this->currentLinearExtension = std::make_shared<LinearExtension>(this->poset->size());
}

// ***********************************************
// ***********************************************
// ***********************************************

std::string LEGByTreeOfIdeals::to_string() const {
    std::string base_string = LinearExtensionGenerator::to_string();
    std::string risultato = "";
    risultato += "ByTreeOfIdeal:";
    if (base_string != "")
        risultato += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
    risultato += "\n\tNumber of iterations: " + std::to_string(this->max_number_le);
    return risultato;
}

// ***********************************************
// ***********************************************
// ***********************************************

void LEGByTreeOfIdeals::start() {
    std::shared_ptr<std::vector<std::uint64_t>> path = this->latticeOfIdeals->getFromPath(this->latticeOfIdealsCrossing, this->moreCrossing);
    
    for (size_t k = 0; k < this->currentLinearExtension->size(); ++k) {
        auto val = path->at(k);
        this->currentLinearExtension->set(k, val);
    }
    
    this->started = true;
    this->current_number_le = 1;
}

// ***********************************************
// ***********************************************
// ***********************************************

void LEGByTreeOfIdeals::next() {
    if (this->started == false) {
        std::string err_str = "LEGByTreeOfIdeals error: not started yet!";
        throw std::invalid_argument(err_str);
    }
    
    auto result = std::find(this->moreCrossing->rbegin(), this->moreCrossing->rend(), true);
    if (result == this->moreCrossing->rend()) {
        std::string err_str = "LEGByTreeOfIdeals error: max number of generation reached!";
        throw std::invalid_argument(err_str);
    }
    ++this->current_number_le;

    auto pos = std::distance(result, this->moreCrossing->rend()) - 1;
    ++this->latticeOfIdealsCrossing->at(pos);
    std::fill (this->latticeOfIdealsCrossing->begin() + pos + 1, this->latticeOfIdealsCrossing->end(), 0);
    
    std::shared_ptr<std::vector<std::uint64_t>> path = this->latticeOfIdeals->getFromPath(this->latticeOfIdealsCrossing, this->moreCrossing);
    
    for (size_t k = 0; k < this->currentLinearExtension->size(); ++k) {
        auto val = path->at(k);
        this->currentLinearExtension->set(k, val);
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

bool LEGByTreeOfIdeals::hasNext() {
    auto result = std::find(this->moreCrossing->rbegin(), this->moreCrossing->rend(), true);
    return result != this->moreCrossing->rend();
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> LEGByTreeOfIdeals::get() {
    if (this->started == false)
    {
        std::string err_str = "LEGByTreeOfIdeals error: not started yet!";
        throw std::invalid_argument(err_str);
    }
    
    std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> result = std::make_shared<std::vector<std::shared_ptr<LinearExtension>>>(1);
    
    result->at(0) = this->currentLinearExtension;
    this->to_file();
    return result;
}


// ***********************************************
// ***********************************************
// ***********************************************
