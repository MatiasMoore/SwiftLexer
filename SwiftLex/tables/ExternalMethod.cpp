#include "ExternalMethod.h"
#include "../ExceptionHelper.h"

ExternalMethod::ExternalMethod(std::string methodName, std::string descriptor, std::string className, std::vector<MethodAccessFlag> flags)
{
	this->_methodName = methodName;
	this->_descriptor = descriptor;
	this->_className = className;
	this->_flags = flags;
}

int ExternalMethod::addMethodRefToConstTable(ConstantTable* constTable)
{
	if (this->findMethodRef(constTable) != -1)
		throw std::runtime_error("Equal method with name " + this->_methodName + " and descriptor " + this->_descriptor + " already exists in this class!" + LINE_AND_FILE);

	int varNameRef = constTable->addUTF8(this->_methodName);
	int descriptorRef = constTable->addUTF8(this->_descriptor);
	int nameAndTypeRef = constTable->addNameAndType(varNameRef, descriptorRef);

	int classNameRef = constTable->addUTF8(this->_className);
	int classRef = constTable->addClassRef(classNameRef);

	int methodRef = constTable->addMethodRef(classRef, nameAndTypeRef);

	return methodRef;
}

int ExternalMethod::findMethodRef(ConstantTable* constTable)
{
	int nameRef = constTable->findUTF8(this->_methodName);
	int descriptorRef = constTable->findUTF8(this->_descriptor);
	int classNameRef = constTable->findUTF8(this->_className);
	if (nameRef == -1 || descriptorRef == -1 || classNameRef == -1)
		return -1;

	int nameAndTypeRef = constTable->findNameAndType(nameRef, descriptorRef);
	int classRef = constTable->findClassRef(classNameRef);

	if (nameAndTypeRef == -1 || classRef == -1)
		return -1;

	return constTable->findMethodRef(classRef, nameAndTypeRef);
}

int ExternalMethod::findOrAddMethodRef(ConstantTable* constTable)
{
	if (findMethodRef(constTable) == -1)
		return addMethodRefToConstTable(constTable);
	else
		return findMethodRef(constTable);
}

std::string ExternalMethod::getMethodName()
{
	return _methodName;
}

std::string ExternalMethod::getDescriptor()
{
	return _descriptor;
}

std::string ExternalMethod::getClassName()
{
	return _className;
}

std::vector<MethodAccessFlag> ExternalMethod::getFlags()
{
	return _flags;
}

int ExternalMethod::accessFlagsToInt()
{
	int accessFlag = 0;
	for (auto& flag : this->_flags)
	{
		accessFlag += (int)flag;
	}
	return accessFlag;
}

ExternalMethod::~ExternalMethod()
{
}
