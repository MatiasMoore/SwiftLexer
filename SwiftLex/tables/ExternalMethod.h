#pragma once
#include <string>
#include <vector>
#include "MethodAccessFlag.h"
#include "ConstantTable.h"

class ExternalMethod
{
public:
    ExternalMethod(std::string methodName, std::string descriptor, std::string className, std::vector<MethodAccessFlag> flags);

    std::string getMethodName();
    std::string getDescriptor();
    std::string getClassName();
    std::vector<MethodAccessFlag> getFlags();

    virtual ~ExternalMethod();

protected:
    int addMethodRefToConstTable(ConstantTable* constTable);
    int findMethodRef(ConstantTable* constTable);
    int findOrAddMethodRef(ConstantTable* constTable);
    std::string _methodName;
    std::string _descriptor;
    std::string _className;
    std::vector<MethodAccessFlag> _flags;

    friend class InternalClass;
};
