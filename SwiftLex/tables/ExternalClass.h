#pragma once
#include <string>
#include "ExternalField.h"
#include "ExternalMethod.h"
#include "MethodContainer.h"
#include "FieldContainer.h"

class ClassTable;

class ExternalClass
{
public:
	ExternalClass(std::string name, std::string baseName);

	ExternalMethod* overrideMethod(ExternalMethod* oldMethod, ExternalMethod* newMethod);

	/*!
	* \brief Add an EXTERNAL method (this method does not write anything to the constant table)
	* \return ExternalMethod - The ExternalMethod method created
	*/
	ExternalMethod* addMethod(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags);

	/*!
	* \brief find an External method in map
	* \return ExternalMethod - The ExternalMethod method
	*/
	ExternalMethod* findMethod(std::string name, std::string argDescriptor, bool isStatic);

	/*!
	* \brief Add an EXTERNAL field (this method does not write anything to the constant table)
	* \return ExternalField - The ExternalField method created
	*/
	ExternalField* addField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags);

	/*!
	* \brief find an External field in map
	* \return ExternalField - The ExternalField field
	*/
	ExternalField* findField(std::string varName, bool isStatic);

	std::vector<ExternalMethod*> getMethodsWithNameAndParamCount(std::string methodName, int argCount);

	std::vector<ExternalMethod*> getMethods();
	std::vector<ExternalField*> getFields();

	std::string getClassName();
	std::string getBaseClassName();

	virtual ~ExternalClass();

protected:
	std::string _name;
	std::string _baseName;

	MethodContainer _methodContainer;
	FieldContainer _fieldContainer;
};


