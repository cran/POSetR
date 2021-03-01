#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <set>
#include <stack>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>

#include "matrice.h"
#include "linearExtension.h"
#include "functionLinearExtension.h"
#include "poset.h"
#include "linearExtensionGenerator.h"
#include "tranformExtension.h"


// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t POSet::ELEMENT_CODE = 1;
const char DELIMETER = ';';

// ***********************************************
// ***********************************************
// ***********************************************

POSet::POSet(std::vector<std::string>& elements,
             std::vector<std::pair<std::string, std::string>>& comparabilities) {
    
    this->rnd = std::make_shared<RandomUni>();
    this->treeOfIdeals = nullptr;
    this->latticeOfIdeals = nullptr;
    
    this->starting_element_presentation = std::make_shared<std::map<std::uint64_t, std::string>>();
    // elements
    POSet::ELEMENT_CODE = 0;
    for (auto elemento:elements)
    {
        this->starting_element_position[elemento] = POSet::ELEMENT_CODE;
        (*this->starting_element_presentation)[POSet::ELEMENT_CODE] = elemento;
        bool r = this->AddToDatastore(POSet::ELEMENT_CODE);
        if (r != true)
        {
            std::string err_str = "POSet error: " + elemento + " duplicated ";
            throw std::invalid_argument(err_str);
        }
        ++POSet::ELEMENT_CODE;
    }
    // comparabilities
    for (auto comp:comparabilities)
    {
        std::uint64_t lower = this->GetElementIdx(comp.first);
        std::uint64_t upper = this->GetElementIdx(comp.second);
        if (lower == upper)
            continue;
        
        DATASTORE::iterator iter_lower = this->elementi.find(lower);
        iter_lower->second->insert(upper);
        if (!CheckAntisymmetric(lower, upper))
        {
            std::string lower_str = comp.first;
            std::string second_str = comp.second;
            std::string err_str = "The binary relation is not antisymmetric due to the comparability " + lower_str + " <= " + second_str;
            throw std::invalid_argument(err_str);
        }
        TransitiveClosure(lower, upper);
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

POSet::~POSet() {
    
}

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t POSet::GetElementIdx(std::string element) {
    STARTINGSTORE::const_iterator starting_iterator = this->starting_element_position.find(element);
    if (starting_iterator == this->starting_element_position.end()) {
        std::string err_str = "Element " + element + " not found!";
        throw std::invalid_argument(err_str);
    }
    
    std::uint64_t element_idx = starting_iterator->second;
    return element_idx;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::string POSet::GetElement(std::uint64_t idx) {
    return this->starting_element_presentation->at(idx);
}


// ***********************************************
// ***********************************************
// ***********************************************

size_t POSet::size() const
{
    return this->starting_element_presentation->size();
}

// ***********************************************
// ***********************************************
// ***********************************************

std::string POSet::to_string() const
{
    std::string result = "";
    for (auto element:(*this->starting_element_presentation))
    {
        result += DELIMETER + element.second;
    }
    result += "\n";
    for (auto out_element:(*this->starting_element_presentation))
    {
        result += out_element.second;
        DATASTORE::const_iterator out_iter = this->elementi.find(out_element.first);
        if (out_iter == this->elementi.end())
        {
            std::string err_str = "Element " + out_element.second + " not found!";
            throw std::invalid_argument(err_str);
        }
        for (auto in_element:(*this->starting_element_presentation))
        {
            if (out_element.first == in_element.first)
            {
                result += ";T";
                continue;
            }
            
            if ((out_iter->second->find(in_element.first) != out_iter->second->end()))
                result += ";T";
            else
                result += DELIMETER;
        }
        result += "\n";
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

bool POSet::AddToDatastore(std::uint64_t val)
{
    bool result = false;
    DATASTORE::const_iterator val_iter = this->elementi.find(val);
    if (val_iter == this->elementi.end())
    {
        this->elementi.insert(std::make_pair(val, std::make_shared<POSet::RELATEDSTORE>()));
        result = true;
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

bool POSet::CheckAntisymmetric(std::uint64_t a, std::uint64_t b)
{
    DATASTORE::iterator in_set_a = this->elementi.find(a);
    DATASTORE::iterator in_set_b = this->elementi.find(b);

    if (in_set_a == this->elementi.end())
    {
        std::string el_str = this->starting_element_presentation->at(a);
        std::string err_str = "Element " + el_str + " not found!";
        throw std::invalid_argument(err_str);
    }
    if (in_set_b == this->elementi.end())
    {
        std::string el_str = this->starting_element_presentation->at(b);
        std::string err_str = "Element " + el_str + " not found!";
        throw std::invalid_argument(err_str);
    }
    
    std::shared_ptr<RELATEDSTORE> successori_a = in_set_a->second;
    std::shared_ptr<RELATEDSTORE> successori_b = in_set_b->second;
    
    bool b_in_successori_a = (successori_a->find(b) != successori_a->end());
    bool a_in_successori_b = (successori_b->find(a) != successori_b->end());
    
    return !(b_in_successori_a && a_in_successori_b);
}

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t POSet::TransitiveClosure(std::uint64_t lower, std::uint64_t upper)
{
    std::uint64_t result = 0;
    
    DATASTORE::iterator upset_lower = this->elementi.find(lower);
    if (upset_lower == this->elementi.end())
    {
        std::string el_str = this->starting_element_presentation->at(lower);
        std::string err_str = "Element " + el_str + " not found!";
        throw std::invalid_argument(err_str);
    }
    DATASTORE::iterator upset_upper = this->elementi.find(upper);
    if (upset_upper == this->elementi.end())
    {
        std::string el_str = this->starting_element_presentation->at(upper);
        std::string err_str = "Element " + el_str + " not found!";
        throw std::invalid_argument(err_str);
    }
    
    // insert successor of upper into successor of lower
    for (auto value_upset_upper:*(upset_upper->second)) {
        auto r = upset_lower->second->insert(value_upset_upper);
        if (r.second)
            ++result;
    }
    
    // insert upper and its successors into successor of element that contains lower
    for (auto element_upset: this->elementi) {
        if (element_upset.first == lower)
            continue;
        if (element_upset.second->find(lower) != element_upset.second->end())
        {
            auto r = element_upset.second->insert(upper);
            if (r.second) ++result;
            for (auto elemento:*(upset_upper->second))
            {
                auto r = element_upset.second->insert(elemento);
                if (r.second)
                    ++result;
            }
        }
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<Rcpp::LogicalMatrix> POSet::IncidenceMatrix() const {
    std::shared_ptr<Rcpp::StringVector> keys = this->ElementsKeys();
    std::shared_ptr<Rcpp::LogicalMatrix> result = std::make_shared<Rcpp::LogicalMatrix>(keys->size(), keys->size());
    
    std::uint64_t row = 0;
    for (auto in_set_out:this->elementi) {
        std::uint64_t value_out = in_set_out.first;
        std::uint64_t cols = 0;
        for (auto in_set_in:this->elementi) {
            std::uint64_t value_in = in_set_in.first;
            if ((value_out == value_in) || (in_set_out.second->find(value_in) != in_set_out.second->end())) {
                (*result)(row, cols) =  true;
            }
            else {
                (*result)(row, cols) =  false;
            }
            ++cols;
        }
        ++row;
    }
    Rcpp::CharacterVector rowsName(keys->begin(), keys->end());
    Rcpp::rownames(*result) = rowsName;
    Rcpp::CharacterVector colsName(keys->begin(), keys->end());
    Rcpp::colnames(*result) = colsName;
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::map<std::uint64_t, std::string>> POSet::Elements() {
    return this->starting_element_presentation;
}




// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<Rcpp::StringVector> POSet::ElementsKeys() const
{
    std::shared_ptr<Rcpp::StringVector> result = std::make_shared<Rcpp::StringVector>(this->starting_element_presentation->size());
    std::uint64_t k = 0;
    for(auto it = this->starting_element_presentation->begin(); it != this->starting_element_presentation->end(); ++it, ++k) {
        result->at(k) = it->second;
    }
    return result;
}


// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<LinearExtension> POSet::FirstLE() const {
    std::shared_ptr<DATASTORE> downSets = this->DownSet();
    std::shared_ptr<LinearExtension> result = std::make_shared<LinearExtension>(this->starting_element_presentation->size());
    std::set<std::uint64_t> setOne;
    
    for (auto el : (*downSets)) {
        if (el.second->size() == 0)
            setOne.insert(el.first);
    }
    
    for (long int k = 0; k < result->size(); ++k) {
        std::uint64_t min_el = *(setOne.begin());
        POSet::UpdateForFirst(downSets, setOne, min_el);
        result->set(k, min_el);
        /*
        std::string min_ele_str = starting_element_presentation->at(min_el);
        result->at(k) = min_ele_str;*/
    }
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<LatticeOfIdeals>  POSet::getLatticeOfIdeals() {
    if (this->latticeOfIdeals == nullptr) {
        std::shared_ptr<TreeOfIdeals> toi = this->getTreeOfIdeals();
        auto treeOfIdealsRoot = toi->getRoot();
        this->latticeOfIdeals = std::make_shared<LatticeOfIdeals>(toi, treeOfIdealsRoot);
    }
    return this->latticeOfIdeals;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<TreeOfIdeals>  POSet::getTreeOfIdeals() {
    if (this->treeOfIdeals == nullptr) {
        auto immediatePredecessors = this->imPred();
        auto linearExtensionForTreeOfIdeals = this->FirstLE();
        // convert the linearExtensionForTreeOfIdeals becouse the TreeOfIdeals assume that the
        // name of the poset elements correspond to the position in the linear extesion
        
        auto ipConverted = std::make_shared<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>>();
        for (auto pe : *immediatePredecessors) {
            auto val = pe.first;
            auto pos_val = linearExtensionForTreeOfIdeals->getPos(val);
            std::shared_ptr<std::set<std::uint64_t>> setConverted = std::make_shared<std::set<std::uint64_t>>();
            for (auto se : *pe.second) {
                auto pos_se = linearExtensionForTreeOfIdeals->getPos(se);
                setConverted->insert(pos_se + 1);
            }
            (*ipConverted)[pos_val + 1] = setConverted;
        }
        
        this->treeOfIdeals = std::make_shared<TreeOfIdeals>(ipConverted, linearExtensionForTreeOfIdeals);
    }
    return this->treeOfIdeals;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<Rcpp::LogicalMatrix> POSet::CoverMatrix() const {
    std::shared_ptr<Rcpp::StringVector> keys = this->ElementsKeys();
    std::shared_ptr<Rcpp::LogicalMatrix> result = std::make_shared<Rcpp::LogicalMatrix>(keys->size(), keys->size());
    
    
    std::uint64_t row = 0;
    for (auto row_el: this->elementi) {
        std::uint64_t col = 0;
        for (auto col_el: this->elementi) {
            if (row_el.second->find(col_el.first) != row_el.second->end()) {
                bool add = true;
                for (auto check_el: *(row_el.second)) {
                    auto check_set = this->elementi.find(check_el);
                    if (check_set->second->find(col_el.first) != check_set->second->end()) {
                        add = false;
                        break;
                    }
                }
                if (add) {
                    (*result)(row, col) = true;
                }
            }
            ++col;
        }
        ++row;
    }
    Rcpp::CharacterVector rowsName(keys->begin(), keys->end());
    Rcpp::rownames(*result) = rowsName;
    Rcpp::CharacterVector colsName(keys->begin(), keys->end());
    Rcpp::colnames(*result) = colsName;
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> POSet::coveredBy(std::shared_ptr<std::vector<std::uint64_t>> els) const {
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> result = std::make_shared<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>>();
    for (std::uint64_t k = 0; k < els->size(); ++k) {
        std::shared_ptr<std::set<std::uint64_t>> cs = coveredBy(els->at(k));
        (*result)[els->at(k)] = (cs);
    }
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::set<std::uint64_t>> POSet::coveredBy(std::uint64_t el) const {
    std::shared_ptr<std::set<std::uint64_t>> result = std::make_shared<std::set<std::uint64_t>>(*elementi.at(el));
    for (auto it_out = result->begin(); it_out != result->end(); ++it_out) {
        for (auto it_in = result->begin(); it_in != result->end();) {
            if (*it_out != *it_in && elementi.at(*it_out)->find(*it_in) != elementi.at(*it_out)->end()) {
                it_in = result->erase(it_in);
            }
            else {
                ++it_in;
            }
        }
    }
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> POSet::imPred() const {
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> result = std::make_shared<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>>();
    for (auto value : this->elementi) {
        std::shared_ptr<std::set<std::uint64_t>> cs = imPred(value.first);
        (*result)[value.first] = (cs);
        
    }
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> POSet::imPred(std::shared_ptr<std::vector<std::uint64_t>> els) const {
    std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> result = std::make_shared<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>>();
    for (std::uint64_t k = 0; k < els->size(); ++k) {
        std::shared_ptr<std::set<std::uint64_t>> cs = imPred(els->at(k));
        (*result)[els->at(k)] = (cs);
    }
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::set<std::uint64_t>> POSet::imPred(std::uint64_t el) const {
    std::shared_ptr<std::set<std::uint64_t>> result = std::make_shared<std::set<std::uint64_t>>();
    for (auto in_set : elementi) {
        if (in_set.second->find(el) != in_set.second->end()) {
            result->insert(in_set.first);
        }
    }
    
    for (auto it_out = result->begin(); it_out != result->end();++it_out) {
        for (auto it_in = result->begin(); it_in != result->end();) {
            if (*it_out != *it_in && elementi.at(*it_in)->find(*it_out) != elementi.at(*it_in)->end()) {
                it_in = result->erase(it_in);
            }
            else {
                ++it_in;
            }
        }
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::uint64_t POSet::StartPosition(std::string e)
{
    std::uint64_t result;
    if (this->starting_element_position.find(e) == this->starting_element_position.end())
        throw 1;
    result = this->starting_element_position[e];
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::map<std::uint64_t, std::uint64_t>> POSet::UpSetSizes()
{
    std::shared_ptr<std::map<std::uint64_t, std::uint64_t>> result;
    result = std::make_shared<std::map<std::uint64_t, std::uint64_t>>();
    
    for (auto el: this->elementi)
    {
        (*result)[el.first] = el.second->size() + 1;
    }

    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<POSet::DATASTORE> POSet::DownSet() const
{
    std::shared_ptr<POSet::DATASTORE> result = std::make_shared<POSet::DATASTORE>();
    
    for (auto el1: this->elementi)
    {
        auto result_insert = result->insert(std::make_pair(el1.first, std::make_shared<POSet::RELATEDSTORE>()));
        std::shared_ptr<RELATEDSTORE> dati = result_insert.first->second;
        for (auto el2: this->elementi)
        {
            if (el2.second->find(el1.first) != el2.second->end())
            {
                result->insert(std::make_pair(el2.first, std::make_shared<POSet::RELATEDSTORE>()));
                dati->insert(el2.first);
            }
        }
    }
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::set<std::uint64_t>> POSet::DownSet(std::set<std::uint64_t>& els) {
    auto result = std::make_shared<std::set<std::uint64_t>>();
    
    for (auto el : els) {
        result->insert(el);
    }
    
    for (auto key_val: this->elementi) {
        bool at_least_one = false;
        for (auto el : els) {
            if (key_val.second->find(el) != key_val.second->end()) {
                at_least_one = true;
                break;
            }
        }
        if (at_least_one) {
            result->insert(key_val.first);
        }
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::set<std::uint64_t>> POSet::UpSet(std::set<std::uint64_t>& els) {
    auto result = std::make_shared<std::set<std::uint64_t>>();
    
    for (auto el : els) {
        result->insert(el);
    }
    
    for (auto el_ext : els) {
        for (auto el : *this->elementi[el_ext]) {
            result->insert(el);
        }
    }
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

bool POSet::GreaterThan(std::uint64_t e1, std::uint64_t e2)
{
    // e2 < e1 true
    // otherwise false
    
    auto v1 = this->elementi.find(e1);
    auto v2 = this->elementi.find(e2);

    if (v1 == this->elementi.end()) {
        std::string el_str = this->starting_element_presentation->at(e1);
        std::string err_str = "POSet error: " + el_str + " not found!";
        throw std::invalid_argument(err_str);
    }
    if (v2 == this->elementi.end()) {
        std::string el_str = this->starting_element_presentation->at(e2);
        std::string err_str = "POSet error: " + el_str + " not found!";
        throw std::invalid_argument(err_str);
    }
    
    std::shared_ptr<RELATEDSTORE> v2_upset = v2->second;
    bool result = v2_upset->find(e1) != v2_upset->end();
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> POSet::incomparabilities()
{
    std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> result;
    result = std::make_shared<std::vector<std::pair<std::uint64_t, std::uint64_t>>>();
    
    for (auto out_iter = this->elementi.begin(); out_iter != this->elementi.end(); ++out_iter) {
        auto in_iter = out_iter;
        for (++in_iter; in_iter != this->elementi.end(); ++in_iter) {
            //for (auto inset_e2 : this->elementi) {
            if (out_iter->second->find(in_iter->first) == out_iter->second->end() && in_iter->second->find(out_iter->first) == in_iter->second->end()) {
                result->push_back(std::make_pair(out_iter->first, in_iter->first));
            }
        }
    }
    return result;
}


// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> POSet::comparabilities()
{
    std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> result;
    result = std::make_shared<std::vector<std::pair<std::uint64_t, std::uint64_t>>>();
    
    for (auto out_iter = this->elementi.begin(); out_iter != this->elementi.end(); ++out_iter) {
        auto out_set = out_iter->second;
        for (auto el : (*out_set)) {
            result->push_back(std::make_pair(out_iter->first, el));
        }
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

void POSet::evaluation(std::shared_ptr<std::map<std::string, ParamType>> parms,
                       std::shared_ptr<EvalResult> result,
                       std::shared_ptr<std::uint64_t> current_number_linear_extension_generated,
                       std::shared_ptr<std::uint64_t> total_number_linear_extension,
                       std::shared_ptr<bool> end_process,
                       std::shared_ptr<DisplayMessage> displayMessage)
{

    std::shared_ptr<std::vector<std::shared_ptr<FunctionLinearExtension>>> fles = nullptr;
    if (auto* p = std::get_if<std::shared_ptr<std::vector<std::shared_ptr<FunctionLinearExtension>>>>(&(*parms)["Functions linear extension"])) {
        fles = *p;
    } else {
      std::string err_str = "POSet error: Function linear extension";
      throw_line(err_str);
    }
    std::shared_ptr<LinearExtensionGenerator> leg = nullptr;
    if (auto* p = std::get_if<std::shared_ptr<LinearExtensionGenerator>>(&(*parms)["Linear extension generator"])) {
        leg = *p;
    } else {
      std::string err_str = "POSet error: Linear extension generator";
      throw_line(err_str);
    }
    std::shared_ptr<TranformExtension> te = nullptr;
    if (auto* p = std::get_if<std::shared_ptr<TranformExtension>>(&(*parms)["Transform extension"])) {
        te = *p;
    } else {
      std::string err_str = "POSet error: Transform extension";
      throw_line(err_str);
    }
    
    std::vector<std::shared_ptr<Matrice>> eval_results;
    std::vector<std::shared_ptr<Rcpp::NumericMatrix>> results_rcpp;
    for (std::uint64_t k = 0; k < fles->size(); ++k) {
        std::shared_ptr<FunctionLinearExtension> fle = fles->at(k);
        std::pair<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::vector<std::string>>> dim = fle->size();
        std::shared_ptr<Matrice> eval_result = std::make_shared<Matrice>(dim.first->size(), dim.second->size());
        eval_results.push_back(eval_result);
        
        std::shared_ptr<Rcpp::NumericMatrix> result_rcpp = std::make_shared<Rcpp::NumericMatrix>(dim.first->size(), dim.second->size());
        Rcpp::CharacterVector rowsName(dim.first->begin(), dim.first->end());
        Rcpp::rownames(*result_rcpp) = rowsName;
        Rcpp::CharacterVector colsName(dim.second->begin(), dim.second->end());
        Rcpp::colnames(*result_rcpp) = colsName;
        results_rcpp.push_back(result_rcpp);
    }
    
    (*current_number_linear_extension_generated) = 0;
    
    leg->start();
    (*total_number_linear_extension) = leg->numberOfLE();

    displayMessage->Start();
    while (true) {
        std::shared_ptr<std::vector<std::shared_ptr<LinearExtension>>> les = leg->get();
        for (size_t k = 0; k < les->size(); ++k) {
            std::shared_ptr<LinearExtension> leval = les->at(k);
            std::shared_ptr<LinearExtension> letrasnsf = (*te)(leval);
            for (std::uint64_t k = 0; k < fles->size(); ++k) {
                std::shared_ptr<FunctionLinearExtension> fle = fles->at(k);
                (*fle)(letrasnsf);
                std::shared_ptr<Matrice> eval_result = eval_results.at(k);
                this->AverageUpdate(eval_result, fle, (*current_number_linear_extension_generated));
            }
            ++(*current_number_linear_extension_generated);
            displayMessage->Display();
        }
        if (!leg->hasNext())
            break;
        leg->next();
    }
    displayMessage->Stop();
    for (size_t k = 0; k < eval_results.size(); ++k) {
        std::shared_ptr<Matrice> eval_result = eval_results.at(k);
        std::shared_ptr<Rcpp::NumericMatrix> result_rcpp = results_rcpp.at(k);
        for (int riga = 0; riga < result_rcpp->nrow(); ++riga) {
            for (int colonna = 0; colonna < result_rcpp->ncol(); ++colonna) {
                (*result_rcpp)(riga, colonna) = eval_result->at(riga, colonna);
            }
        }
        result->push_back(result_rcpp);
    }
    *end_process = true;
    return;
}

// ***********************************************
// ***********************************************
// ***********************************************

void POSet::AverageUpdate(std::shared_ptr<Matrice> ris,
                          std::shared_ptr<FunctionLinearExtension> fle,
                          std::uint64_t le_number) {
    // ris <- ris * (iter / (iter + 1)) + (val / (iter + 1)))
    
    for (std::uint64_t k = 0; k < fle->resSize(); ++k) {
        std::uint64_t first = fle->at0(k);
        std::uint64_t second = fle->at1(k);
        double add_val = fle->at2(k);
        
        double old_val = ris->at(first, second);
        double new_val = old_val * (le_number / (le_number + 1.0)) + add_val / (le_number + 1.0);
        (*ris)(first, second) = new_val;
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<POSet> POSet::clone() {
    std::vector<std::string> elements;
    for (auto v:*this->starting_element_presentation)
        elements.push_back(v.second);
    
    std::vector<std::pair<std::string, std::string>> comparabilities;
    for (auto d1:this->elementi) {
        std::string v1 = this->starting_element_presentation->at(d1.first);
        std::shared_ptr<RELATEDSTORE> d1_set = d1.second;
        for (auto d2 : *(d1_set)) {
            std::string v2 = this->starting_element_presentation->at(d2);
            comparabilities.push_back(std::pair<std::string, std::string>(v1, v2));
        }
    }
    
    std::shared_ptr<POSet> result = std::make_shared<POSet>(elements, comparabilities);
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::vector<std::uint64_t>> POSet::maximals() {
    std::shared_ptr<std::vector<std::uint64_t>> result = std::make_shared<std::vector<std::uint64_t>>();
    for (auto d:this->elementi) {
        std::shared_ptr<RELATEDSTORE> d_set = d.second;
        if (d_set->empty())
            result->push_back(d.first);
    }
    return result;
}


// ***********************************************
// ***********************************************
// ***********************************************

void POSet::remove(std::shared_ptr<std::vector<std::uint64_t>> input_elemets) {
    
    for (auto element_to_be_removed: *input_elemets) {
        // remove from this->elementi
        this->elementi.erase(element_to_be_removed);
        for (auto d:this->elementi) {
            d.second->erase(element_to_be_removed);
        }
        // remove from this->starting_element_presentation
        auto value = this->starting_element_presentation->at(element_to_be_removed);
        this->starting_element_presentation->erase(element_to_be_removed);
        // remove from this->starting_element_position
        this->starting_element_position.erase(value);
    }
}


// ***********************************************
// ***********************************************
// ***********************************************

void POSet::remove(std::shared_ptr<std::vector<std::string>> input_elemets) {
    std::shared_ptr<std::vector<std::uint64_t>> input_elements_index = std::make_shared<std::vector<std::uint64_t>>();
    for (auto element_to_be_removed: *input_elemets) {
        std::uint64_t value = this->starting_element_position.at(element_to_be_removed);
        input_elements_index->push_back(value);
    }
    this->remove(input_elements_index);
}

// ***********************************************
// ***********************************************
// ***********************************************

template <typename SHSet>
bool set_compare (SHSet const &lhs, SHSet const &rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}


// ***********************************************
// ***********************************************
// ***********************************************

void POSet::UpdateForFirst(std::shared_ptr<POSet::DATASTORE> downSets, std::set<std::uint64_t>& setOne, std::uint64_t min_el)
{
    setOne.erase(min_el);
    downSets->erase(min_el);
    for (auto el : (*downSets))
    {
        if (el.second->find(min_el) != el.second->end())
        {
            el.second->erase(min_el);
            if (el.second->size() == 0)
            {
                setOne.insert(el.first);
            }
        }
        
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

