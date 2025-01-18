#include "localVarTable.h"
#include <stdexcept>

LocalVariableElement* LocalVariableTable::addLocalVar(std::string name, std::string descriptor, ConstantTable* constantTable)
{
    if (items.find(name) != items.cend())
        throw std::runtime_error("Local variable with name " + name + " already exists!");

    int newLocalId = this->items.size();
    this->items[name] = new LocalVariableElement(newLocalId, name, descriptor, constantTable);
    return this->items[name];
}

LocalVariableElement* LocalVariableTable::findLocalVar(std::string name)
{
    if (items.find(name) == items.cend())
        throw std::runtime_error("Local variable with name \"" + name + "\" doesn't exist!");

    return this->items[name];
}

LocalVariableElement::LocalVariableElement(int localId, std::string name, std::string descriptor, ConstantTable* constantTable)
{
    this->name = name;
    this->_descriptor = descriptor;
    this->localId = localId;
    this->nameIndex = constantTable->findOrAddConstant(Utf8_C, name);
    this->descriptorIndex = constantTable->findOrAddConstant(Utf8_C, descriptor);
}