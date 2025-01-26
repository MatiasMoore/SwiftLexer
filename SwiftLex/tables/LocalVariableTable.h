#pragma once
#include <map>
#include <string>
#include "ConstantTable.h"
#include <vector>

class VariableScope;

/*! \brief Таблица локальных переменных. */
class LocalVariableTable
{
private:
    int _newLocalId = 0;
    int getNewLocalId();
    std::vector<VariableScope*> _variableScopes = {};
public:

    VariableScope* getRootScope();

    VariableScope* createRootVariableScope();

    VariableScope* createVariableScope(VariableScope* parent);

    int getTotalVarCount();

    friend class VariableScope;
};

class LocalVariableElement
{
public:
    std::string name; // Имя локальной переменной.
    std::string _descriptor; // Тип локальной переменной.
    int localId;

    int start_pc = 0;
    int length = 9;
    int nameIndex;
    int descriptorIndex;
    bool isConst;

    LocalVariableElement(int localId, std::string name, std::string descriptor, bool isConst);
};
