#include "FieldContainer.h"
#include "../ExceptionHelper.h"

ExternalField* FieldContainer::findField(std::string name, bool isStatic)
{
    if (_nameIsStaticToField.count(name) == 0)
        return nullptr;

    if (_nameIsStaticToField[name].count(isStatic) == 0)
        return nullptr;

    return _nameIsStaticToField[name][isStatic];
}

ExternalField* FieldContainer::addField(ExternalField* field)
{
    auto fieldName = field->getVarName();
    bool isStatic = field->containsFlag(FieldAccessFlag::F_ACC_STATIC);
    if (this->findField(fieldName, isStatic) != nullptr)
    {
        auto prefix = std::string(isStatic ? "Static" : "Non-static");
        throw std::runtime_error(prefix + " field \"" + fieldName + "\" in class \"" + field->getClassName() + "\" already exists!" + LINE_AND_FILE);
    }

    _nameIsStaticToField[fieldName][isStatic] = field;
    return field;
}

std::vector<ExternalField*> FieldContainer::getAll()
{
    std::vector<ExternalField*> allFields = {};

    for (auto& nameMapPair : this->_nameIsStaticToField)
    {
        auto nameMap = nameMapPair.second;
        for (auto& staticMapPair : nameMap)
        {
            auto field = staticMapPair.second;
            allFields.push_back(field);
        }
    }

    return allFields;
}
