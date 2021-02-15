//

#ifndef MyException_h
#define MyException_h


#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class my_exception : public std::runtime_error {
    std::string msg;
public:
    my_exception(const std::string &arg, const char *file, int line) :
    std::runtime_error(arg) {
        std::ostringstream o;
        o << file << ":" << line << ": " << arg;
        msg = o.str();
    }
    ~my_exception() throw() {}
    const char *what() const throw() {
        return msg.c_str();
    }
};



#endif /* MyException_h */
