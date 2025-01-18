#pragma once
#include <string>
#include <map>
#include "constTable.h"
#include "methodTable.h"
#include "../ExceptionHelper.h"

class ExternalClass
{
public:
	std::string _name;
	std::string _baseName;

    std::map<std::string, class ExternalMethod*> _externalMethodMap = {};

	ExternalMethod* addMethod(std::string name, std::string descriptor);
};

class InternalClass : public ExternalClass
{
public:
	InternalClass();

	int _classRef;
	int _nameRef;
	int _baseRef;

    std::map<std::string, class InternalMethod*> _internalMethodMap = {};

	ConstantTable* _constTable;

	InternalMethod* addMethod(int accessFlag, std::string name, std::string descriptor, class StmtListNode* body);
};

class ClassTable
{
public:
	std::map<std::string, ExternalClass*> _externalClasses = {};

	std::map<std::string, InternalClass*> _internalClasses = {};

	InternalClass* addInternalClass(std::string name, std::string baseName);

	ExternalClass* addExternalClass(std::string name, std::string baseName);
};

