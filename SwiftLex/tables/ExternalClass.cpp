#include "ExternalClass.h"
#include "../ExceptionHelper.h"
#include "../generation/generationHelpers.h"

ExternalClass::ExternalClass(std::string name, std::string baseName)
{
	this->_name = name;
	this->_baseName = baseName;
}

ExternalMethod* ExternalClass::overrideMethod(ExternalMethod* oldMethod, ExternalMethod* newMethod)
{
	if (this->findMethod(oldMethod->getMethodName(), oldMethod->getArgsDescriptor(), oldMethod->containsFlag(M_ACC_STATIC)) == nullptr)
		throw std::runtime_error("Failed to override! Method \"" + oldMethod->getMethodName() + 
			"\" is not found in class \"" + this->getClassName() + "\"!" + LINE_AND_FILE);

	return this->_methodContainer.overrideMethod(oldMethod, newMethod);
}

ExternalMethod* ExternalClass::addMethod(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags)
{
	auto newMethod = new ExternalMethod(methodName, descriptor, getClassName(), flags);
	this->_methodContainer.addMethod(newMethod);
	return newMethod;
}

ExternalMethod* ExternalClass::findMethod(std::string name, std::string argDescriptor, bool isStatic)
{
	return this->_methodContainer.findMethod(name, argDescriptor, isStatic);
}

ExternalField* ExternalClass::addField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags)
{
	auto newField = new ExternalField(varName, descriptor, getClassName(), flags);
	this->_fieldContainer.addField(newField);
	return newField;
}

ExternalField* ExternalClass::findField(std::string varName, bool isStatic)
{
	return this->_fieldContainer.findField(varName, isStatic);
}

std::vector<ExternalMethod*> ExternalClass::getMethodsWithNameAndParamCount(std::string methodName, int argCount)
{
	return this->_methodContainer.getMethodsWithNameAndParamCount(methodName, argCount);
}

std::vector<ExternalMethod*> ExternalClass::getMethods()
{
	return this->_methodContainer.getAll();
}

std::vector<ExternalField*> ExternalClass::getFields()
{
	return this->_fieldContainer.getAll();
}

std::string ExternalClass::getClassName()
{
	return _name;
}

std::string ExternalClass::getBaseClassName()
{
	return _baseName;
}

ExternalClass::~ExternalClass()
{
}
