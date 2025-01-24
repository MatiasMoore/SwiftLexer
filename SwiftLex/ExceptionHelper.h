#pragma once
#include <stdexcept>

#ifdef  _DEBUG
#define LINE_AND_FILE std::string("\n") + std::string(" error occured at line ") + std::to_string(__LINE__) + std::string(" in file ") + std::string(__FILE__) + std::string("\n\n")
#else
#define LINE_AND_FILE std::string("")

#endif //  _DEBUG

