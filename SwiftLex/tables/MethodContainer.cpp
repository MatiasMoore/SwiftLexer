#include "MethodContainer.h"
#include "../generation/generationHelpers.h"
#include "../ExceptionHelper.h"

ExternalMethod* MethodContainer::overrideMethod(ExternalMethod* oldMethod, ExternalMethod* newMethod)
{
    if (this->findMethod(oldMethod->getMethodName(), oldMethod->getArgsDescriptor(), oldMethod->containsFlag(M_ACC_STATIC)) == nullptr)
        throw std::runtime_error("Failed to override! Method \"" + oldMethod->getMethodName() +
            "\" is not found!" + LINE_AND_FILE);

    this->_nameArgDescIsStaticToMethod[oldMethod->getMethodName()][oldMethod->getArgsDescriptor()][oldMethod->containsFlag(M_ACC_STATIC)] = newMethod;

    return newMethod;
}

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

std::vector<ExternalMethod*> MethodContainer::getMethodsWithNameAndParamCount(std::string methodName, int argCount)
{
    std::vector<ExternalMethod*> found = {};

    if (this->_nameArgDescIsStaticToMethod.count(methodName) == 0)
        return found;

    auto allMethods = this->getAll();
    for (auto& method : allMethods)
    {
        auto currentArgCount = method->getArgDescriptorsVector().size();
        if (currentArgCount == argCount && method->getMethodName() == methodName)
            found.push_back(method);
    }

    return found;
}
