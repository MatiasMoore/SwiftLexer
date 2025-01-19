#include "InternalMethod.h"
#include "FieldAccessFlag.h"
#include "../ExceptionHelper.h"

LocalVariableTable* InternalMethod::getVarTable()
{
	return this->_varTable;
}

InternalMethod::InternalMethod(ConstantTable* constTable, StmtListNode* body, std::string methodName, std::string descriptor, std::string className, std::vector<MethodAccessFlag> flags)
	: ExternalMethod(methodName, descriptor, className, flags)
{

	_varTable = new LocalVariableTable();
	_nameRef = constTable->findOrAddUTF8(methodName);
	_descriptorRef = constTable->findOrAddUTF8(descriptor);
	_classRef = constTable->findOrAddClassRef(_nameRef);
	_nameAndTypeRef = constTable->findOrAddNameAndType(_nameRef, _descriptorRef);
	_methodRef = constTable->findOrAddMethodRef(_classRef, _nameAndTypeRef);

	_body = body;
}

bool InternalMethod::isStatic()
{
	return std::find(this->_flags.begin(), this->_flags.end(), F_ACC_STATIC) != this->_flags.end();
}
