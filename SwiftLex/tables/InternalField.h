#pragma once
#include <string>
#include <map>
#include <vector>
#include "ExternalField.h"
#include "ConstantTable.h"

class ExprNode;

class InternalField : public ExternalField
{
public:
    InternalField(ConstantTable* constantTable, std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr);
    
    int accessFlagsToInt();

    bool isStatic();

    int _fieldRef;
    int _nameRef;
    int _descriptorRef;
    int _classRef;
    int _nameAndTypeRef;
private:
    std::vector<FieldAccessFlag> _flags;

};
