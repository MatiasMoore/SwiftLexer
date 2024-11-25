#pragma once
#include "dottable.h"

class ExprNode;

class TryNode : public Dottable
{

public:
	
	ExprNode* _expr;

	static TryNode* create(ExprNode* expr);

	void generateDot(std::ofstream& file) override;
};

