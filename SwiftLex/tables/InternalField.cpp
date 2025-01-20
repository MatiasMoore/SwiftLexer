#include "InternalField.h"
#include "../ExceptionHelper.h"

InternalField::InternalField(ConstantTable* constantTable, std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags) : ExternalField(varName, descriptor, className, flags)
{
	this->_nameRef = constantTable->findOrAddUTF8(varName);
	this->_descriptorRef = constantTable->findOrAddUTF8(descriptor);
	this->__classNameRef = constantTable->findOrAddUTF8(className);

	this->_classRef = constantTable->findOrAddClassRef(this->__classNameRef);
	this->_nameAndTypeRef = constantTable->findOrAddNameAndType(this->_nameRef, this->_descriptorRef);

	this->_fieldRef = constantTable->findOrAddFieldRef(this->_classRef, this->_nameAndTypeRef);
	
	this->_flags = flags;
}

int InternalField::accessFlagsToInt()
{
	int accessFlag = 0;
	for (auto& flag : this->_flags)
	{
		accessFlag += (int)flag;
	}
	return accessFlag;
}

bool InternalField::isStatic()
{
	return std::find(this->_flags.begin(), this->_flags.end(), F_ACC_STATIC) != this->_flags.end();
}
