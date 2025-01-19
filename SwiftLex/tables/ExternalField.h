#pragma once
#include <string>
#include <vector>
#include "FieldAccessFlag.h"
#include "ConstantTable.h"

class ExprNode;

class ExternalField
{
public:
    ExternalField(std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr);

    std::string getVarName();
    std::string getDescriptor();
    std::string getClassName();
    std::vector<FieldAccessFlag> getFlags();
    ExprNode* getConstValue();

    virtual ~ExternalField();

protected:
    int findOrAddFieldRef(ConstantTable* constTable);
    int addFieldRefToConstTable(ConstantTable* constTable);
    int findFieldRef(ConstantTable* constTable);
    std::string _varName;

    std::string _descriptor;

    std::string _className;

    std::vector<FieldAccessFlag> _flags;

    ExprNode* _constValue;

    friend class InternalClass;
};