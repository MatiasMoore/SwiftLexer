#include "classTable.h"
#include "../ExceptionHelper.h"

InternalClass* ClassTable::addInternalClass(std::string name, std::string baseName) 
{
	if (this->_internalClasses.count(name) != 0)
		throw std::runtime_error("Class " + name + " already exists!" + LINE_AND_FILE);

	auto newClass = new InternalClass(name, baseName, this);
	_internalClasses[name] = newClass;
	return newClass;
}

ExternalClass* ClassTable::addExternalClass(std::string name, std::string baseName)
{
	if (this->_externalClasses.count(name) != 0)
		throw std::runtime_error("Class " + name + " already exists!" + LINE_AND_FILE);

	auto newClass = new ExternalClass(name, baseName);
	_externalClasses[name] = newClass;
	return newClass;
}

InternalClass* ClassTable::findInternalClass(std::string name)
{
	if (this->_internalClasses.count(name) == 0)
		return nullptr;
	return this->_internalClasses[name];
}

ExternalClass* ClassTable::findExternalClass(std::string name)
{
	if (this->_externalClasses.count(name) == 0)
		return nullptr;
	return this->_externalClasses[name];
}

ExternalClass::ExternalClass(std::string name, std::string baseName)
{
	this->_name = name;
	this->_baseName = baseName;
}

InternalClass::InternalClass(std::string name, std::string baseName, ClassTable* _classTable) : ExternalClass(name, baseName)
{
	_constTable = new ConstantTable();
	this->_classTable = _classTable;
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
	_internalMethodMap[name] = newMethod;
	return newMethod;
}

InternalField* InternalClass::addInternalField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue, std::string className)
{
	className = className == "" ? this->_name : className;
	auto newField = new InternalField(varName, descriptor, this->_name, flags, constValue);

	if (this->_currentInternalFieldMap.count(varName) != 0)
		throw std::runtime_error("Field " + varName + " already exists in class " + this->_name + LINE_AND_FILE);

	this->_currentInternalFieldMap[varName] = newField;

	newField->addFieldRefToConstTable(this->_constTable);

	return newField;
}

InternalField* InternalClass::addCompilatedClassInternalField(InternalField* internalField)
{
	std::string varName = internalField->getVarName();
	std::string className = internalField->getClassName();

	if (this->_usedCompilatedClassFieldMap.count(className) == 0)
		this->_usedCompilatedClassFieldMap[className] = {};

	if (this->_usedCompilatedClassFieldMap[className].count(varName) != 0)
		throw std::runtime_error("Something went wrong! We should not be there! For field" + varName + " compilated class " + className + " for class " + this->_name + LINE_AND_FILE);

	this->_usedCompilatedClassFieldMap[className][varName] = internalField;

	internalField->addFieldRefToConstTable(this->_constTable);

	return internalField;
}

InternalField* InternalClass::findCurrentInternalField(std::string varName)
{
	if (this->_currentInternalFieldMap.count(varName) == 0)
		return nullptr;
	return this->_currentInternalFieldMap[varName];
}

InternalField* InternalClass::findUsedCompilatedClassInternalField(std::string varName, std::string className)
{
	if (this->_usedCompilatedClassFieldMap.count(className) == 0)
		return nullptr;
	auto classFieldMap = this->_usedCompilatedClassFieldMap[className];
	if (classFieldMap.count(varName) == 0)
		return nullptr;
	return classFieldMap[varName];
}

InternalField* InternalClass::findInternalField(std::string varName, std::string className)
{
	if (className == "")
	{
		return this->findCurrentInternalField(varName);
	}
	else 
	{
		return this->findUsedCompilatedClassInternalField(varName, className);
	}

}

ExternalField* InternalClass::addExternalField(std::string varName, std::string descriptor, std::string className)
{
	auto newField = new ExternalField(varName, descriptor, this->_name);
	if (_usedExternalClassFieldMap.count(className) == 0)
		_usedExternalClassFieldMap[className] = {};
	_usedExternalClassFieldMap[className][varName] = newField;
	newField->addFieldRefToConstTable(this->_constTable);
	return newField;
}

