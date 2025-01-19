#include "fieldTable.h"
#include "../ExceptionHelper.h"

ExternalField::ExternalField(std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags, ExprNode* constValue)
{
	this->_varName = varName;
	this->_descriptor = descriptor;
	this->_className = className;
	this->_flags = flags;
	this->_constValue = constValue;
}

int ExternalField::addFieldRefToConstTable(ConstantTable* constTable)
{
	if (this->findFieldRef(constTable) != -1 )
		throw std::runtime_error("Equal field with name " + this->_varName + " and descriptor " + this->_descriptor + " already exists in this class!" + LINE_AND_FILE);

	int varNameRef = constTable->addUTF8(this->_varName);
	int descriptorRef = constTable->addUTF8(this->_descriptor);
	int nameAndTypeRef = constTable->addNameAndType(varNameRef, descriptorRef);

	int classNameRef = constTable->addUTF8(this->_className);
	int classRef = constTable->addClassRef(classNameRef);

	int fieldRef = constTable->addFieldRef(classRef, nameAndTypeRef);

	return fieldRef;
}

int ExternalField::findFieldRef(ConstantTable* constTable)
{
	int nameRef = constTable->findUTF8(this->_varName);
	int descriptorRef = constTable->findUTF8(this->_descriptor);
	int classNameRef = constTable->findUTF8(this->_className);
	if (nameRef == -1 || descriptorRef == -1 || classNameRef == -1)
		return -1;

	int nameAndTypeRef = constTable->findNameAndType(nameRef, descriptorRef);
	int classRef = constTable->findClassRef(classNameRef);

	if (nameAndTypeRef == -1 || classRef == -1)
		return -1;

	return constTable->findFieldRef(classRef, nameAndTypeRef);
}

std::string ExternalField::getVarName()
{
	return this->_varName;
}

std::string ExternalField::getClassName()
{
	return this->_className;
}

std::vector<FieldAccessFlag> ExternalField::getFlags()
{
	return _flags;
}

ExprNode* ExternalField::getConstValue()
{
	return _constValue;
}

std::string ExternalField::getDescriptor()
{
	return this->_descriptor;
}

InternalField::InternalField(ConstantTable* constantTable, std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags, ExprNode* constValue) : ExternalField(varName, descriptor, className, flags, constValue)
{
	this->_fieldRef = this->addFieldRefToConstTable(constantTable);
	this->_nameRef = constantTable->findUTF8(varName);
	this->_descriptorRef = constantTable->findUTF8(descriptor);
	this->_classRef = constantTable->findUTF8(className);
	this->_nameAndTypeRef = constantTable->findNameAndType(this->_nameRef, this->_descriptorRef);
}

int InternalField::accessFlagsToInt(std::vector<FieldAccessFlag> flags)
{
	int accessFlag = 0;
	for (auto& flag : flags)
	{
		accessFlag += (int)flag;
	}
	return accessFlag;
}

bool InternalField::isStatic()
{
	return std::find(this->_flags.begin(), this->_flags.end(), F_ACC_STATIC) != this->_flags.end();
}
