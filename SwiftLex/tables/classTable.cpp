#include "classTable.h"
#include "../ExceptionHelper.h"

ExternalField* ClassTable::findField(std::string name, std::string descriptor, std::string className)
{
	auto myClass = this->findClass(className);
	if (myClass != nullptr)
	{
		auto externalField = myClass->findField(name);
		if (externalField != nullptr && externalField->getDescriptor() == descriptor)
			return externalField;
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

ExternalClass::ExternalClass(std::string name, std::string baseName)
{
	this->_name = name;
	this->_baseName = baseName;
}

InternalClass::InternalClass(std::string name, std::string baseName) : ExternalClass(name, baseName)
{
	_constTable = new ConstantTable();
	_nameRef = _constTable->findOrAddUTF8(name);
	_baseRef = _constTable->findOrAddUTF8(baseName);
	_classRef = _constTable->findOrAddClassRef(_nameRef);
}

InternalMethod* InternalClass::addMethod(int accessFlag, std::string name, std::string descriptor, class StmtListNode* body)
{
	auto newMethod = new InternalMethod();
	newMethod->accessFlag = accessFlag;
	newMethod->_name = name;
	newMethod->_descriptor = descriptor;
	newMethod->_body = body;
	newMethod->_nameRef = _constTable->findOrAddUTF8(name);
	newMethod->_descriptorRef = _constTable->findOrAddUTF8(descriptor);
	auto nameAndTypeRef = _constTable->findOrAddNameAndType(newMethod->_nameRef, newMethod->_descriptorRef);
	newMethod->_methodRef = _constTable->findOrAddMethodRef(newMethod->_nameRef, this->_classRef);
	_methodMap[name] = newMethod;
	return newMethod;
}

InternalField* InternalClass::addInternalField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue)
{
	auto newField = new InternalField(this->_constTable, varName, descriptor, this->_name, flags, constValue);

	if (this->_fieldMap.count(varName) != 0)
		throw std::runtime_error("Field " + varName + " already exists in class " + this->_name + LINE_AND_FILE);

	this->_fieldMap[varName] = newField;

	return newField;
}

InternalField* InternalClass::addExternalClassFieldToConstantTable(ExternalField* externalField)
{
	std::string varName = externalField->getVarName();
	std::string className = externalField->getClassName();

	if (findExternalClassField(varName, className) != nullptr)
		throw std::runtime_error("Field " + varName + " already exists in class " + this->_name + LINE_AND_FILE);

	auto internalField = new InternalField(this->_constTable, externalField->getVarName(), externalField->getDescriptor(), externalField->getClassName(), externalField->getFlags(), externalField->getConstValue());

	_externalClassFieldMap.push_back(internalField);

	return internalField;
}

InternalField* InternalClass::findInternalField(std::string varName)
{
	auto field = this->findField(varName);

	auto internalField = dynamic_cast<InternalField*>(field);

	if (internalField == nullptr)
		std::runtime_error("Something went wrong! We should not be there! For field" + varName + " in class " + this->_name + LINE_AND_FILE);

	return internalField;
}

InternalField* InternalClass::findExternalClassField(std::string varName, std::string className)
{
	for (auto& externalField : this->_externalClassFieldMap)
	{
		if (externalField->getVarName() == varName && externalField->getClassName() == className)
			return externalField;
	}
}

ExternalMethod* ExternalClass::addMethod(std::string name, std::string descriptor)
{
	auto newMethod = new ExternalMethod();
	newMethod->_class = this;
	newMethod->_name = name;
	newMethod->_descriptor = descriptor;

	return newMethod;
}

ExternalField* ExternalClass::addField(std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags, ExprNode* constValue)
{
	if (this->_fieldMap.count(varName) != 0)
		throw std::runtime_error("Field " + varName + " already exists in class " + this->_name + LINE_AND_FILE);

	auto newField = new ExternalField(varName, descriptor, this->_name, flags, constValue);
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
