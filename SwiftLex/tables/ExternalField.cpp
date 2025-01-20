#include "ExternalField.h"
#include "../ExceptionHelper.h"

ExternalField::ExternalField(std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags)
{
	this->_varName = varName;
	this->_descriptor = descriptor;
	this->_className = className;
	this->_flags = flags;
}

int ExternalField::findOrAddFieldRef(ConstantTable* constTable)
{
	if (this->findFieldRef(constTable) == -1)
		return this->addFieldRefToConstTable(constTable);
	else
		return this->findFieldRef(constTable);
}

int ExternalField::addFieldRefToConstTable(ConstantTable* constTable)
{
	if (this->findFieldRef(constTable) != -1)
		throw std::runtime_error("Equal field with name " + this->_varName + " and descriptor " + this->_descriptor + " already exists in this class!" + LINE_AND_FILE);

	int varNameRef = constTable->findOrAddUTF8(this->_varName);
	int descriptorRef = constTable->findOrAddUTF8(this->_descriptor);
	int nameAndTypeRef = constTable->findOrAddNameAndType(varNameRef, descriptorRef);

	int classNameRef = constTable->findOrAddUTF8(this->_className);
	int classRef = constTable->findOrAddClassRef(classNameRef);

	int fieldRef = constTable->findOrAddFieldRef(classRef, nameAndTypeRef);

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

bool ExternalField::containsFlag(FieldAccessFlag flag)
{
	return std::find(_flags.begin(), _flags.end(), flag) != _flags.end();
}

ExternalField::~ExternalField()
{
}

std::string ExternalField::getDescriptor()
{
	return this->_descriptor;
}