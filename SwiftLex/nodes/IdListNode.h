#pragma once
#include "dottable.h"
#include "GenericListNode.h"

class IdNode : public Dottable
{
public:
	std::string _idName;

	static IdNode* create(std::string idName);

	void generateDot(std::ofstream& file) override;
};

class IdListNode : public GenericListNode<IdNode, IdListNode>
{
public:
	std::string getName() override;
};



