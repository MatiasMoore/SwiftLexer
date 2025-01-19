#pragma once
#include <string>
#include <map>
#include "constTable.h"
#include "methodTable.h"
#include "../ExceptionHelper.h"
#include "fieldTable.h"
#include "../tables/localVarTable.h"

class ExternalClass
{
public:
	ExternalClass(std::string name, std::string baseName);

	virtual ExternalMethod* addMethod(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags, LocalVariableTable* varTable);
	ExternalMethod* findMethod(std::string name);

	virtual ExternalField* addField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr);
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

    std::vector<class InternalMethod*> _externalClassMethodMap = {};
	std::vector<class InternalField*> _externalClassFieldMap = {};

	// Methods
	InternalMethod* addInternalMethod(std::string methodName, std::string descriptor, std::vector<MethodAccessFlag> flags, StmtListNode* body, LocalVariableTable* varTable);
	InternalMethod* addExternalClassMethodToConstantTable(ExternalMethod* externalMethod);

	InternalMethod* findInternalMethod(std::string methodName);
	InternalMethod* findExternalClassMethod(std::string methodName, std::string className);
	
	// Fields
	InternalField* addInternalField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr);
	InternalField* addExternalClassFieldToConstantTable(ExternalField* externalField);

	InternalField* findInternalField(std::string varName);
	InternalField* findExternalClassField(std::string varName, std::string className);

private:
	ConstantTable* _constTable;

	int _classRef;
	int _nameRef;
	int _baseRef;
};

class ClassTable
{
public:
	std::map<std::string, ExternalClass*> _classes = {};

	ExternalField* findField(std::string name, std::string descriptor, std::string className);
	ExternalMethod* findMethod(std::string name, std::string descriptor, std::string className);

	InternalClass* addInternalClass(std::string name, std::string baseName);
	ExternalClass* addExternalClass(std::string name, std::string baseName);

	ExternalClass* findClass(std::string name);
	InternalClass* findInternalClass(std::string name);
	ExternalClass* findNotInternalClass(std::string name);

	std::vector<InternalClass*> getInternalClasses();
	
};

