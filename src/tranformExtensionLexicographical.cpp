#include "tranformExtension.h"


// ***********************************************
// ***********************************************
// ***********************************************

TELexicographical::TELexicographical(std::shared_ptr<std::map<std::string, ParamType>> p) : TranformExtension(p) {
    this->calls = 0;
    //this->modalities = std::get<std::shared_ptr<std::vector<std::uint64_t>>>((*this->parms)["TEModalities"]);
    if (auto* p = std::get_if<std::shared_ptr<std::vector<std::uint64_t>>>(&(*this->parms)["TEModalities"])) {
        this->modalities = *p;
    } else {
      std::string err_str = "TELexicographical error: TEModalities";
      throw_line(err_str);
    }
    
    this->total_elements = 1;
    for (std::uint64_t k = 0; k < this->modalities->size(); ++k) {
        this->total_elements *= this->modalities->at(k);
    }
    
    this->result_vector.resize(this->total_elements);
    for (std::uint64_t k = 0; k < this->result_vector.size(); ++k) {
        this->result_vector.at(k).resize(this->modalities->size());
    }
    
    this->pos_labels = std::make_shared<std::map<std::uint64_t, std::string>>();
    this->labels_pos = std::make_shared<std::map<std::string, std::uint64_t>>();
    
    std::uint64_t change_at = this->total_elements;
    for (std::uint64_t k = 0; k < this->modalities->size(); ++k) {
        std::uint64_t m_size = this->modalities->at(k);
        change_at = change_at / m_size;
        for (std::uint64_t h = 0, c = 0; h < this->result_vector.size(); ++h, ++c) {
            std::uint64_t val_m_p = (c / change_at) % m_size;
            this->result_vector.at(h).at(k) = std::to_string(val_m_p);
        }
    }
    for (std::uint64_t k = 0; k < this->result_vector.size(); ++k) {
        std::string value = "";
        for (std::uint64_t h = 0; h < this->result_vector.at(k).size(); ++h) {
            if (h < this->result_vector.at(k).size() - 1)
                value += this->result_vector.at(k).at(h) + "-";
            else
                value += this->result_vector.at(k).at(h);
        }
        (*this->pos_labels)[k] = value;
        (*this->labels_pos)[value] = k;
    }

    
}

// ***********************************************
// ***********************************************
// ***********************************************

std::shared_ptr<LinearExtension> TELexicographical::operator()(std::shared_ptr<LinearExtension> x) {
    std::uint64_t change_at = this->total_elements;
    for (std::uint64_t k = 0; k < this->modalities->size(); ++k) {
        std::uint64_t m = x->getVal(k);
        std::uint64_t m_size = this->modalities->at(m);
        change_at = change_at / m_size;
        
        for (std::uint64_t h = 0, c = 0; h < this->result_vector.size(); ++h, ++c) {
            std::uint64_t val_m_p = (c / change_at) % m_size;
            //std::string val_m = this->modalities->at(m).at(val_m_p);
            //this->result_vector.at(h).at(m) = val_m;
            this->result_vector.at(h).at(m) = std::to_string(val_m_p);

        }
    }
    
    std::shared_ptr<LinearExtension> result = std::make_shared<LinearExtension>(this->result_vector.size());
    for (std::uint64_t k = 0; k < this->result_vector.size(); ++k) {
        std::string value = "";
        for (std::uint64_t h = 0; h < this->result_vector.at(k).size(); ++h) {
            if (h < this->result_vector.at(k).size() - 1)
                value += this->result_vector.at(k).at(h) + "-";
            else
                value += this->result_vector.at(k).at(h);
        }
        std::uint64_t pos = (*this->labels_pos)[value];

        result->set(k, pos);
    }
    return result;
}

// ***********************************************
// ***********************************************
// ***********************************************
