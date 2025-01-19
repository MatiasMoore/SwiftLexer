#pragma once
#include <string>
#include "ExternalField.h"
#include "ExternalMethod.h"

class ExternalClass
{
public:
	ExternalClass(std::string name, std::string baseName);

	/*!
	* \brief Add an EXTERNAL method (this method does not write anything to the constant table)
	* \return ExternalMethod - The ExternalMethod method created
	*/
	ExternalMethod* addMethod(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags);

	/*!
	* \brief find an External method in map
	* \return ExternalMethod - The ExternalMethod method
	*/
	ExternalMethod* findMethod(std::string name);

	/*!
	* \brief Add an EXTERNAL field (this method does not write anything to the constant table)
	* \return ExternalField - The ExternalField method created
	*/
	ExternalField* addField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr);

	/*!
	* \brief find an External field in map
	* \return ExternalField - The ExternalField field
	*/
	ExternalField* findField(std::string varName);

	std::string getClassName();
	std::string getBaseClassName();

	virtual ~ExternalClass();

protected:
	std::string _name;
	std::string _baseName;

	std::map<std::string, class ExternalMethod*> _methodMap = {};
	std::map<std::string, class ExternalField*> _fieldMap = {};
};


