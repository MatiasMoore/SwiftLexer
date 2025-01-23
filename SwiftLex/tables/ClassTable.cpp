#include "ClassTable.h"
#include "../ExceptionHelper.h"

bool ClassTable::isClassDerivedFromClass(std::string derivedName, std::string parentName)
{
	auto derivedClass = this->findClass(derivedName);
	if (derivedClass == nullptr)
		return false;

	if (derivedClass->getBaseClassName() == parentName)
		return true;

	return this->isClassDerivedFromClass(derivedClass->getBaseClassName(), parentName);
}

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
	auto baseClass = this->findClass(baseName);
	if (baseClass == nullptr)
		std::runtime_error("Error adding class \"" + name + "\"! It's base class \"" + baseName + "\" doesn't exist!");

	for (auto& baseClassMethod : baseClass->getMethods())
	{
		//FIXME Skip constructors for now
		if (baseClassMethod->getMethodName() == "<init>")
			continue;

		//Don't inherit private methods
		if (baseClassMethod->containsFlag(MethodAccessFlag::M_ACC_PRIVATE))
			continue;

		newClass->addMethod(baseClassMethod->getMethodName(), baseClassMethod->getDescriptor(), baseClassMethod->getFlags());
	}

	for (auto& baseClassField : baseClass->getFields())
	{
		//Don't inherit private fields
		if (baseClassField->containsFlag(FieldAccessFlag::F_ACC_PRIVATE))
			continue;

		newClass->addField(baseClassField->getVarName(), baseClassField->getDescriptor(), baseClassField->getFlags());
	}

	_classes[name] = newClass;
	return newClass;
}

ExternalClass* ClassTable::addExternalClass(std::string name, std::string baseName)
{
	if (this->_classes.count(name) != 0)
		throw std::runtime_error("Class " + name + " already exists!" + LINE_AND_FILE);

	//TODO copy methods from base class, same as for internal classes

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