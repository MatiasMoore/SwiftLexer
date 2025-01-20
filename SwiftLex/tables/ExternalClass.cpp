#include "ExternalClass.h"
#include "../ExceptionHelper.h"
#include "../generation/generationHelpers.h"

ExternalClass::ExternalClass(std::string name, std::string baseName)
{
	this->_name = name;
	this->_baseName = baseName;
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
	if (this->_fieldMap.count(varName) != 0)
		throw std::runtime_error("Field " + varName + " already exists in class " + this->_name + LINE_AND_FILE);

	auto newField = new ExternalField(varName, descriptor, getClassName(), flags);
	this->_fieldMap[varName] = newField;
	return newField;
}

ExternalField* ExternalClass::findField(std::string varName)
{
	if (this->_fieldMap.count(varName) == 0)
		return nullptr;
	return this->_fieldMap[varName];
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
