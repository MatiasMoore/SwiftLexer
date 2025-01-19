#pragma once
#include <string>
#include <vector>
#include "LocalVariableTable.h"
#include "ExternalMethod.h"

class StmtListNode;

class InternalMethod : public ExternalMethod
{
public:
    InternalMethod(ConstantTable* constTable, StmtListNode* body, std::string methodName, std::string descriptor, std::string className, std::vector<MethodAccessFlag> flags);
    
    bool isStatic();
    class LocalVariableTable* getVarTable();

    int _methodRef;
    int _nameRef;
    int _descriptorRef;
    int _classRef;
    int _nameAndTypeRef;

    class StmtListNode* _body;

private:
    class LocalVariableTable* _varTable;
};
