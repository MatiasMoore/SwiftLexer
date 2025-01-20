#pragma once
#include <stdexcept>
#define LINE_AND_FILE std::string("\n") + std::string(" error occured at line ") + std::to_string(__LINE__) + std::string(" in file ") + std::string(__FILE__) + std::string("\n\n")
