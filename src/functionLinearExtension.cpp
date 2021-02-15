#include "functionLinearExtension.h"

//************************************
//************************************
//************************************

double FunctionLinearExtension::ERRORE = 1.0e-4;

std::map<std::string, FunctionLinearExtension* (*)(std::shared_ptr<std::map<std::string, void*>> p)> FunctionLinearExtension::map_type;

//************************************
//************************************
//************************************
