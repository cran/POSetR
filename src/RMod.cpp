#include <Rcpp.h>
#include <memory>
#include <cstdint>
#include <mutex>
#include "RMod.h"

#include "tranformExtension.h"
#include "linearExtensionGenerator.h"
#include "functionLinearExtension.h"
#include "functionLinearExtensionR.h"
#include "displayMessage.h"
#include "displayMessageR.h"
#include "utilita.h"
#include "poset.h"
#include "myException.h"


#define throw_line(arg) throw my_exception(arg, __FILE__, __LINE__);


// [[Rcpp::plugins(cpp17)]]

/*
 IN R
 
 prima di compilare:
 Sys.setenv("PKG_CXXFLAGS"="-std=c++11 -Wsign-compare")
 
 per eseguire:

 require(Rcpp)
 require(inline)
 library(nome_del_package)
 variabile_modulo <- Module(nome_del_modulo, getDynLib(nome_del_package))
 nome_classe <- variabile_modulo$Classe_Esposta
 oggetto_costruito <- new(nome_classe, lista_argomenti_costruttore)
 
 */

// ***********************************************
// ***********************************************
// ***********************************************

POSetR::POSetR(Rcpp::StringVector elements1, Rcpp::StringMatrix comparabilities1,
               Rcpp::StringVector elements2, Rcpp::StringMatrix comparabilities2,
               Rcpp::String sep) {
    if (elements1.size() == 0) {
        std::string err_str = "Empty POSet 1";
        throw_line(err_str);
    }
    if (elements2.size() == 0) {
        std::string err_str = "Empty POSet 2";
        throw_line(err_str);
    }
    try {
        if (comparabilities1.ncol() != 2) {
            std::string err_str = "POSet error: comparabilities 1 size (" + std::to_string(comparabilities1.ncol()) + ")";
            throw_line(err_str);
        }
        if (comparabilities2.ncol() != 2) {
            std::string err_str = "POSet error: comparabilities 2 size (" + std::to_string(comparabilities2.ncol()) + ")";
            throw_line(err_str);
        }
        
        std::map<std::string, std::shared_ptr<std::set<std::string>>> map_comparabilities_1;
        for (auto k = 0; k < elements1.size(); ++k) {
            std::string e = Rcpp::as<std::string>(elements1[k]);
            if (map_comparabilities_1.find(e) == map_comparabilities_1.end()) {
                std::shared_ptr<std::set<std::string>> dati = std::make_shared<std::set<std::string>>();
                dati->insert(e);
                map_comparabilities_1.insert(std::make_pair(e, dati));
            }
        }
        
        
        for (auto k = 0; k < comparabilities1.nrow(); ++k) {
            std::string first = Rcpp::as<std::string>(comparabilities1(k, 0));
            std::string second = Rcpp::as<std::string>(comparabilities1(k, 1));
            map_comparabilities_1.at(first)->insert(second);
            
        }
       
       std::map<std::string, std::shared_ptr<std::set<std::string>>> map_comparabilities_2;
        for (auto k = 0; k < elements2.size(); ++k) {
            std::string e = Rcpp::as<std::string>(elements2[k]);
            if (map_comparabilities_2.find(e) == map_comparabilities_2.end()) {
                std::shared_ptr<std::set<std::string>> dati = std::make_shared<std::set<std::string>>();
                dati->insert(e);
                map_comparabilities_2.insert(std::make_pair(e, dati));
            }
        }
        for (auto k = 0; k < comparabilities2.nrow(); ++k) {
            std::string first = Rcpp::as<std::string>(comparabilities2(k, 0));
            std::string second = Rcpp::as<std::string>(comparabilities2(k, 1));
            map_comparabilities_2.at(first)->insert(second);
            
        }
       
        std::vector<std::string> poset_elements;
        std::map<std::string, std::pair<std::string, std::string>> map_comparabilities;

        for (auto k = 0; k < elements1.size(); ++k) {
            std::string e1 = Rcpp::as<std::string>(elements1[k]);
            for (auto h = 0; h < elements2.size(); ++h) {
                std::string e2 = Rcpp::as<std::string>(elements2[h]);
                std::string e = e1 + sep.get_cstring() + e2;
                poset_elements.push_back(e);
                map_comparabilities.insert(std::make_pair(e, std::make_pair(e1, e2)));
            }
        }
        
        std::vector<std::pair<std::string, std::string>> poset_comparabilities;
        for (auto const& [key1, val1] : map_comparabilities) {
            for (auto const& [key2, val2] : map_comparabilities) {
                
                if (
                    map_comparabilities_1.at(val1.first)->find(val2.first) != map_comparabilities_1.at(val1.first)->end() &&
                    map_comparabilities_2.at(val1.second)->find(val2.second) != map_comparabilities_2.at(val1.second)->end()
                    ) {
                    poset_comparabilities.push_back(std::pair<std::string, std::string>(key1, key2));
                }
            }
        }
        this->poset = std::make_shared<POSet>(poset_elements, poset_comparabilities);
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

POSetR::POSetR(Rcpp::StringVector elements, Rcpp::StringMatrix comparabilities) {
    if (elements.size() == 0) {
        std::string err_str = "Empty POSet";
        throw_line(err_str);
    }
    try {
        if (comparabilities.ncol() != 2) {
            std::string err_str = "POSet error: comparabilities size (" + std::to_string(comparabilities.ncol()) + ")";
            throw_line(err_str);
        }
        std::vector<std::string> poset_elements;
        for (auto k = 0; k < elements.size(); ++k) {
            std::string e = Rcpp::as<std::string>(elements[k]);
            poset_elements.push_back(e);
        }
        std::vector<std::pair<std::string, std::string>> poset_comparabilities;
        for (auto k = 0; k < comparabilities.nrow(); ++k) {
            std::string first = Rcpp::as<std::string>(comparabilities(k, 0));
            std::string second = Rcpp::as<std::string>(comparabilities(k, 1));
        
            poset_comparabilities.push_back(std::pair<std::string, std::string>(first, second));
        }
        this->poset = std::make_shared<POSet>(poset_elements, poset_comparabilities);
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

POSetR::POSetR(Rcpp::StringVector elements) {
    if (elements.size() == 0) {
        std::string err_str = "Empty POSet";
        throw_line(err_str);
    }
    try {
        std::vector<std::string> poset_elements;
        for (auto k = 0; k < elements.size(); ++k) {
            std::string e = Rcpp::as<std::string>(elements[k]);
            poset_elements.push_back(e);
        }
        std::vector<std::pair<std::string, std::string>> poset_comparabilities;
        this->poset = std::make_shared<POSet>(poset_elements, poset_comparabilities);
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::LogicalMatrix POSetR::IncidenceMatrix() const {
    try {
        std::shared_ptr<Rcpp::LogicalMatrix> incidenceMatrixFromPoset = this->poset->IncidenceMatrix();
        return *incidenceMatrixFromPoset;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::LogicalMatrix();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::LogicalMatrix POSetR::CoverMatrix() const {
    try {
        std::shared_ptr<Rcpp::LogicalMatrix> coverMatrixFromPoset = this->poset->CoverMatrix();
        return *coverMatrixFromPoset;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::LogicalMatrix();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::StringMatrix POSetR::comparabilities() const {
    try {
        std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> comparabilitiesFromPoset = this->poset->comparabilities();
        Rcpp::StringMatrix result((int) comparabilitiesFromPoset->size(), 2);
        
        for (std::uint64_t row = 0; row < comparabilitiesFromPoset->size(); ++row) {
            std::string first = this->poset->GetElement(comparabilitiesFromPoset->at(row).first);
            std::string second = this->poset->GetElement(comparabilitiesFromPoset->at(row).second);
            result(row, 0) = first;
            result(row, 1) = second;
        }
        return result;
        
        return result;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::StringMatrix();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::StringMatrix POSetR::incomparabilities() const {
    try {
        std::shared_ptr<std::vector<std::pair<std::uint64_t, std::uint64_t>>> incomparabilitiesFromPoset = this->poset->incomparabilities();
        Rcpp::StringMatrix result((int) incomparabilitiesFromPoset->size(), 2);
        
        for (std::uint64_t row = 0; row < incomparabilitiesFromPoset->size(); ++row) {
            std::string first = this->poset->GetElement(incomparabilitiesFromPoset->at(row).first);
            std::string second = this->poset->GetElement(incomparabilitiesFromPoset->at(row).second);
            result(row, 0) = first;
            result(row, 1) = second;
        }
        return result;
        
        return result;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::StringMatrix();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::StringVector POSetR::Elements() const {
    try {
        std::shared_ptr<Rcpp::StringVector> elements = this->poset->ElementsKeys();
        return *elements;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::StringVector();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::StringVector POSetR::firstLE() const {
    try {
        std::shared_ptr<LinearExtension> first_linear_extention_poset = this->poset->FirstLE();
        std::shared_ptr<Rcpp::StringVector> first_linear_extention_R = std::make_shared<Rcpp::StringVector>(first_linear_extention_poset->size());
        for (std::uint64_t k = 0; k < first_linear_extention_poset->size(); ++k) {
            auto val = first_linear_extention_poset->getVal(k);

            std::string min_ele_str = this->poset->GetElement(val);
            first_linear_extention_R->at(k) = min_ele_str;
        }
        
        
        return *first_linear_extention_R;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::StringVector();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::List POSetR::coveredBy(Rcpp::StringVector r_elements) const {
    std::shared_ptr<std::vector<std::uint64_t>> c_elements = std::make_shared<std::vector<std::uint64_t>>();
    for (int k = 0; k < r_elements.size(); ++k) {
        std::string r_el = Rcpp::as<std::string>(r_elements[k]);
        try {
            std::uint64_t c_el = this->poset->StartPosition(r_el);
            c_elements->push_back(c_el);
        } catch(int ex) {
            std::string err_msg = r_el + " not used";
            ::Rf_warning(r_el.c_str());
        }
    }
    try {
        std::shared_ptr<std::map<std::uint64_t, std::shared_ptr<std::set<std::uint64_t>>>> c_coveredBy = this->poset->coveredBy(c_elements);
        Rcpp::List r_coveredBy;
        for (std::uint64_t k = 0; k < c_elements->size(); ++k) {
            if (c_coveredBy->at(c_elements->at(k))->size() > 0) {
                Rcpp::StringVector r_covered(c_coveredBy->at(c_elements->at(k))->size());
                int h = 0;
                for (auto c_ds_el : *c_coveredBy->at(c_elements->at(k))) {
                    std::string r_ds_el = this->poset->GetElement(c_ds_el);
                    r_covered[h++] = r_ds_el;
                }
                r_coveredBy.push_back(r_covered);
            } else {
                r_coveredBy.push_back(Rcpp::StringVector());
            }
        }
        
        return r_coveredBy;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::List();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::StringVector POSetR::downSet(Rcpp::StringVector r_elements) const {
    std::shared_ptr<std::set<std::uint64_t>> c_elements = std::make_shared<std::set<std::uint64_t>>();
    for (int k = 0; k < r_elements.size(); ++k) {
        std::string r_el = Rcpp::as<std::string>(r_elements[k]);
        try {
            std::uint64_t c_el = this->poset->StartPosition(r_el);
            c_elements->insert(c_el);
        } catch(int ex) {
            std::string err_msg = r_el + " not used";
            ::Rf_warning(r_el.c_str());
        }
    }
    try {
        std::shared_ptr<std::set<std::uint64_t>> c_downSet = this->poset->DownSet(*c_elements);
        Rcpp::StringVector r_downSet(c_downSet->size());
        
        int k = 0;
        for (auto c_ds_el : *c_downSet) {
            std::string r_ds_el = this->poset->GetElement(c_ds_el);
            r_downSet[k++] = r_ds_el;
        }
        return r_downSet;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::StringVector();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::StringVector POSetR::upSet(Rcpp::StringVector r_elements) const {
    std::shared_ptr<std::set<std::uint64_t>> c_elements = std::make_shared<std::set<std::uint64_t>>();
    for (int k = 0; k < r_elements.size(); ++k) {
        std::string r_el = Rcpp::as<std::string>(r_elements[k]);
        try {
            std::uint64_t c_el = this->poset->StartPosition(r_el);
            c_elements->insert(c_el);
        } catch(int ex) {
            std::string err_msg = r_el + " not used";
            ::Rf_warning(r_el.c_str());
        }
    }
    try {
        std::shared_ptr<std::set<std::uint64_t>> c_upSet = this->poset->UpSet(*c_elements);
        Rcpp::StringVector r_upSet(c_upSet->size());
        
        int k = 0;
        for (auto c_us_el : *c_upSet) {
            std::string r_us_el = this->poset->GetElement(c_us_el);
            r_upSet[k++] = r_us_el;
        }
        return r_upSet;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    
    return Rcpp::StringVector();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::List POSetR::latticeOfIdeals() const {
    try {
        auto c_lattice_of_ideals = this->poset->getLatticeOfIdeals()->toList();
        
        Rcpp::List r_lattice_of_ideals;
        for (auto c_elem : *c_lattice_of_ideals) {
            size_t a = std::get<0>(c_elem);
            size_t b = std::get<1>(c_elem);
            size_t label = std::get<2>(c_elem);
            Rcpp::StringVector r_elem = Rcpp::StringVector::create(std::to_string(a), std::to_string(b));
            r_lattice_of_ideals.push_back(r_elem, this->poset->GetElement(label));
        }
        return r_lattice_of_ideals;
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    return Rcpp::List();
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::List POSetR::evaluation(Rcpp::List args)
{
    try {
        std::string output_file_name = "";
        if (args.containsElementNamed("OutputFileName"))
            output_file_name = Rcpp::as<std::string>(args["OutputFileName"]);

        std::shared_ptr<TranformExtension> tle = evaluationTE(args);
        std::shared_ptr<std::vector<std::shared_ptr<FunctionLinearExtension>>> fles = evaluationFLE(args, tle->Elements());
        std::shared_ptr<LinearExtensionGenerator> gle = evaluationLEG(args);

        std::shared_ptr<std::map<std::string, ParamType>> evaluation_parms = std::make_shared<std::map<std::string, ParamType>>();
        (*evaluation_parms)["Functions linear extension"] = fles;
        (*evaluation_parms)["Linear extension generator"] = gle;
        (*evaluation_parms)["Transform extension"] = tle;
        std::shared_ptr<EvalResult> evaluation_result = std::make_shared<EvalResult>();
        std::shared_ptr<std::uint64_t> number_of_extension = std::make_shared<std::uint64_t>(0);
        std::shared_ptr<std::uint64_t> total_number_of_extension = std::make_shared<std::uint64_t>(0);
        std::shared_ptr<bool> end_process = std::make_shared<bool>(false);
        
        // START

        std::shared_ptr<std::map<std::string, ParamType>> displayParms = std::make_shared<std::map<std::string, ParamType>>();
        std::shared_ptr<Rcpp::Function> pb_update_function = nullptr;
        if (args.containsElementNamed("ProgressBarUpdate")) {
            pb_update_function = std::make_shared<Rcpp::Function>(Rcpp::as<Rcpp::Function>(args["ProgressBarUpdate"]));
            (*displayParms)["ProgressBarUpdate"] = pb_update_function;
        }
        (*displayParms)["TotalNumberExtension"] = total_number_of_extension;
        (*displayParms)["NumberExtension"] = number_of_extension;
        std::shared_ptr<DisplayMessage> displayMessage = std::make_shared<DisplayMessageR>(displayParms);

        this->poset->evaluation(evaluation_parms, evaluation_result, number_of_extension, total_number_of_extension, end_process, displayMessage);

        if (output_file_name != "") {
            evaluation_result->to_file(output_file_name);
        }
        Rcpp::List result = BuildResult(evaluation_result);

        return result;
        
    } catch(std::exception &ex) {
        forward_exception_to_r(ex);
    } catch(...) {
        ::Rf_error("c++ exception (unknown reason)");
    }
    
    return Rcpp::List();
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<TranformExtension> POSetR::evaluationTE(Rcpp::List args) {
    std::shared_ptr<TranformExtension> result = nullptr;
    if (args.containsElementNamed("TranformExtension")) {
        std::string tle_name = Rcpp::as<std::string>(args["TranformExtension"]);
        if (tranformExtensionMapType.find(tle_name) == tranformExtensionMapType.end()) {
            std::string err_str = "TranformExtension name not valid!!!";
            throw_line(err_str);
        }
        TranformExtensionType te_type = tranformExtensionMapType.at(tle_name);
        std::shared_ptr<std::map<std::string, ParamType>> te_parms = std::make_shared<std::map<std::string, ParamType>>();
        (*te_parms)["TEElements"] = this->poset->Elements();
        switch (te_type) {
            case TranformExtensionType::TETItentity: {
                result = std::make_shared<TEItentity>(te_parms);
                break;
            }
            case TranformExtensionType::TETLexicographical: {
                std::shared_ptr<std::vector<std::uint64_t>> modalities= nullptr;
                if (!args.containsElementNamed("TEModalities")) {
                    std::string err_str = "Lexicographical need TEModalities parameters";
                    throw_line(err_str);
                }
                Rcpp::NumericVector lmod = Rcpp::as<Rcpp::NumericVector>(args["TEModalities"]);
                modalities = std::make_shared<std::vector<std::uint64_t>>(lmod.size());
                for (int k = 0; k < lmod.size(); ++k) {
                    modalities->at(k) = lmod[k];
                }
                (*te_parms)["TEModalities"] = modalities;
                result = std::make_shared<TELexicographical>(te_parms);
                break;
            }
            default: {
                std::string err_str = "TranformExtension type not valid!!!";
                throw_line(err_str);
            }
        }
    } else {
        std::shared_ptr<std::map<std::string, ParamType>> te_parms = std::make_shared<std::map<std::string, ParamType>>();
        (*te_parms)["TEElements"] = this->poset->Elements();
        result = std::make_shared<TEItentity>(te_parms);
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<std::vector<std::shared_ptr<FunctionLinearExtension>>> POSetR::evaluationFLE(
                                                                                             Rcpp::List args,
                                                                                             std::shared_ptr<std::map<std::uint64_t, std::string>> fleKeys) {
    std::shared_ptr<std::vector<std::shared_ptr<FunctionLinearExtension>>> result = nullptr;
    result = std::make_shared<std::vector<std::shared_ptr<FunctionLinearExtension>>>();
    
    if (!args.containsElementNamed("FunctionsLinearExtension")) {
        std::string err_str = "FunctionsLinearExtension required";
        throw_line(err_str);
    }
    
    if (!Rcpp::is<Rcpp::List>(args["FunctionsLinearExtension"])) {
        std::string err_str = "FunctionsLinearExtension is not a List";
        throw_line(err_str);
    }
    Rcpp::List fle_list = Rcpp::as<Rcpp::List>(args["FunctionsLinearExtension"]);
    for (long fle_list_index = 0; fle_list_index < fle_list.size(); ++fle_list_index) {
        std::shared_ptr<std::map<std::string, ParamType>> fle_parms = std::make_shared<std::map<std::string, ParamType>>();
        (*fle_parms)["FunctionLinearExtensionPOSet"] = this->poset;
        (*fle_parms)["FLEKeys"] = fleKeys;
        
        if (Rcpp::is<Rcpp::String>(fle_list.at(fle_list_index))) {
            std::string fle_name = Rcpp::as<std::string>(fle_list.at(fle_list_index));
            
            if (functionLinearMapType.find(fle_name) == functionLinearMapType.end()) {
                std::string err_str = "FunctionLinearExtension name not valid!!!";
                throw_line(err_str);
            }
            FunctionLinearType fle_type = functionLinearMapType.at(fle_name);
            switch (fle_type) {
                case FunctionLinearType::FLETMutualRankingProbability: {
                    if (args.containsElementNamed("FLEMRPSelection")) {
                        if (Rcpp::is<Rcpp::StringMatrix>(args["FLEMRPSelection"])) {
                            (*fle_parms)["FLEMRPSelection"] = std::make_shared<Rcpp::StringMatrix>(Rcpp::as<Rcpp::StringMatrix>(args["FLEMRPSelection"]));
                        }
                    }
                    result->push_back(std::make_shared<FLEMutualRankingProbability>(fle_parms));
                    break;
                }
                case FunctionLinearType::FLETMutualRankingProbability2: {
                    if (args.containsElementNamed("FLEMRPSelection")) {
                        if (Rcpp::is<Rcpp::StringMatrix>(args["FLEMRPSelection"])) {
                            (*fle_parms)["FLEMRPSelection"] = std::make_shared<Rcpp::StringMatrix>(Rcpp::as<Rcpp::StringMatrix>(args["FLEMRPSelection"]));
                        }
                    }
                    result->push_back(std::make_shared<FLEMutualRankingProbability2>(fle_parms));
                    break;
                }
                case FunctionLinearType::FLETAverageHeight: {
                    result->push_back(std::make_shared<FLEAverageHeight>(fle_parms));
                    break;
                }
                case FunctionLinearType::FLETSeparation: {
                    if (args.containsElementNamed("FLESepSelection")) {
                        if (Rcpp::is<Rcpp::StringMatrix>(args["FLESepSelection"])) {
                            (*fle_parms)["FLESepSelection"] = std::make_shared<Rcpp::StringMatrix>(Rcpp::as<Rcpp::StringMatrix>(args["FLESepSelection"]));
                        }
                    }
                    result->push_back(std::make_shared<FLESeparation>(fle_parms));
                    break;
                }
                default:
                    throw std::invalid_argument{"FunctionLinearExtension!!!"};
            }
        } else if (Rcpp::is<Rcpp::Function>(fle_list.at(fle_list_index))) {
            
            if (!args.containsElementNamed("FunctionsLinearExtensionSize")) {
                std::string err_str = "FunctionsLinearExtensionSize required";
                throw_line(err_str);
            }
            Rcpp::List fle_size_list = Rcpp::as<Rcpp::List>(args["FunctionsLinearExtensionSize"]);

            std::shared_ptr<Rcpp::NumericVector> size = std::make_shared<Rcpp::NumericVector>(Rcpp::as<Rcpp::NumericVector>(fle_size_list.at(fle_list_index)));
            std::shared_ptr<Rcpp::Function> Rfunction = std::make_shared<Rcpp::Function>(Rcpp::as<Rcpp::Function>(fle_list.at(fle_list_index)));

            (*fle_parms)["FunctionLinearExtensionSize"] = size;
            (*fle_parms)["FunctionLinearExtension"] = Rfunction;
            result->push_back(std::make_shared<FLER>(fle_parms));

        } else {
            throw std::invalid_argument{"FunctionLinearExtension!!!"};
        }
    }
    
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<LinearExtensionGenerator> POSetR::evaluationLEG(Rcpp::List args) {
    std::shared_ptr<LinearExtensionGenerator> result = nullptr;
    if (!args.containsElementNamed("LinearExtensionGenerator")) {
        std::string err_str = "LinearExtensionGenerator required!";
        throw_line(err_str);
    }
    
    std::string leg_name = Rcpp::as<std::string>(args["LinearExtensionGenerator"]);
    if (linearExtensionGeneratorMapType.find(leg_name) == linearExtensionGeneratorMapType.end()) {
        std::string err_str = "LinearExtensionGenerator name not valid!";
        throw_line(err_str);
    }
    LinearExtensionGeneratorType leg_type = linearExtensionGeneratorMapType.at(leg_name);
    std::shared_ptr<std::map<std::string, ParamType>> gle_parms = std::make_shared<std::map<std::string, ParamType>>();
    
    std::shared_ptr<std::string> leg_file_output = nullptr;

    if (args.containsElementNamed("LinearExtensionGeneratorOutputFile")) {
        leg_file_output = std::make_shared<std::string>(Rcpp::as<std::string>(args["LinearExtensionGeneratorOutputFile"]));
        (*gle_parms)["LinearExtensionGeneratorOutputFile"] = leg_file_output;
    }
    switch (leg_type)
    {
        case LinearExtensionGeneratorType::LEGTBubleyDyer: {
            std::uint64_t PRECISION_BD = 0;
            std::shared_ptr<std::uint64_t> SEED_BD = nullptr;
            std::uint64_t MAX_ITERATIONS = 0;
            if (args.containsElementNamed("BubleyDyerPrecision"))
                PRECISION_BD = Rcpp::as<std::uint64_t>(args["BubleyDyerPrecision"]);
            if (args.containsElementNamed("BubleyDyerSeed"))
                SEED_BD = std::make_shared<std::uint64_t>(Rcpp::as<std::uint64_t>(args["BubleyDyerSeed"]));
            if (args.containsElementNamed("BubleyDyerMaxIterations"))
                MAX_ITERATIONS = Rcpp::as<std::uint64_t>(args["BubleyDyerMaxIterations"]);
            (*gle_parms)["BubleyDyerPOSet"] = this->poset;
            (*gle_parms)["BubleyDyerPrecision"] = PRECISION_BD;
            (*gle_parms)["BubleyDyerSeed"] = SEED_BD;
            (*gle_parms)["BubleyDyerMaxIterations"] = MAX_ITERATIONS;
            result = std::make_shared<LEGBubleyDyer>(gle_parms);
            break;
        }
        case LinearExtensionGeneratorType::LEGThmnsAllLE: {
            (*gle_parms)["POSet"] = this->poset;
            result = std::make_shared<LEGByTreeOfIdeals>(gle_parms);
            break;
        }
        case LinearExtensionGeneratorType::LEGTAllLE: {
            (*gle_parms)["POSet"] = this->poset;
            result = std::make_shared<LEGAllLE>(gle_parms);
            break;
        }
        default: {
            std::string err_str = "LinearExtensionGenerator name not valid!!!";
            throw_line(err_str);
        }
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************

Rcpp::List POSetR::BuildResult(std::shared_ptr<EvalResult> evaluationResult) {
    Rcpp::List result;
    for (size_t k = 0; k < evaluationResult->size(); ++k) {
        std::shared_ptr<Rcpp::NumericMatrix> val = evaluationResult->at(k);
        result.push_back(*val);
    }
    
    return result;
}


// ***********************************************
// ***********************************************
// ***********************************************

void POSetR::show() const {
    Rcpp::StringVector e = this->Elements();
    
    Rcpp::Rcout << "elements:\n";
    for (auto k = 0; k < e.length(); ++k) {
        if (k == e.length() - 1)
            Rcpp::Rcout << e(k);
        else
            Rcpp::Rcout << e(k) << ", " ;
    }
    Rcpp::Rcout << "\n";

    Rcpp::StringMatrix x = this->comparabilities();
    auto n = x.nrow();
    if (n > 0) {
        Rcpp::Rcout << "\nstrict comparabilities:\n";
        for (auto k = 0; k < n; ++k) {
            Rcpp::Rcout << x(k, 0) << " < " << x(k, 1) << "\n";
        }
    } else {
        if (e.length() > 1) {
            Rcpp::Rcout << "\nthe poset is an anti-chain\n";
        }
    }
}

// ***********************************************
// ***********************************************
// ***********************************************

RCPP_EXPOSED_CLASS(POSetR)


RCPP_MODULE(poset_module) {
    Rcpp::class_<POSetR>( "POSet" )
    .constructor<Rcpp::StringVector, Rcpp::StringMatrix, Rcpp::StringVector, Rcpp::StringMatrix, Rcpp::String>()
    .constructor<Rcpp::StringVector, Rcpp::StringMatrix>()
    .constructor<Rcpp::StringVector>()
    .method("comparabilities", &POSetR::comparabilities)
    .method("coverMatrix", &POSetR::CoverMatrix)
    .method("coveredBy", &POSetR::coveredBy)
    .method("downSet", &POSetR::downSet)
    .method("elements", &POSetR::Elements)
    .method("evaluation", &POSetR::evaluation)
    .method("incidenceMatrix", &POSetR::IncidenceMatrix)
    .method("incomparabilities", &POSetR::incomparabilities)
    .method("show", &POSetR::show)
    .method("latticeOfIdeals", &POSetR::latticeOfIdeals)
    .method("upSet", &POSetR::upSet)
    .method("firstLE", &POSetR::firstLE)
    ;
}
