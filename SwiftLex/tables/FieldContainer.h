#pragma once
#include "ExternalField.h"
class FieldContainer
{
public:
	ExternalField* findField(std::string name, bool isStatic);

	ExternalField* addField(ExternalField* field);

	std::vector<ExternalField*> getAll();

private:
	std::map < std::string,
		std::map<bool, ExternalField*>
	> _nameIsStaticToField = {};
};

