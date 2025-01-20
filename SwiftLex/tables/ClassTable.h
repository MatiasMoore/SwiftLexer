#pragma once
#include <string>
#include <map>
#include "InternalField.h"
#include "InternalMethod.h"
#include "ExternalClass.h"
#include "InternalClass.h"

class ClassTable
{
public:
	std::map<std::string, ExternalClass*> _classes = {};

	/*!
	* \brief Search a field in class
	* \return ExternalField - The ExternalField field
	*/
	ExternalField* findField(std::string name, std::string className, bool isStatic);

	/*!
	* \brief Search a method across all classes
	* \return ExternalMethod - The ExternalMethod field
	*/
	ExternalMethod* findMethod(std::string name, std::string argDescriptor, std::string className, bool isStatic);

	/*!
	* \brief Create a class that will have the constant table
	* \return ExternalClass - The ExternalClass created
	*/
	InternalClass* addInternalClass(std::string name, std::string baseName);

	/*!
	* \brief Create a class that will not have the constant table (For already compiled files) P.S.for RTL
	* \return ExternalClass - The ExternalClass created
	*/
	ExternalClass* addExternalClass(std::string name, std::string baseName);

	/*!
	* \brief Search for all classes, that class can be Internal or External
	* \return ExternalClass - The ExternalClass
	*/
	ExternalClass* findClass(std::string name);

	/*!
	* \brief Search for only InternalClass
	* \return InternalClass - The InternalClass
	*/
	InternalClass* findInternalClass(std::string name);

	/*!
	* \brief Search for all ExternalClass that are not InternalClass
	* \return ExternalClass - The ExternalClass that can't dynamic_cast to InternalClass
	*/
	ExternalClass* findNotInternalClass(std::string name);

	std::vector<InternalClass*> getInternalClasses();

};