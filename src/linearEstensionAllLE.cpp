#include "linearExtensionGenerator.h"
#include <cstdint>


// ***********************************************
// ***********************************************
// ***********************************************


LEGAllLE::LEGAllLE(std::shared_ptr<std::map<std::string, ParamType>> p) : LinearExtensionGenerator(p)
{
    if (auto* p = std::get_if<std::shared_ptr<POSet>>(&(*this->parms)["POSet"])) {
        this->poset = *p;
    } else {
        std::string err_str = "LEGAllLE error: POSet";
        throw_line(err_str);
    }
    this->started = false;
    this->currentLinearExtension = std::make_shared<LinearExtension>(this->poset->size());
}

// ***********************************************
// ***********************************************
// ***********************************************

std::string LEGAllLE::to_string() const {
    std::string base_string = LinearExtensionGenerator::to_string();
    std::string risultato = "";
    risultato += "AllLE:";
    if (base_string != "")
        risultato += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
    risultato += "\n\tNumber of iterations: " + std::to_string(this->max_number_le);
    
    return risultato;
    
}

// ***********************************************
// ***********************************************
// ***********************************************

void LEGAllLE::start() {
    std::shared_ptr<POSet::DATASTORE> downSets = this->poset->DownSet();
    std::set<std::uint64_t> setOne;
    
    for (auto el : (*downSets))
    {
        if (el.second->size() == 0)
            setOne.insert(el.first);
    }
    
    for (size_t k = 0; k < this->currentLinearExtension->size(); ++k)
    {
        std::uint64_t min_el = *setOne.begin();
        POSet::UpdateForFirst(downSets, setOne, min_el);
        this->currentLinearExtension->set(k, min_el);
    }
    
    this->currentStatus.assign(this->currentLinearExtension->size(), false);
    this->extension_stack_size = 0;
    this->current_number_le = 1;
    this->more_extensions = true;
    this->started = true;
}

// ***********************************************
// ***********************************************
// ***********************************************

void LEGAllLE::next() {
    if (this->started == false)
    {
        std::string err_str = "LEGAllLE error: not started yet!";
        throw std::invalid_argument(err_str);
    }
    
    if (!this->more_extensions)
    {
        std::string err_str = "LEGAllLE error: max numeber of generation reached!";
        throw std::invalid_argument(err_str);
    }
    
    ++this->current_number_le;
    
    std::uint64_t position_to_analyze = this->currentStatus.size() - 2;
    bool switch_done = false;
    while (true) {
        if (this->currentStatus.at(position_to_analyze) == false) {
            for (std::uint64_t k = position_to_analyze + 1; k  < this->currentStatus.size(); ++k) {
                if (IsPossibleToSwitch(position_to_analyze, k)) {
                    std::shared_ptr<EST_TYPE> estToPush = std::make_shared<EST_TYPE>(this->currentStatus.size());
                    for (std::uint64_t j = 0; j < this->currentLinearExtension->size(); ++j) {
                        estToPush->at(j) = this->currentLinearExtension->getVal(j);
                    }
                    std::uint64_t temp = estToPush->at(k);
                    for (std::uint64_t h = k; h  > position_to_analyze; --h) {
                        estToPush->at(h) = estToPush->at(h - 1);
                    }
                    estToPush->at(position_to_analyze) = temp;
                    
                    std::shared_ptr<STATUS_TYPE> statusToPush = std::make_shared<STATUS_TYPE>(this->currentStatus.size());
                    std::copy(this->currentStatus.begin(), this->currentStatus.end(), statusToPush->begin());
                    statusToPush->at(position_to_analyze) = true;
                    STATUS_TYPE::iterator it = statusToPush->begin();
                    advance(it, position_to_analyze + 1);
                    std::fill (it, statusToPush->end(), false);
                    this->extension_stack.push_back(estToPush);
                    this->status_stack.push_back(statusToPush);
                    
                    switch_done = true;
                }
            }
        } else {
            if (!this->status_stack.empty()) {
                std::shared_ptr<STATUS_TYPE> top_status = this->status_stack.back();
                if (top_status->at(position_to_analyze) == true) {
                    break;
                }
            }
        }
        if (switch_done) {
            break;
        }
        if (position_to_analyze == 0)
            break;
        --position_to_analyze;
    }

    if (switch_done) {
        std::shared_ptr<EST_TYPE> top_est = this->extension_stack.back();
        for (std::uint64_t j = 0; j < this->currentLinearExtension->size(); ++j) {
            this->currentLinearExtension->set(j, top_est->at(j));
        }
        this->extension_stack.pop_back();
        std::shared_ptr<STATUS_TYPE> top_stato = this->status_stack.back();
        std::copy(top_stato->begin(), top_stato->end(), this->currentStatus.begin());
        this->status_stack.pop_back();
        this->extension_stack_size = this->status_stack.size();
        return;
    } else {
        if (!this->extension_stack.empty()) {
            std::shared_ptr<EST_TYPE> top_est = this->extension_stack.back();
            for (std::uint64_t j = 0; j < this->currentLinearExtension->size(); ++j) {
                this->currentLinearExtension->set(j, top_est->at(j));
            }
            this->extension_stack.pop_back();
            std::shared_ptr<STATUS_TYPE> top_stato = this->status_stack.back();
            std::copy(top_stato->begin(), top_stato->end(), this->currentStatus.begin());
            this->status_stack.pop_back();
            this->extension_stack_size = this->status_stack.size();
            return;
        }
    }
    this->extension_stack_size = this->status_stack.size();
}

// ***********************************************
// ***********************************************
// ***********************************************

bool LEGAllLE::IsPossibleToSwitch(std::uint64_t p1, std::uint64_t p2) {
    std::uint64_t b = this->currentLinearExtension->getVal(p2);

    for (std::uint64_t k = p1; k < p2; ++k) {
        std::uint64_t a = this->currentLinearExtension->getVal(k);
        if (this->poset->GreaterThan(b, a)) {
            return false;
        }
    }
    return true;
}

// ***********************************************
// ***********************************************
// ***********************************************

bool LEGAllLE::hasNext() {
    std::uint64_t position_to_analyze = this->currentStatus.size() - 2;
    while (true) {
        if (this->currentStatus.at(position_to_analyze) == false) {
            for (std::uint64_t k = position_to_analyze + 1; k  < this->currentStatus.size(); ++k) {
                std::uint64_t e1 = this->currentLinearExtension->getVal(position_to_analyze);
                std::uint64_t e2 = this->currentLinearExtension->getVal(k);
                if (!this->poset->GreaterThan(e2, e1)) {
                    return true;
                }
            }
        } else {
            if (!this->status_stack.empty()) {
                std::shared_ptr<STATUS_TYPE> top_status = this->status_stack.back();
                if (top_status->at(position_to_analyze) == true) {
                    break;
                }
            }
        }
        if (position_to_analyze == 0)
            break;
        --position_to_analyze;
    }
    if (!this->extension_stack.empty()) {
       return true;
    }
    this->more_extensions = false;
    return false;
}


// ***********************************************
// ***********************************************
// ***********************************************



std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> LEGAllLE::get() {
    if (this->started == false) {
        std::string err_str = "LEGAllLE error: not started yet!";
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
