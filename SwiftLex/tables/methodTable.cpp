#include "methodTable.h"
#include "classTable.h"

InternalMethod::InternalMethod()
{
	_varTable = new LocalVariableTable();
}

int ExternalMethod::addMethodRefToConstTable(ConstantTable* constTable)
{
	auto nameRef = constTable->findOrAddUTF8(this->_name);
	auto descriptorRef = constTable->findOrAddUTF8(this->_descriptor);
	auto nameAndTypeRef = constTable->findOrAddNameAndType(nameRef, descriptorRef);
	auto classNameRef = constTable->findOrAddUTF8(this->_class->_name);
	auto classRef = constTable->findOrAddClassRef(classNameRef);
	auto methodRef = constTable->findOrAddMethodRef(classRef, nameAndTypeRef);
	return methodRef;
}
