#pragma once
#include <iostream>
#include <string>
#include <map>
#include "tables/tables.h"
#include "ExceptionHelper.h"

class RTLHelper {
public:
    // Default class and main function
    inline static std::string _defaultMainFunc = "main";
    inline static std::string _defaultMainClass = "MainClass";

    // Class names
    inline static std::string _baseC = "java/lang/Object";

    inline static std::string _IOC = "rtl/InputOutput";

    inline static std::string _boolC = "rtl/Boolean";
    inline static std::string _floatC = "rtl/Float";
    inline static std::string _intC = "rtl/Integer";
    inline static std::string _strC = "rtl/String";

    inline static std::string _arrayC = "rtl/Array";
    inline static std::string _rangeC = "rtl/Range";

    // Property names
    inline static std::string _propertyCount = "count";

    inline static std::string _propertyIsEmpty = "isEmpty";

    // Method names
    inline static std::string _sum = "sum";
    inline static std::string _sub = "sub";
    inline static std::string _mul = "mul";
    inline static std::string _div = "div";

    inline static std::string _print = "print";
    inline static std::string _readLine = "readLine";

    inline static std::string _toBool = "toBool";

    inline static std::string _logAnd = "logAnd";
    inline static std::string _logOr = "logOr";
    inline static std::string _logNot = "logNot";

    inline static std::string _lt = "lt";

    inline static std::string _toArray = "toArray";

    // Generate a unique id for internally created variables (e.g. during tree transform)
    static std::string getUniqueInternalVarName();
};

