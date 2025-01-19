#include "InternalClass.h"
#include "../ExceptionHelper.h"

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

	if (this->_methodMap.count(methodName) != 0)
		throw std::runtime_error("Method " + methodName + " already exists in class " + getClassName() + LINE_AND_FILE);

	this->_methodMap[methodName] = newMethod;

	return newMethod;
}

int InternalClass::getMethodRefForExternalMethod(ExternalMethod* externalMethod)
{
	return externalMethod->findOrAddMethodRef(_constTable);
}

int InternalClass::getFieldRefForExternalField(ExternalField* externalField)
{
	return externalField->addFieldRefToConstTable(_constTable);
}

InternalMethod* InternalClass::findInternalMethod(std::string methodName, std::string descriptor)
{
	if (this->_methodMap.count(methodName) == 0)
		return nullptr;
	if (this->_methodMap[methodName]->getDescriptor() != descriptor)
		return nullptr;
	return dynamic_cast<InternalMethod*>(this->_methodMap[methodName]);
}


InternalField* InternalClass::findInternalField(std::string varName)
{
	auto field = this->findField(varName);

	auto internalField = dynamic_cast<InternalField*>(field);

	if (internalField == nullptr)
		std::runtime_error("Something went wrong! We should not be there! For field" + varName + " in class " + this->_name + LINE_AND_FILE);

	return internalField;
}

std::vector<InternalMethod*> InternalClass::getInternalMethods()
{
	std::vector<InternalMethod*> internalMethods;
	for (auto& methodPair : this->_methodMap)
	{
		auto internalMethod = dynamic_cast<InternalMethod*>(methodPair.second);
		if (internalMethod != nullptr)
			internalMethods.push_back(internalMethod);
	}
	return internalMethods;
}

std::vector<InternalField*> InternalClass::getInternalFields()
{
	std::vector<InternalField*> internalFields;
	for (auto& fieldPair : this->_fieldMap)
	{
		auto internalField = dynamic_cast<InternalField*>(fieldPair.second);
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

