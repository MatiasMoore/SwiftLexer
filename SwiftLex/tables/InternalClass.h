#pragma once
#include <string>
#include <map>
#include "InternalField.h"
#include "InternalMethod.h"
#include "ExternalClass.h"

class InternalClass : public ExternalClass
{
public:
	InternalClass(std::string name, std::string baseName);

	// Methods
	/*!
	* \brief Add this class method to the constant table
	* \return InternalMethod - The internal method created
	*/
	InternalMethod* addInternalMethodToConstantTable(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags, StmtListNode* body);
	
	int getMethodRefForExternalMethod(ExternalMethod* externalMethod);

	int getFieldRefForExternalField(ExternalField* externalField);

	/*!
	* \brief Search for this class methods
	* \return InternalMethod - The internal method
	*/
	InternalMethod* findInternalMethod(std::string methodName, std::string descriptor);
	
	/*!
	* \brief Search for this class fields
	* \return InternalField - The internal field
	*/
	InternalField* findInternalField(std::string varName);

	std::vector<InternalMethod*> getInternalMethods();

	std::vector<InternalField*> getInternalFields();

	ConstantTable* getConstTable();

	int getNameRef();

	int getBaseNameRef();

	int getBaseClassRef();

	int getClassRef();

private:
	ConstantTable* _constTable;

	int _classRef;
	int _baseClassRef;
	int _nameRef;
	int _baseNameRef;
};