ExternalField* InternalClass::addExternalField(ExternalField* externalField)
{
	std::string varName = externalField->getVarName();
	std::string className = externalField->getClassName();
	if (_usedExternalClassFieldMap.count(className) == 0)
		_usedExternalClassFieldMap[className] = {};
	_usedExternalClassFieldMap[className][varName] = externalField;
	externalField->addFieldRefToConstTable(this->_constTable);
	return externalField;
}

ExternalField* InternalClass::findCurrentExternalField(std::string varName, std::string className)
{
	if (_usedExternalClassFieldMap.count(className) == 0)
		return nullptr;
	auto classFieldMap = _usedExternalClassFieldMap[className];
	if (classFieldMap.count(varName) == 0)
		return nullptr;
	return classFieldMap[varName];
}

int InternalClass::findOrAddFieldRef(std::string varName, std::string className)
{
	// Search in my fields
	if (className == "")
	{
		InternalField* field = this->findCurrentInternalField(varName);
		if (field != nullptr)
		{
			int fieldRef = field->findFieldRef(_constTable);
			if (fieldRef == -1)
				throw std::runtime_error("Something went wrong! We should not be there! For field" + varName + " in class " + this->_name + LINE_AND_FILE);
			return fieldRef;
		}
		return -1;
	}

	// Search in compiled internal classes field
	// Search if field already used
	InternalField* field = this->findUsedCompilatedClassInternalField(varName, className);
	if (field != nullptr)
	{
		int fieldRef = field->findFieldRef(_constTable);
		if (fieldRef == -1)
			throw std::runtime_error("Something went wrong! We should not be there! For field" + varName + " compilated class " + className + " in class " + this->_name + LINE_AND_FILE);
		return fieldRef;
	}

	// If not used, search in compiled internal classes field
	InternalClass* compiledInternalClass = this->_classTable->findInternalClass(className);
	if (compiledInternalClass != nullptr)
	{
		InternalField* field = compiledInternalClass->findInternalField(varName);
		if (field != nullptr)
			// inf exist -> add field to our constant pool
			return this->addCompilatedClassInternalField(field)->findFieldRef(this->_constTable);
	}

	// Search in rtl classes field
	// Search if field already used 
	ExternalField* externalField = this->findCurrentExternalField(varName, className);
	if (externalField != nullptr)
	{
		int fieldRef = externalField->findFieldRef(_constTable);
		if (fieldRef == -1)
			throw std::runtime_error("Something went wrong! We should not be there! For field" + varName + " external class " + className + " in class " + this->_name + LINE_AND_FILE);
		return fieldRef;
	}

	// If not used, search in rtl classes field
	ExternalClass* externalClass = this->_classTable->findExternalClass(className);
	if (externalClass == nullptr)
		return -1;

	externalField = externalClass->findExternalField(varName);
	if (externalField != nullptr)
		return this->addExternalField(externalField)->findFieldRef(_constTable);

	return -1;
}

ExternalMethod* ExternalClass::addMethod(std::string name, std::string descriptor)
{
	auto newMethod = new ExternalMethod();
	newMethod->_class = this;
	newMethod->_name = name;
	newMethod->_descriptor = descriptor;

	return newMethod;
}

ExternalField* ExternalClass::addExternalField(std::string varName, std::string descriptor)
{
	auto newField = new ExternalField(varName, descriptor, this->_name);
	this->_externalFieldMap[varName] = newField;
	return newField;
}

ExternalField* ExternalClass::addExternalField(ExternalField* externalField)
{
	this->_externalFieldMap[externalField->getVarName()] = externalField;
	return externalField;
}

ExternalField* ExternalClass::findExternalField(std::string varName)
{
	if (this->_externalFieldMap.count(varName) == 0)
		return nullptr;
	return this->_externalFieldMap[varName];
}
