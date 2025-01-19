#pragma once
#include <string>
#include <map>
#include "constTable.h"
#include "methodTable.h"
#include "../ExceptionHelper.h"
#include "../tables/fieldTable.h"
#include "../tables/constTable.h"
#include "../tables/methodTable.h"
#include "../tables/localVarTable.h"
#include "../nodes/ExprNode.h"
#include "../nodes/StmtNode.h"

class ExternalField;
enum FieldAccessFlag;
enum MethodAccessFlag;
class ExternalMethod;
class StmtListNode;

class ExternalClass
{
public:
	ExternalClass(std::string name, std::string baseName);

	/*!
	* \brief Add an EXTERNAL method (this method does not write anything to the constant table) 
	* \return ExternalMethod - The ExternalMethod method created
	*/
	virtual ExternalMethod* addMethod(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags);
	
	/*!
	* \brief find an External method in map
	* \return ExternalMethod - The ExternalMethod method
	*/
	ExternalMethod* findMethod(std::string name);

	/*!
	* \brief Add an EXTERNAL field (this method does not write anything to the constant table)
	* \return ExternalField - The ExternalField method created
	*/
	virtual ExternalField* addField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr);
	
	/*!
	* \brief find an External field in map
	* \return ExternalField - The ExternalField field
	*/
	ExternalField* findField(std::string varName);

	std::string getClassName();
	std::string getBaseClassName();

protected:
	std::string _name;
	std::string _baseName;

	std::map<std::string, class ExternalMethod*> _methodMap = {};
	std::map<std::string, class ExternalField*> _fieldMap = {};
};

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
	
	/*!
	* \brief Add an external class (not this class) method to the constant table
	* \param ExternalMethod - The external method from another class to add
	* \return InternalMethod - The internal method created
	*/
	int findOrAddExternalClassMethod(ExternalMethod* externalMethod);

	/*!
	* \brief Search for this class methods
	* \return InternalMethod - The internal method
	*/
	InternalMethod* findInternalMethod(std::string methodName);
	
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

class ClassTable
{
public:
	std::map<std::string, ExternalClass*> _classes = {};

	/*!
	* \brief Search a field across all classes
	* \return ExternalField - The ExternalField field
	*/
	ExternalField* findField(std::string name, std::string descriptor, std::string className);

	/*!
	* \brief Search a method across all classes
	* \return ExternalMethod - The ExternalMethod field
	*/
	ExternalMethod* findMethod(std::string name, std::string descriptor, std::string className);

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

