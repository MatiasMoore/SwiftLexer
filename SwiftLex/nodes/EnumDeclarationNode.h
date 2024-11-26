#pragma once
#include "dottable.h"
class StmtListNode;
class EnumDeclarationNode : public Dottable
{
public:
	StmtListNode* _stmtList;

	std::string _name;

	static EnumDeclarationNode* createEnumDeclarationNode(std::string name, StmtListNode* stmtList);

	void generateDot(std::ofstream& file) override;
};

