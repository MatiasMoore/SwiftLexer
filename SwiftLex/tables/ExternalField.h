#pragma once
#include <string>
#include <vector>
#include "FieldAccessFlag.h"
#include "ConstantTable.h"

class ExprNode;

class ExternalField
{
public:
    ExternalField(std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags);

    std::string getVarName();
    std::string getDescriptor();
    std::string getClassName();
    std::vector<FieldAccessFlag> getFlags();
    bool isHasFlag(FieldAccessFlag flag);

    virtual ~ExternalField();

protected:
    int findOrAddFieldRef(ConstantTable* constTable);
    int addFieldRefToConstTable(ConstantTable* constTable);
    int findFieldRef(ConstantTable* constTable);
    std::string _varName;

    std::string _descriptor;

    std::string _className;

    std::vector<FieldAccessFlag> _flags;

    friend class InternalClass;
};