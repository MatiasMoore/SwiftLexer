#pragma once
#include "allNodes.h"
#include "RTLHelper.h"
#include "tables/tables.h"

class NodeTreeTransform
{
public:
	static StmtListNode* transformTree(ClassTable* classTable, StmtListNode* root)
	{
		// Add main class and method for free stmts
		auto newRoot = wrapFreeStmtsInsideMainClass(root);

		// Copy the rtl/Operators into <Operators>
		auto internalOperatorsClass = NodeTreeTransform::createInternalOperatorsClass(classTable, RTLHelper::_rtlOpClassName, RTLHelper::_internalOpClassName);

		// Add internal Operators class to root
		newRoot->appendNodeAtIndex(StmtNode::createClassDecl(internalOperatorsClass), 0);

		return newRoot;
	}

private: 
	
	static StmtListNode* wrapFreeStmtsInsideMainClass(StmtListNode* root);

	static ClassDeclNode* createInternalOperatorsClass(ClassTable* classTable, std::string rtlOpClassName, std::string internalOpClassName);

};

