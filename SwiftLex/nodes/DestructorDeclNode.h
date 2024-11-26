#pragma once
#include "dottable.h"

class StmtListNode;

class DestructorDeclNode : public Dottable
{
public:
	StmtListNode* _body;

	static DestructorDeclNode* createDestructor(StmtListNode* body);

	void generateDot(std::ofstream& file) override;
};

