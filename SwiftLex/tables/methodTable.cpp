#include "methodTable.h"
#include "classTable.h"

InternalMethod::InternalMethod()
{
	_varTable = new LocalVariableTable();
}

int ExternalMethod::addMethodRefToConstTable(ConstantTable* constTable)
{
	auto nameRef = constTable->findOrAddConstant(Utf8_C, this->_name);
	auto descriptorRef = constTable->findOrAddConstant(Utf8_C, this->_descriptor);
	auto nameAndTypeRef = constTable->findOrAddConstant(NameAndType_C, "", 0, 0, nameRef, descriptorRef);
	auto classNameRef = constTable->findOrAddConstant(Utf8_C, this->_class->_name);
	auto classRef = constTable->findOrAddConstant(Class_C, "", 0, 0, classNameRef);
	auto methodRef = constTable->findOrAddConstant(MethodRef_C, "", 0, 0, classRef);
	return methodRef;
}
