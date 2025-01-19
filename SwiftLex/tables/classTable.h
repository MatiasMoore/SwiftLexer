#pragma once
#include <string>
#include <map>
#include "constTable.h"
#include "methodTable.h"
#include "../ExceptionHelper.h"
#include "fieldTable.h"

class ExternalClass
{
public:
	ExternalClass(std::string name, std::string baseName);

	virtual ExternalMethod* addMethod(std::string name, std::string descriptor);

	virtual ExternalField* addField(std::string varName, std::string descriptor, std::string className, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr);

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

	InternalMethod* addMethod(int accessFlag, std::string name, std::string descriptor, class StmtListNode* body);
	
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

	InternalClass* addInternalClass(std::string name, std::string baseName);
	ExternalClass* addExternalClass(std::string name, std::string baseName);

	ExternalClass* findClass(std::string name);
	InternalClass* findInternalClass(std::string name);
	ExternalClass* findNotInternalClass(std::string name);

	std::vector<InternalClass*> getInternalClasses();
	
	// myclass.addToConstanTable(findClass(className).findField(name, descriptor))
};

