#include "MethodContainer.h"
#include "../generation/generationHelpers.h"
#include "../ExceptionHelper.h"

ExternalMethod* MethodContainer::findMethod(std::string name, std::string argDescriptor, bool isStatic)
{
    if (_nameArgDescIsStaticToMethod.count(name) == 0)
        return nullptr;

    if (_nameArgDescIsStaticToMethod[name].count(argDescriptor) == 0)
        return nullptr;

    if (_nameArgDescIsStaticToMethod[name][argDescriptor].count(isStatic) == 0)
        return nullptr;

    return _nameArgDescIsStaticToMethod[name][argDescriptor][isStatic];
}

ExternalMethod* MethodContainer::addMethod(ExternalMethod* method)
{
    auto methodName = method->getMethodName();
    auto argDesc = getArgDescFromFullDesc(method->getDescriptor());
    bool isStatic = method->containsFlag(MethodAccessFlag::M_ACC_STATIC);
    if (this->findMethod(methodName, argDesc, isStatic) != nullptr)
    {
        auto prefix = std::string(isStatic ? "Static" : "Non-static");
        throw std::runtime_error(prefix + " method \"" + methodName + "\" with arg descriptor \"" + argDesc + "\" already exists!" + LINE_AND_FILE);
    }

    _nameArgDescIsStaticToMethod[methodName][argDesc][isStatic] = method;
    return method;
}

std::vector<ExternalMethod*> MethodContainer::getAll()
{
    std::vector<ExternalMethod*> allMethods = {};

    for (auto& nameMapPair : this->_nameArgDescIsStaticToMethod)
    {
        auto nameMap = nameMapPair.second;
        for (auto& argMapPair : nameMap)
        {
            auto argMap = argMapPair.second;
            for (auto& methodPair : argMap)
            {
                auto method = methodPair.second;
                allMethods.push_back(method);
            }
        }
    }

    return allMethods;
}
