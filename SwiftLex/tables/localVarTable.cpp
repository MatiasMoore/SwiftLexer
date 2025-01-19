#include "localVarTable.h"
#include <stdexcept>

LocalVariableElement* LocalVariableTable::addLocalVar(std::string name, std::string descriptor)
{
    if (items.find(name) != items.cend())
        throw std::runtime_error("Local variable with name " + name + " already exists!");

    int newLocalId = this->items.size();
    this->items[name] = new LocalVariableElement(newLocalId, name, descriptor);
    return this->items[name];
}

LocalVariableElement* LocalVariableTable::addLocalVarToConstantTable(std::string name, std::string descriptor, ConstantTable* constantTable)
{
    if (items.find(name) != items.cend())
        throw std::runtime_error("Local variable with name " + name + " already exists!");

    int newLocalId = this->items.size();
    this->items[name] = new LocalVariableElement(newLocalId, name, descriptor);
    this->items[name]->toConstantTable(constantTable);
    return this->items[name];
}

LocalVariableElement* LocalVariableTable::findLocalVar(std::string name)
{
    if (items.find(name) == items.cend())
        throw std::runtime_error("Local variable with name \"" + name + "\" doesn't exist!");

    return this->items[name];
}

LocalVariableElement::LocalVariableElement(int localId, std::string name, std::string descriptor)
{
    this->name = name;
    this->_descriptor = descriptor;
    this->localId = localId;
    this->nameIndex = -1;
    this->descriptorIndex = -1;
}

int LocalVariableElement::toConstantTable(ConstantTable* constantTable)
{
    this->nameIndex = constantTable->findOrAddUTF8(this->name);
    this->descriptorIndex = constantTable->findOrAddUTF8(this->_descriptor);
}
