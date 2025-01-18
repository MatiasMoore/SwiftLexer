#include "classTable.h"

InternalClass::InternalClass()
{
	_constTable = new ConstantTable();
}

InternalClass* ClassTable::addInternalClass(std::string name, std::string baseName)
{
	if (this->_internalClasses.count(name) != 0)
		throw std::runtime_error("Class " + name + " already exists!" + LINE_AND_FILE);

	auto newClass = new InternalClass();
	auto constTable = newClass->_constTable;
	newClass->_name = name;
	newClass->_baseName = baseName;
	newClass->_nameRef = constTable->findOrAddConstant(Utf8_C, name);
	newClass->_baseRef = constTable->findOrAddConstant(Utf8_C, baseName);
	newClass->_classRef = constTable->findOrAddConstant(Class_C, "", 0, 0, newClass->_nameRef);
	_internalClasses[name] = newClass;
	return newClass;
}

ExternalClass* ClassTable::addExternalClass(std::string name, std::string baseName)
{
	if (this->_externalClasses.count(name) != 0)
		throw std::runtime_error("Class " + name + " already exists!" + LINE_AND_FILE);

	auto newClass = new ExternalClass();
	newClass->_name = name;
	newClass->_baseName = baseName;
	_externalClasses[name] = newClass;
	return newClass;
}

InternalMethod* InternalClass::addMethod(int accessFlag, std::string name, std::string descriptor, class StmtListNode* body)
{
	auto newMethod = new InternalMethod();
	newMethod->accessFlag = accessFlag;
	newMethod->_name = name;
	newMethod->_descriptor = descriptor;
	newMethod->_body = body;
	newMethod->_nameRef = _constTable->findOrAddConstant(Utf8_C, name);
	newMethod->_descriptorRef = _constTable->findOrAddConstant(Utf8_C, descriptor);
	auto nameAndTypeRef = _constTable->findOrAddConstant(NameAndType_C, "", 0, 0, newMethod->_nameRef, newMethod->_descriptorRef);
	newMethod->_methodRef = _constTable->findOrAddConstant(MethodRef_C, "", 0, 0, this->_classRef);
	_internalMethodMap[name] = newMethod;
	return newMethod;
}

ExternalMethod* ExternalClass::addMethod(std::string name, std::string descriptor)
{
	auto newMethod = new ExternalMethod();
	newMethod->_class = this;
	newMethod->_name = name;
	newMethod->_descriptor = descriptor;

	return newMethod;
}
