#include "linearExtensionGenerator.h"

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t LEGBubleyDyer::PRECISION = 10;

// ***********************************************
// ***********************************************
// ***********************************************

LEGBubleyDyer::LEGBubleyDyer(std::shared_ptr<std::map<std::string, ParamType>> p) : LinearExtensionGenerator(p)
{
    this->max_number_le = 0;
    if ((*this->parms).find("BubleyDyerMaxIterations") != (*this->parms).end()) {
        if (auto* p = std::get_if<std::uint64_t>(&(*this->parms)["BubleyDyerMaxIterations"])) {
            this->max_number_le = *p;
        } else {
            std::string err_str = "LEGBubleyDyer error: BubleyDyerMaxIterations";
            throw_line(err_str);
        }
    }
    this->precision = LEGBubleyDyer::PRECISION;
    if ((*this->parms).find("BubleyDyerPrecision") != (*this->parms).end()) {
        if (auto* p = std::get_if<std::uint64_t>(&(*this->parms)["BubleyDyerPrecision"])) {
            this->precision = *p;
        } else {
            std::string err_str = "LEGBubleyDyer error: BubleyDyerPrecision";
            throw_line(err_str);
        }
    }
    
    if (auto* p = std::get_if<std::shared_ptr<POSet>>(&(*this->parms)["BubleyDyerPOSet"])) {
        this->poset = *p;
    } else {
        std::string err_str = "LEGBubleyDyer error: BubleyDyerPOSet";
        throw_line(err_str);
    }
    
    if (this->precision == 0.0)
    {
        this->precision = LEGBubleyDyer::PRECISION;
    }
    if (this->max_number_le == 0)
    {
        this->max_number_le = this->numberOfIterations();
    }
    this->currentLinearExtension = std::make_shared<LinearExtension>(this->poset->size());
    
    
    std::shared_ptr<std::uint64_t> seed = nullptr;
    if ((*this->parms).find("BubleyDyerSeed") != (*this->parms).end()) {
        if (auto* p = std::get_if<std::shared_ptr<std::uint64_t>>(&(*this->parms)["BubleyDyerSeed"])) {
            seed = *p;
        } else {
            std::string err_str = "LEGBubleyDyer error: BubleyDyerSeed";
            throw_line(err_str);
        }
    }
    
    this->rnd = std::make_shared<RandomUni>(seed);
    
    this->started = false;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::string LEGBubleyDyer::to_string() const {
    std::string base_string = LinearExtensionGenerator::to_string();
    std::string risultato = "";
    risultato += "BubleyDyer:";
    if (base_string != "")
        risultato += "\n\t" + FindAndReplaceAll(base_string, "\n", "\n\t");
    risultato += "\n\tNumber of iterations: " + std::to_string(this->max_number_le);
    
    std::stringstream  streamObj;
    streamObj << this->precision;
    risultato += "\n\tPrecision: " + streamObj.str();
    return risultato;
}

// ***********************************************
// ***********************************************
// ***********************************************

void LEGBubleyDyer::start()
{
    std::shared_ptr<POSet::DATASTORE> downSets = this->poset->DownSet();
    std::set<std::uint64_t> setOne;
    
    for (auto el : (*downSets))
    {
        if (el.second->size() == 0)
            setOne.insert(el.first);
    }
    
    for (size_t k = 0; k < this->currentLinearExtension->size(); ++k)
    {
        std::uint64_t min_el = this->getSetOneElement(setOne);
        POSet::UpdateForFirst(downSets, setOne, min_el);
        this->currentLinearExtension->set(k, min_el);
    }
    
    this->started = true;
    this->current_number_le = 1;
    
    this->toUpdate = false;
    this->isSwitched = false;
    this->positionToUpdate = 0;
    
    return;
}

// ***********************************************
// ***********************************************
// ***********************************************

void LEGBubleyDyer::next()
{
    if (this->started == false) {
        std::string err_str = "LEGBubleyDyer error: not started yet!";
        throw std::invalid_argument(err_str);
    }
    
    if (this->current_number_le > this->max_number_le) {
        std::string err_str = "LEGBubleyDyer error: max numebr of generation reached!";
        throw std::invalid_argument(err_str);
    }
    
    this->toUpdate = this->rnd->RndNextInt(0, 1);
    this->isSwitched = false;

    if (this->toUpdate != 0) {
        this->positionToUpdate = this->rnd->RndNextInt(0, this->currentLinearExtension->size() - 2);
        std::uint64_t e1 = this->currentLinearExtension->getVal(this->positionToUpdate);
        std::uint64_t e2 = this->currentLinearExtension->getVal(this->positionToUpdate + 1);
        if (!this->poset->GreaterThan(e2, e1)) {
            this->currentLinearExtension->set(this->positionToUpdate, e2);
            this->currentLinearExtension->set(this->positionToUpdate + 1, e1);
            this->isSwitched = true;
        }
    }
    ++this->current_number_le;
    return;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> LEGBubleyDyer::get() {
    if (this->started == false)
    {
        std::string err_str = "LEGBubleyDyer error: not started yet!";
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

std::uint64_t LEGBubleyDyer::numberOfIterations()
{
    std::uint64_t risultato;
    size_t nelementi = this->poset->size();
    double add = std::pow(nelementi, 4) * std::pow(std::log(nelementi), 2);
    double sub = std::pow(nelementi, 3) * std::log(nelementi) * this->precision;
    risultato = ((std::uint64_t)(add + sub));
    
    return risultato;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t LEGBubleyDyer::getSetOneElement(std::set<std::uint64_t>& setOne)
{
    std::uint64_t n = setOne.size();
    std::uint64_t p = this->rnd->RndNextInt(0, n - 1);
    
    std::set<std::uint64_t>::iterator it = setOne.begin();
    std::advance(it, p);
    std::uint64_t result = *it;
    return result;
}


// ***********************************************
// ***********************************************
// ***********************************************

void LEGBubleyDyer::to_file() {
    if (this->file_le != nullptr && this->file_le->is_open())
    {
        std::string str_le = "";
        bool first = true;
        for (std::uint64_t k = 0; k < this->currentLinearExtension->size(); ++k) {
            std::string nome_etichetta = this->poset->GetElement(this->currentLinearExtension->getVal(k));
            if (first) {
                str_le = "" + nome_etichetta;
                first = false;
            }
            else {
                str_le += DELIMETER + nome_etichetta;
            }
        }
        (*this->file_le)  << str_le;
        (*this->file_le)  << DELIMETER + std::to_string(this->toUpdate);
        (*this->file_le)  << DELIMETER + std::to_string(this->positionToUpdate);
        (*this->file_le)  << DELIMETER + std::to_string(this->isSwitched);

        (*this->file_le)  << std::endl;
    }
}
// ***********************************************
// ***********************************************
// ***********************************************


