
#ifndef tranformExtension_hpp
#define tranformExtension_hpp

#include <memory>
#include <map>
#include <vector>

#include "paramType.h"
#include "linearExtension.h"

// ***********************************************
// ***********************************************
// ***********************************************

class TranformExtension {
protected:
    std::uint64_t calls;
    std::shared_ptr<std::map<std::string, ParamType>> parms;
    std::shared_ptr<std::map<std::uint64_t, std::string>> pos_labels;
    std::shared_ptr<std::map<std::string, std::uint64_t>> labels_pos;
public:
    TranformExtension(std::shared_ptr<std::map<std::string, ParamType>> p) : parms(p) {
        //this->pos_labels = std::get<std::shared_ptr<std::map<std::uint64_t, std::string>>>((*this->parms)["TEElements"]);
        if (auto* p = std::get_if<std::shared_ptr<std::map<std::uint64_t, std::string>>>(&(*this->parms)["TEElements"])) {
            this->pos_labels = *p;
        } else {
          std::string err_str = "TranformExtension error: TEElements";
          throw_line(err_str);
        }
    }
    virtual ~TranformExtension() {};
    std::shared_ptr<std::map<std::uint64_t, std::string>> Elements() {
        return this->pos_labels;
    }
    virtual std::shared_ptr<LinearExtension> operator()(std::shared_ptr<LinearExtension> x) = 0;
    virtual std::string to_string() const = 0;
};


// ***********************************************
// ***********************************************
// ***********************************************

class TEItentity : public TranformExtension {
protected:
public:
    TEItentity(std::shared_ptr<std::map<std::string, ParamType>> p);
    virtual ~TEItentity() {};
    std::shared_ptr<LinearExtension> operator()(std::shared_ptr<LinearExtension> x);
    virtual std::string to_string() const {return "";};
};

// ***********************************************
// ***********************************************
// ***********************************************

class TELexicographical : public TranformExtension { 
private:
    std::shared_ptr<std::vector<std::uint64_t>> modalities;
    std::uint64_t total_elements;
    std::vector<std::vector<std::string>> result_vector;

public:
    TELexicographical(std::shared_ptr<std::map<std::string, ParamType>> p);
    virtual ~TELexicographical() {};
    std::shared_ptr<LinearExtension> operator()(std::shared_ptr<LinearExtension> x);
    virtual std::string to_string() const {return "";};
};

// ***********************************************
// ***********************************************
// ***********************************************

#endif /* tranformEstention_hpp */
