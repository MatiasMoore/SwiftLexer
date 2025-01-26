#include "VariableScope.h"
#include "../ExceptionHelper.h"
#include "LocalVariableTable.h"

VariableScope::VariableScope(LocalVariableTable* parentTable)
{
    _parentTable = parentTable;
    _parentScope = nullptr;
}

VariableScope::VariableScope(VariableScope* parentScope)
{
    if (parentScope == nullptr)
        throw std::runtime_error("Critical error! Null parent scope in constructor of new scope!" + LINE_AND_FILE);
    _parentScope = parentScope;
    _parentScope->addChildScope(this);
    _parentTable = _parentScope->_parentTable;
}

void VariableScope::addChildScope(VariableScope* childScope)
{
    this->_children.push_back(childScope);
}

LocalVariableElement* VariableScope::addLocalVar(std::string name, std::string descriptor, bool isConst)
{
    if (_parentTable == nullptr)
        throw std::runtime_error("Critical error! Null parent table while adding new local var!" + LINE_AND_FILE);

    if (this->_nameToVar.count(name) != 0)
        throw std::runtime_error("Local variable with name " + name + " already exists!" + LINE_AND_FILE);

    int newLocalId = _parentTable->getNewLocalId();
    this->_nameToVar[name] = new LocalVariableElement(newLocalId, name, descriptor, isConst);
    return this->_nameToVar[name];
}

LocalVariableElement* VariableScope::findLocalVar(std::string name)
{
    if (_nameToVar.find(name) == _nameToVar.cend())
    {
        if (_parentScope == nullptr)
            return nullptr;
        else
            return _parentScope->findLocalVar(name);
    }
    else
        return this->_nameToVar[name];
}
