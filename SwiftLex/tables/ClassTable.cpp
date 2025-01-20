#include "ClassTable.h"
#include "../ExceptionHelper.h"

ExternalField* ClassTable::findField(std::string name, std::string className, bool isStatic)
{
	auto myClass = this->findClass(className);
	if (myClass != nullptr)
	{
		auto externalField = myClass->findField(name, isStatic);
		if (externalField != nullptr)
			return externalField;
	}
	return nullptr;
}

ExternalMethod* ClassTable::findMethod(std::string name, std::string argDescriptor, std::string className, bool isStatic)
{
	auto myClass = this->findClass(className);
	if (myClass != nullptr)
	{
		auto externalMethod = myClass->findMethod(name, argDescriptor, isStatic);
		if (externalMethod != nullptr)
			return externalMethod;
	}
	return nullptr;
}

ExternalClass* ClassTable::findClass(std::string name)
{
	if (this->_classes.count(name) == 0)
		return nullptr;
	return this->_classes[name];
}

InternalClass* ClassTable::addInternalClass(std::string name, std::string baseName)
{
	if (this->_classes.count(name) != 0)
		throw std::runtime_error("Class " + name + " already exists!" + LINE_AND_FILE);

	auto newClass = new InternalClass(name, baseName);
	// Add all methods and field from base class
	//TODO fields
	auto baseClass = this->findClass(baseName);
	if (baseClass == nullptr)
		std::runtime_error("Error adding class \"" + name + "\"! It's base class \"" + baseName + "\" doesn't exist!");

	for (auto& baseClassMethod : baseClass->getMethods())
	{
		if (baseClassMethod->getMethodName() != "<init>")
			newClass->addMethod(baseClassMethod->getMethodName(), baseClassMethod->getDescriptor(), baseClassMethod->getFlags());
	}

	_classes[name] = newClass;
	return newClass;
}

ExternalClass* ClassTable::addExternalClass(std::string name, std::string baseName)
{
	if (this->_classes.count(name) != 0)
		throw std::runtime_error("Class " + name + " already exists!" + LINE_AND_FILE);

	auto newClass = new ExternalClass(name, baseName);
	_classes[name] = newClass;
	return newClass;
}

InternalClass* ClassTable::findInternalClass(std::string name)
{
	return dynamic_cast<InternalClass*>(findClass(name));
}

ExternalClass* ClassTable::findNotInternalClass(std::string name)
{
	auto myClass = findClass(name);
	InternalClass* internalClass = dynamic_cast<InternalClass*>(myClass);
	if (internalClass == nullptr)
		return myClass;
	return nullptr;
}

std::vector<InternalClass*> ClassTable::getInternalClasses()
{
	std::vector<InternalClass*> internalClasses;
	for (auto& classPair : this->_classes)
	{
		auto internalClass = dynamic_cast<InternalClass*>(classPair.second);
		if (internalClass != nullptr)
			internalClasses.push_back(internalClass);
	}
	return internalClasses;
}