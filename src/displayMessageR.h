#ifndef displayMessageR_hpp
#define displayMessageR_hpp

#include <memory>
#include <Rcpp.h>
#include <cstdint>


#include "displayMessage.h"

class DisplayMessageR : public DisplayMessage {
private:
    std::shared_ptr<Rcpp::Function> pb_update_function;
    std::uint64_t progress_update;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_output = start;
    std::shared_ptr<std::uint64_t> number_of_extension;
public:
    DisplayMessageR(std::shared_ptr<std::map<std::string, ParamType>> p) : DisplayMessage(p) {
        this->pb_update_function = nullptr;
        if ((*this->parms).find("ProgressBarUpdate") != (*this->parms).end()) {
            if (auto* p = std::get_if<std::shared_ptr<Rcpp::Function>>(&(*this->parms)["ProgressBarUpdate"])) {
                this->pb_update_function = *p;
            } else {
              std::string err_str = "DisplayMessageR error: ProgressBarUpdate";
              throw_line(err_str);
            }
        }
        this->number_of_extension = nullptr;
        if ((*this->parms).find("NumberExtension") != (*this->parms).end()) {
            //this->number_of_extension = std::get<std::shared_ptr<std::uint64_t>>((*this->parms)["NumberExtension"]);
            if (auto* p = std::get_if<std::shared_ptr<std::uint64_t>>(&(*this->parms)["NumberExtension"])) {
                this->number_of_extension = *p;
            } else {
              std::string err_str = "DisplayMessageR error: NumberExtension";
              throw_line(err_str);
            }
        }
    }
    
    virtual void Display() {
         if (this->pb_update_function == nullptr) return;
         auto intevallo = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - last_output);
         if (last_output == start || ((unsigned long) intevallo.count()) > progress_update) {
             (*this->pb_update_function)(*number_of_extension);
             last_output = std::chrono::high_resolution_clock::now();
         }
    }
    
    virtual void Start() {
        this->progress_update = 1;
        this->start = std::chrono::high_resolution_clock::now();
        this->last_output = start;
    };
    virtual void Stop() {
        if (this->pb_update_function == nullptr) return;
        (*this->pb_update_function)(*number_of_extension);
    };
    virtual std::string to_string() const
    {
        std::string result = "";
        result += "Calls: " + std::to_string(this->calls);
        return result;
    }
};

#endif /* displayMessageR_hpp */
