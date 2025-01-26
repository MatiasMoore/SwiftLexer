#include "LocalVariableTable.h"
#include "../ExceptionHelper.h"
#include "VariableScope.h"

LocalVariableElement::LocalVariableElement(int localId, std::string name, std::string descriptor, bool isConst)
{
    this->name = name;
    this->_descriptor = descriptor;
    this->localId = localId;
    this->nameIndex = -1;
    this->descriptorIndex = -1;
    this->isConst = isConst;
}

int LocalVariableTable::getNewLocalId()
{
    int curr = _newLocalId;
    _newLocalId++;
    return curr;
}

VariableScope* LocalVariableTable::getRootScope()
{
    if (_variableScopes.size() == 0)
        throw std::runtime_error("Critical error! Can't find a root scope!" + LINE_AND_FILE);

    return _variableScopes[0];
}

VariableScope* LocalVariableTable::createRootVariableScope()
{
    if (_variableScopes.size() != 0)
        throw std::runtime_error("Critical error! Can't create another root variable scope!" + LINE_AND_FILE);

    auto newScope = new VariableScope(this);
    _variableScopes.push_back(newScope);
    return newScope;
}

VariableScope* LocalVariableTable::createVariableScope(VariableScope* parent)
{
    if (_variableScopes.size() == 0)
        throw std::runtime_error("Critical error! Can't create another variable scope without a parent scope!" + LINE_AND_FILE);

    if (std::find(_variableScopes.cbegin(), _variableScopes.cend(), parent) == _variableScopes.cend())
        throw std::runtime_error("Critical error! Can't create another variable scope with parent scope from a different var table!" + LINE_AND_FILE);

    auto newScope = new VariableScope(parent);
    _variableScopes.push_back(newScope);
    return newScope;
}

int LocalVariableTable::getTotalVarCount()
{
    return _newLocalId;
}
