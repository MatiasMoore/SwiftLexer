#include "NodeTreeTransform.h"

StmtListNode* NodeTreeTransform::wrapFreeStmtsInsideMainClass(StmtListNode* root)
{
	// Main method
	auto mainFuncArgType = TypeNode::createArrayType(TypeNode::createType(TypeType::StringT));
	auto mainFuncArg = FuncDeclArgNode::createPositionalArg("args", mainFuncArgType, nullptr);
	auto mainFuncArgs = FuncDeclArgListNode::createListNode(mainFuncArg);
	auto mainFuncReturn = StmtNode::createStmtReturn(ReturnNode::createVoidReturn());
	auto mainFuncBody = StmtListNode::createListNode(mainFuncReturn);
	auto mainFuncDecl = FuncDeclNode::createRegular(RTLHelper::_defaultMainFunc, mainFuncArgs, mainFuncBody, nullptr, false);
	auto mainFuncModifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(Public));
	mainFuncModifiers->appendNode(AccessModifierNode::createModifier(Static));
	mainFuncDecl->addModifiers(mainFuncModifiers);
	auto mainFuncDeclStmt = StmtNode::createStmtFuncDecl(mainFuncDecl);

	// Main class
	auto mainClassBody = StmtListNode::createListNode(mainFuncDeclStmt);
	auto mainClassDecl = ClassDeclNode::createClass(RTLHelper::_defaultMainClass, mainClassBody);
	auto mainClassDeclStmt = StmtNode::createClassDecl(mainClassDecl);

	StmtListNode* newRoot = StmtListNode::createListNode(mainClassDeclStmt);

	if (root != nullptr)
	{
		for (auto& stmt : root->_vec)
		{
			// For classes
			if (stmt->_type == StmtType::ClassDecl)
			{
				newRoot->appendNodeBeforeNode(stmt, mainClassDeclStmt);
			}
			// For functions
			else if (stmt->_type == StmtType::FuncDecl)
			{
				// Make all free methods public and static
				auto funcDecl = stmt->_funcDecl;
				if (!funcDecl->_hasModifiers)
				{
					auto defaultModifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(Public));
					defaultModifiers->appendNode(AccessModifierNode::createModifier(Static));
					funcDecl->addModifiers(defaultModifiers);
				}

				mainClassBody->appendNodeBeforeNode(stmt, mainFuncDeclStmt);
			}
			else // All other stmts
			{
				mainFuncBody->appendNodeBeforeNode(stmt, mainFuncReturn);
			}
		}
	}

	return newRoot;
}

ClassDeclNode* NodeTreeTransform::createInternalOperatorsClass(ClassTable* classTable, std::string rtlOpClassName, std::string internalOpClassName)
{
	// Find rtl/Operators
	auto rtlOperators = classTable->findClass(rtlOpClassName);
	if (rtlOperators == nullptr)
		throw std::runtime_error("Can't find " + rtlOpClassName);

	// Create internal Operators class
	StmtListNode* internalOpClassBody = nullptr;
	auto rtlMethods = rtlOperators->getMethods();
	for (auto& rtlMethod : rtlMethods)
	{
		// Skip constructors
		if (rtlMethod->getMethodName() == "<init>")
			continue;

		// TODO check for public
		if (!rtlMethod->containsFlag(MethodAccessFlag::M_ACC_STATIC))
			throw std::runtime_error("All methods inside \"" + rtlOpClassName + "\" must be static, but method \""
				+ rtlMethod->getMethodName() + "\" is not static!" + LINE_AND_FILE);

		// Create argument nodes
		auto rtlArgDescriptors = rtlMethod->getArgDescriptorsVector();
		FuncCallArgListNode* callArgs = nullptr;
		FuncDeclArgListNode* declArgs = nullptr;
		for (auto& argDesc : rtlArgDescriptors)
		{
			auto newArgName = RTLHelper::getUniqueInternalVarName();

			auto newDeclArg = FuncDeclArgNode::createPositionalArg(newArgName, TypeNode::createFromDescriptor(argDesc), nullptr);
			auto newCallArg = FuncCallArgNode::createFromExpr(ExprNode::createId(newArgName));

			if (callArgs == nullptr)
				callArgs = FuncCallArgListNode::createListNode(newCallArg);
			else
				callArgs->appendNode(newCallArg);

			if (declArgs == nullptr)
				declArgs = FuncDeclArgListNode::createListNode(newDeclArg);
			else
				declArgs->appendNode(newDeclArg);

		}

		// Call node
		auto rtlCall = FuncCallNode::createFuncCall(rtlMethod->getMethodName(), callArgs);
		rtlCall->setAsExprAccess(ExprNode::createId(rtlOpClassName));

		// New method body
		StmtListNode* methodBody = nullptr;
		TypeNode* methodReturn = nullptr;
		if (rtlMethod->getReturnDescriptor() == "V")
		{
			methodBody = StmtListNode::createListNode(
				StmtNode::createStmtExpr(ExprNode::createFuncCall(rtlCall))
			);
			methodBody->appendNode(StmtNode::createStmtReturn(ReturnNode::createVoidReturn()));
		}
		else
		{
			methodBody = StmtListNode::createListNode(
				StmtNode::createStmtReturn(ReturnNode::createExprReturn(ExprNode::createFuncCall(rtlCall)))
			);
			methodReturn = TypeNode::createFromDescriptor(rtlMethod->getReturnDescriptor());
		}

		// New method declaration
		auto methodDecl = FuncDeclNode::createRegular(rtlMethod->getMethodName(), declArgs, methodBody, methodReturn, false);
		methodDecl->addModifiers(
			AccessModifierListNode::createListNode(AccessModifierNode::createModifier(Public))
			->appendNode(AccessModifierNode::createModifier(Static))
		);

		// Add to internal Operators class body
		if (internalOpClassBody == nullptr)
			internalOpClassBody = StmtListNode::createListNode(StmtNode::createStmtFuncDecl(methodDecl));
		else
			internalOpClassBody->appendNode(StmtNode::createStmtFuncDecl(methodDecl));
	}

	// Create internal Operators class
	return ClassDeclNode::createClass(internalOpClassName, internalOpClassBody);
}
