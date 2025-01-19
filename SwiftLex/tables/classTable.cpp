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

ExternalMethod* ClassTable::findMethod(std::string name, std::string descriptor, std::string className)
{
	auto myClass = this->findClass(className);
	if (myClass != nullptr)
	{
		auto externalMethod = myClass->findMethod(name);
		if (externalMethod != nullptr && externalMethod->getDescriptor() == descriptor)
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

InternalMethod* InternalClass::addInternalMethodToConstantTable(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags, StmtListNode* body, LocalVariableTable* varTable)
{
	auto newMethod = new InternalMethod(_constTable, body, methodName, descriptor, getClassName(), flags, varTable);

	if (this->_methodMap.count(methodName) != 0)
		throw std::runtime_error("Method " + methodName + " already exists in class " + getClassName() + LINE_AND_FILE);

	this->_methodMap[methodName] = newMethod;

	return newMethod;
}

InternalMethod* InternalClass::addExternalClassMethodToConstantTable(ExternalMethod* externalMethod)
{
	std::string methodName = externalMethod->getMethodName();
	std::string className = externalMethod->getClassName();

	if (findExternalClassMethod(methodName, className) != nullptr)
		throw std::runtime_error("Method " + methodName + " already exists in class " + getClassName() + LINE_AND_FILE);

	auto internalMethod = new InternalMethod(
		_constTable, 
		nullptr, 
		externalMethod->getMethodName(), 
		externalMethod->getDescriptor(), 
		externalMethod->getClassName(), 
		externalMethod->getFlags(), 
		externalMethod->getVarTable()
	);

	_externalClassMethodMap.push_back(internalMethod);

	return internalMethod;
}

InternalMethod* InternalClass::findInternalMethod(std::string methodName)
{
	if (this->_methodMap.count(methodName) == 0)
		return nullptr;
	return dynamic_cast<InternalMethod*>(this->_methodMap[methodName]);
}

InternalMethod* InternalClass::findExternalClassMethod(std::string methodName, std::string className)
{
	for (auto& externalField : this->_externalClassMethodMap)
	{
		if (externalField->getMethodName() == methodName && externalField->getClassName() == className)
			return externalField;
	}
	return nullptr;
}

InternalField* InternalClass::addInternalFieldToConstantTable(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue)
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
	return nullptr;
}

ExternalMethod* ExternalClass::addMethod(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags, LocalVariableTable* varTable)
{
	if (this->_methodMap.count(methodName) != 0)
		throw std::runtime_error("Method " + methodName + " already exists in class " + this->_name + LINE_AND_FILE);
	
	auto newMethod = new ExternalMethod(methodName, descriptor, getClassName(), flags, varTable);
	this->_methodMap[methodName] = newMethod;
	return newMethod;
}

ExternalMethod* ExternalClass::findMethod(std::string name)
{
	if (this->_methodMap.count(name) == 0)
		return nullptr;
	return this->_methodMap[name];
}

ExternalField* ExternalClass::addField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue)
{
	if (this->_fieldMap.count(varName) != 0)
		throw std::runtime_error("Field " + varName + " already exists in class " + this->_name + LINE_AND_FILE);

	auto newField = new ExternalField(varName, descriptor, getClassName(), flags, constValue);
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
