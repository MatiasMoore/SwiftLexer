#include "InternalField.h"
#include "../ExceptionHelper.h"

InternalField::InternalField(ConstantTable* constantTable, std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags, ExprNode* constValue) : ExternalField(varName, descriptor, className, flags, constValue)
{
	this->_fieldRef = this->addFieldRefToConstTable(constantTable);
	this->_nameRef = constantTable->findUTF8(varName);
	this->_descriptorRef = constantTable->findUTF8(descriptor);
	this->_classRef = constantTable->findUTF8(className);
	this->_nameAndTypeRef = constantTable->findNameAndType(this->_nameRef, this->_descriptorRef);
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
