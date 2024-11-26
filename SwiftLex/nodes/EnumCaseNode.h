#pragma once
#include "dottable.h"

class IdListNode;

class EnumCaseNode : public Dottable
{
public:
	IdListNode* _idList;


	static EnumCaseNode* createEnumCaseNode(IdListNode* idList);

	void generateDot(std::ofstream& file) override;
};

