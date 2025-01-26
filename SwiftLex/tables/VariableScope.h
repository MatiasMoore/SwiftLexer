#pragma once
#include <vector>
#include <map>
#include <string>

class LocalVariableElement;
class LocalVariableTable;

class VariableScope
{
private:
	VariableScope(LocalVariableTable* parentTable);

	VariableScope(VariableScope* parentScope);

	void addChildScope(VariableScope* childScope);

public:
	VariableScope* _parentScope;
	LocalVariableTable* _parentTable;
	std::vector<VariableScope*> _children = {};

	std::map<std::string, LocalVariableElement*> _nameToVar = {};

	LocalVariableElement* addLocalVar(std::string name, std::string descriptor, bool isConst);

	LocalVariableElement* findLocalVar(std::string name);

	friend class LocalVariableTable;
};

