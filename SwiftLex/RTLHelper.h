#pragma once
#include <iostream>
#include <string>
#include <map>
#include "tables/tables.h"
#include "ExceptionHelper.h"

#define D(c) std::string("L") + std::string(c) + std::string(";") 

class RTLHelper {
public:
    static void addRTLToClassTable(ClassTable* classTable);

    // Class names
    inline static std::string _baseC = "java/lang/Object";

    inline static std::string _IOC = "rtl/InputOutput";

    inline static std::string _boolC = "rtl/Boolean";
    inline static std::string _floatC = "rtl/Float";
    inline static std::string _intC = "rtl/Integer";
    inline static std::string _strC = "rtl/String";

    // Method names
    inline static std::string _constructor = "<init>";
    inline static std::string _sum = "sum";
    inline static std::string _sub = "sub";
    inline static std::string _mul = "mul";
    inline static std::string _div = "div";
    inline static std::string _print = "print";
    inline static std::string _readLine = "readLine";

    // Flags
    inline static std::vector<MethodAccessFlag> _publicFlags = { MethodAccessFlag::M_ACC_PUBLIC };
    inline static std::vector<MethodAccessFlag> _publicStaticFlags = { MethodAccessFlag::M_ACC_PUBLIC, MethodAccessFlag::M_ACC_STATIC };

    struct MethodInfo {
        std::string name;
        std::string descriptor;
    };

    // Methods to add
    inline static std::map<std::string, std::vector<MethodInfo>> _nonStaticMethods = {
        {_baseC, {
			{_constructor, "()V"}
		}},
        {_intC, {
            {_constructor, "(I)V"},
            {_sum, "(" + D(_intC) + ")" + D(_intC)},
            {_sub, "(" + D(_intC) + ")" + D(_intC)},
        	{_mul, "(" + D(_intC) + ")" + D(_intC)},
			{_div, "(" + D(_intC) + ")" + D(_intC)}, //FIXME
        }},
        {_strC, {
            {_constructor, "(Ljava/lang/String;)V"},
            {_sum, "(" + D(_strC) + ")" + D(_strC)}
        }},
        {_boolC, {
			{_constructor, "(Z)V"}
		}},

    };

private:
    inline static std::map<std::string, std::vector<MethodInfo>> _staticMethods = {
        {_IOC, {
            {_print, "(" + D(_intC) + ")V"},
            {_print, "(" + D(_strC) + ")V"},
			{_print, "(" + D(_floatC) + ")V"},
            {_print, "(" + D(_boolC) + ")V"},
			{_readLine, "()" + D(_strC)}
        }}
    };

    static void addExternalMethod(ClassTable* classTable, std::string className, MethodInfo methodInfo, std::vector<MethodAccessFlag> flags);
};

