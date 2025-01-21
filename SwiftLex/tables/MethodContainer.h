#pragma once
#include "ExternalMethod.h"

class MethodContainer
{
public:
	ExternalMethod* findMethod(std::string name, std::string argDescriptor, bool isStatic);

	ExternalMethod* addMethod(ExternalMethod* method);

	std::vector<ExternalMethod*> getAll();

	std::vector<ExternalMethod*> getMethodsWithNameAndParamCount(std::string methodName, int argCount);

private:
	std::map < std::string,
		std::map<std::string,
			std::map<bool, ExternalMethod*>
			>
	> _nameArgDescIsStaticToMethod = {};
};

