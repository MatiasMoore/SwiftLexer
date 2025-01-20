#include "InternalClass.h"
#include "../ExceptionHelper.h"
#include "../generation/generationHelpers.h"

InternalClass::InternalClass(std::string name, std::string baseName) : ExternalClass(name, baseName)
{
	_constTable = new ConstantTable();
	_nameRef = _constTable->findOrAddUTF8(name);
	_baseNameRef = _constTable->findOrAddUTF8(baseName);
	_classRef = _constTable->findOrAddClassRef(_nameRef);
	_baseClassRef = _constTable->findOrAddClassRef(_baseNameRef);
}

InternalMethod* InternalClass::addInternalMethodToConstantTable(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags, StmtListNode* body)
{
	auto newMethod = new InternalMethod(_constTable, body, methodName, descriptor, getClassName(), flags);

	this->_methodContainer.addMethod(newMethod);

	return newMethod;
}

InternalField* InternalClass::addInternalFieldToConstantTable(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags)
{
	auto newField = new InternalField(_constTable, varName, descriptor, getClassName(), flags);

	this->_fieldContainer.addField(newField);

	return newField;
}

int InternalClass::getMethodRefForExternalMethod(ExternalMethod* externalMethod)
{
	return externalMethod->findOrAddMethodRef(_constTable);
}

int InternalClass::getFieldRefForExternalField(ExternalField* externalField)
{
	return externalField->findOrAddFieldRef(_constTable);
}

int InternalClass::getClassRefForExternalClass(ExternalClass* externalClass)
{
	auto nameRef = this->_constTable->findOrAddUTF8(externalClass->getClassName());
	return this->_constTable->findOrAddClassRef(nameRef);
}

InternalMethod* InternalClass::findInternalMethod(std::string methodName, std::string argDescriptor, bool isStatic)
{
	auto externalMethod = this->_methodContainer.findMethod(methodName, argDescriptor, isStatic);
	return dynamic_cast<InternalMethod*>(externalMethod);
}


InternalField* InternalClass::findInternalField(std::string varName, bool isStatic)
{
	auto field = this->findField(varName, isStatic);

	auto internalField = dynamic_cast<InternalField*>(field);

	if (internalField == nullptr)
		std::runtime_error("Something went wrong! We should not be there! For field" + varName + " in class " + this->_name + LINE_AND_FILE);

	return internalField;
}

std::vector<InternalMethod*> InternalClass::getInternalMethods()
{
	std::vector<InternalMethod*> internalMethods;
	auto allMethods = this->_methodContainer.getAll();
	for (auto& externalMethod : allMethods)
	{
		auto internalMethod = dynamic_cast<InternalMethod*>(externalMethod);
		if (internalMethod != nullptr)
			internalMethods.push_back(internalMethod);
	}
	return internalMethods;
}

std::vector<InternalField*> InternalClass::getInternalFields()
{
	std::vector<InternalField*> internalFields;
	auto allFields = this->_fieldContainer.getAll();
	for (auto& externalField : allFields)
	{
		auto internalField = dynamic_cast<InternalField*>(externalField);
		if (internalField != nullptr)
			internalFields.push_back(internalField);
	}
	return internalFields;
}

ConstantTable* InternalClass::getConstTable()
{
	return this->_constTable;
}

int InternalClass::getNameRef()
{
	return this->_nameRef;
}

int InternalClass::getBaseNameRef()
{
	return this->_baseNameRef;
}

int InternalClass::getBaseClassRef()
{
	return this->_baseClassRef;
}

int InternalClass::getClassRef()
{
	return this->_classRef;
}

