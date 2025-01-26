#include "StmtNode.h"
#include "ExprNode.h"
#include "VarDeclarationNode.h"
#include "ReturnNode.h"
#include "LoopNode.h"
#include "IfElseNode.h"
#include "FuncDeclNode.h"
#include "ThrowNode.h"
#include "TryNode.h"
#include "DoCatchNode.h"
#include "SwitchNode.h"
#include "EnumCaseNode.h"
#include "EnumDeclarationNode.h"
#include "StructDeclarationNode.h"
#include "ConstructorDeclNode.h"
#include "DestructorDeclNode.h"
#include "ClassDeclNode.h"
#include "TypeNode.h"
#include "../generation/generationHelpers.h"
#include <set>
#include "../ExceptionHelper.h"
#include "../RTLHelper.h"
#include "FuncCallNode.h"

void StmtNode::setSkipConstCheck(bool flag)
{
	this->_assignmentSkipConstCheck = flag;
}

StmtNode* StmtNode::createStmtExpr(ExprNode* expr)
{
	auto node = new StmtNode();
	node->_type = StmtType::Expr;
	node->_expr = expr;
	printf("N: stmt expr\n");
	return node;
}

StmtNode* StmtNode::createStmtAssignment(ExprNode* left, ExprNode* right)
{
	auto node = new StmtNode();
	node->_type = StmtType::Assignment;
	node->_assignLeft = left;
	node->_assignRight = right;
	printf("N: stmt assignment\n");
	return node;
}

StmtNode* StmtNode::createStmtVarDeclaration(VarDeclarationListNode* varDeclList)
{
	auto node = new StmtNode();
	node->_type = StmtType::VarDeclarationList;
	node->_varDeclList = varDeclList;
	printf("N: stmt var decl\n");
	return node;
}

StmtNode* StmtNode::createStmtReturn(ReturnNode* ret)
{
	auto node = new StmtNode();
	node->_type = StmtType::Return;
	node->_return = ret;
	printf("N: stmt return\n");
	return node;
}

StmtNode* StmtNode::createStmtLoop(LoopNode* loop)
{
	auto node = new StmtNode();
	node->_type = StmtType::Loop;
	node->_loop = loop;
	printf("N: stmt loop\n");
	return node;
}

StmtNode* StmtNode::createStmtIfElse(IfElseNode* ifElse)
{
	auto node = new StmtNode();
	node->_type = StmtType::IfElse;
	node->_ifElse = ifElse;
	printf("N: stmt ifElse\n");
	return node;
}

StmtNode* StmtNode::createStmtFuncDecl(FuncDeclNode* funcDecl)
{
	auto node = new StmtNode();
	node->_type = StmtType::FuncDecl;
	node->_funcDecl = funcDecl;
	printf("N: stmt funcDecl\n");
	return node;
}

StmtNode* StmtNode::createStmtThrow(ThrowNode* throwNode)
{
	auto node = new StmtNode();
	node->_type = StmtType::Throw;
	node->_throw = throwNode;
	printf("N: stmt throw\n");
	return node;
}

StmtNode* StmtNode::createStmtTry(TryNode* tryNode)
{
	auto node = new StmtNode();
	node->_type = StmtType::Try;
	node->_tryNode = tryNode;
	printf("N: stmt throw\n");
	return node;
}

StmtNode* StmtNode::createStmtDo(DoCatchNode* doCatch)
{
	auto node = new StmtNode();
	node->_type = StmtType::Do;
	node->_do = doCatch;
	printf("N: stmt doCatch\n");
	return node;
}

StmtNode* StmtNode::createStmtSwitch(SwitchNode* switchNode)
{
	auto node = new StmtNode();
	node->_type = StmtType::Switch;
	node->_switch = switchNode;
	printf("N: stmt switch\n");
	return node;
}

StmtNode* StmtNode::createStmtEnumCase(EnumCaseNode* enumCase)
{
	auto node = new StmtNode();
	node->_type = StmtType::EnumCase;
	node->_enumCase = enumCase;
	printf("N: stmt enumCase\n");
	return node;
}

StmtNode* StmtNode::createStmtEnumDeclaration(EnumDeclarationNode* enumDeclaration)
{
	auto node = new StmtNode();
	node->_type = StmtType::EnumDeclaration;
	node->_enumDeclaration = enumDeclaration;
	printf("N: stmt enumDeclaration\n");
	return node;
}

StmtNode* StmtNode::createStmtStructDeclaration(StructDeclarationNode* structDeclaration)
{
	auto node = new StmtNode();
	node->_type = StmtType::StructDeclaration;
	node->_structDeclaration = structDeclaration;
	printf("N: stmt structDeclaration\n");
	return node;
}

StmtNode* StmtNode::createStmtConstructorDecl(ConstructorDeclNode* constructor)
{
	auto node = new StmtNode();
	node->_type = StmtType::ConstructorDecl;
	node->_constructorDecl = constructor;
	printf("N: stmt constructorDecl\n");
	return node;
}

StmtNode* StmtNode::createStmtDestructorDecl(DestructorDeclNode* destructor)
{
	auto node = new StmtNode();
	node->_type = StmtType::DestructorDecl;
	node->_destructorDecl = destructor;
	printf("N: stmt destructorDecl\n");
	return node;
}

StmtNode* StmtNode::createClassDecl(ClassDeclNode* classDecl)
{
	auto node = new StmtNode();
	node->_type = StmtType::ClassDecl;
	node->_classDecl = classDecl;
	printf("N: stmt classDecl\n");
	return node;
}

StmtNode* StmtNode::createDefaultClassConstructor(std::string baseClassName)
{
	auto node = new StmtNode();
	node->_type = StmtType::DefaultConstructor;
	node->_defaultConstructorBaseName = baseClassName;
	return node;
}

void StmtNode::generateDot(std::ofstream& file)
{
	switch (this->_type)
	{
	case StmtType::Expr:
		file << dotLabel(this->_id, "ExprStmt");
		file << dotConnection(this->_id, this->_expr->_id);
		this->_expr->generateDot(file);
		break;
	case StmtType::VarDeclarationList:
		file << dotLabel(this->_id, "VarDeclarationStmt");
		file << dotConnection(this->_id, this->_varDeclList->_id);
		this->_varDeclList->generateDot(file);
		break;
	case StmtType::Assignment:
		if (this->_assignArrayElem)
		{
			file << dotLabel(this->_id, "ArrayElemAssignment");
			file << dotConnectionWithLabel(this->_id, this->_assignArray->_id, "array");
			file << dotConnectionWithLabel(this->_id, this->_assignArrayIndex->_id, "index");
			file << dotConnectionWithLabel(this->_id, this->_assignRight->_id, "value");
			this->_assignArray->generateDot(file);
			this->_assignArrayIndex->generateDot(file);
			this->_assignRight->generateDot(file);
		}
		else
		{
			file << dotLabel(this->_id, "AssignmentStmt");
			file << dotConnectionWithLabel(this->_id, this->_assignLeft->_id, "left");
			file << dotConnectionWithLabel(this->_id, this->_assignRight->_id, "right");
			this->_assignLeft->generateDot(file);
			this->_assignRight->generateDot(file);
		}
		break;
	case StmtType::Return:
		file << dotLabel(this->_id, "ReturnStmt");
		file << dotConnection(this->_id, this->_return->_id);
		this->_return->generateDot(file);
		break;
	case StmtType::Loop:
		file << dotLabel(this->_id, "LoopStmt");
		file << dotConnection(this->_id, this->_loop->_id);
		this->_loop->generateDot(file);
		break;
	case StmtType::IfElse:
		file << dotLabel(this->_id, "IfElseStmt");
		file << dotConnection(this->_id, this->_ifElse->_id);
		this->_ifElse->generateDot(file);
		break;
	case StmtType::FuncDecl:
		file << dotLabel(this->_id, "FuncDeclStmt");
		file << dotConnection(this->_id, this->_funcDecl->_id);
		this->_funcDecl->generateDot(file);
		break;
	case StmtType::Throw:
		file << dotLabel(this->_id, "ThrowStmt");
		file << dotConnection(this->_id, this->_throw->_id);
		this->_throw->generateDot(file);
		break;
	case StmtType::Try:
		file << dotLabel(this->_id, "TryStmt");
		file << dotConnection(this->_id, this->_tryNode->_id);
		this->_tryNode->generateDot(file);
		break;
	case StmtType::Do:
		file << dotLabel(this->_id, "DoStmt");
		file << dotConnection(this->_id, this->_do->_id);
		this->_do->generateDot(file);
		break;
	case StmtType::Switch:
		file << dotLabel(this->_id, "SwitchStmt");
		file << dotConnection(this->_id, this->_switch->_id);
		this->_switch->generateDot(file);
		break;
	case StmtType::EnumCase:
		file << dotLabel(this->_id, "EnumCaseStmt");
		file << dotConnection(this->_id, this->_enumCase->_id);
		this->_enumCase->generateDot(file);
		break;
	case StmtType::EnumDeclaration:
		file << dotLabel(this->_id, "EnumDeclarationStmt");
		file << dotConnection(this->_id, this->_enumDeclaration->_id);
		this->_enumDeclaration->generateDot(file);
		break;
	case StmtType::StructDeclaration:
		file << dotLabel(this->_id, "StructDeclarationStmt");
		file << dotConnection(this->_id, this->_structDeclaration->_id);
		this->_structDeclaration->generateDot(file);
		break;
	case StmtType::ConstructorDecl:
		file << dotLabel(this->_id, "ConstructorDeclStmt");
		file << dotConnection(this->_id, this->_constructorDecl->_id);
		this->_constructorDecl->generateDot(file);
		break;
	case StmtType::DestructorDecl:
		file << dotLabel(this->_id, "DestructorDeclStmt");
		file << dotConnection(this->_id, this->_destructorDecl->_id);
		this->_destructorDecl->generateDot(file);
		break;
	case StmtType::ClassDecl:
		file << dotLabel(this->_id, "ClassDeclStmt");
		file << dotConnection(this->_id, this->_classDecl->_id);
		this->_classDecl->generateDot(file);
		break;
	case StmtType::DefaultConstructor:
		file << dotLabel(this->_id, "DefaultConstructor for class " + this->_defaultConstructorBaseName);
		break;
	default:
		throw std::runtime_error("Unknown type!");
		break;
	}
}

SemanticsBase* StmtNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	std::set<StmtType> topLevelStmtTypes = { StmtType::ClassDecl };

	bool isTopLevel = topLevelStmtTypes.count(this->_type) != 0;
	if (!isTopLevel)
	{
		// For functions, constructors and field declarations
		if (this->_type == StmtType::FuncDecl || this->_type == StmtType::ConstructorDecl || (this->_type == StmtType::VarDeclarationList && this->_varDeclList->isFieldDecl()))
		{
			std::set<SemanticsBase*> ignore = {};

			StmtNode* parentClass = nullptr;
			while (parentClass == nullptr)
			{
				auto stmt = stack.getClosest<StmtNode>(ignore);
				if (stmt == nullptr)
					break;

				if (stmt->_type != StmtType::ClassDecl)
				{
					ignore.emplace(stmt);
				}
				else
				{
					parentClass = stmt;
					break;
				}
			}

			bool isInsideClass = parentClass != nullptr && parentClass != this;

			if (!isInsideClass)
				throw std::runtime_error("All function declarations must me inside a class!");
		}
		else
		{
			std::set<SemanticsBase*> ignore = {};

			StmtNode* parentMethod = nullptr;
			while (parentMethod == nullptr)
			{
				auto stmt = stack.getClosest<StmtNode>(ignore);
				if (stmt == nullptr)
					break;

				if (stmt->_type != StmtType::FuncDecl && stmt->_type != StmtType::ConstructorDecl)
				{
					ignore.emplace(stmt);
				}
				else
				{
					parentMethod = stmt;
					break;
				}
			}

			bool isInsideMethod = parentMethod != nullptr && parentMethod != this;

			if (!isInsideMethod)
				throw std::runtime_error("All low level stmts must me inside a method!");
		}
	}

	if (this->_type == StmtType::Expr)
	{
		this->_expr = _expr->semanticsTransform(stack)->typecast<ExprNode>();
	}
	else if (this->_type == StmtType::Assignment) {
		this->_assignLeft = _assignLeft->semanticsTransform(stack)->typecast<ExprNode>();
		this->_assignRight = _assignRight->semanticsTransform(stack)->typecast<ExprNode>();
	}
	else if (this->_type == StmtType::ClassDecl)
	{
		this->_classDecl = this->_classDecl->semanticsTransform(stack)->typecast<ClassDeclNode>();
	}
	else if (this->_type == StmtType::FuncDecl)
	{
		this->_funcDecl->semanticsTransform(stack);
	}
	else if (this->_type == StmtType::ConstructorDecl)
	{
		this->_constructorDecl = this->_constructorDecl->semanticsTransform(stack)->typecast<ConstructorDeclNode>();
	}
	else if (this->_type == StmtType::VarDeclarationList)
	{
		this->_varDeclList->semanticsTransform(stack);
	}
	else if (this->_type == StmtType::IfElse)
	{
		this->_ifElse = this->_ifElse->semanticsTransform(stack)->typecast<IfElseNode>();
	}
	else if (this->_type == StmtType::Loop)
	{
		this->_loop = this->_loop->semanticsTransform(stack)->typecast<LoopNode>();
	}
	else if (this->_type == StmtType::DefaultConstructor)
	{
		//do nothing
	}
	else if (this->_type == StmtType::Return)
	{
		this->_return = this->_return->semanticsTransform(stack)->typecast<ReturnNode>();
	}
	else {
		throw std::runtime_error("Unsupported stmnt with enum type " + std::to_string(this->_type) + "!");
	}
	return this;
}

void StmtNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope, bool initialScan)
{
	std::set<StmtType> allowedOnInitialScan = { StmtType::ClassDecl, StmtType::ConstructorDecl, StmtType::FuncDecl, StmtType::VarDeclarationList };

	if (initialScan && allowedOnInitialScan.count(this->_type) == 0)
		return;

	switch (this->_type)
	{
	case StmtType::FuncDecl:
		if (currentMethod != nullptr)
			throw std::runtime_error("Nested methods are not supported!");

		if (currentClass == nullptr)
			throw std::runtime_error("Methods must be inside a class!");

		this->_funcDecl->fillTable(classTable, currentClass, currentMethod, currentScope, initialScan);
		break;
	case StmtType::ConstructorDecl:
		if (currentMethod != nullptr)
			throw std::runtime_error("Nested constructors are not supported!");

		if (currentClass == nullptr)
			throw std::runtime_error("Constructor must be inside the class!");

		this->_constructorDecl->fillTable(classTable, currentClass, currentMethod, currentScope, initialScan);
		break;
	case StmtType::ClassDecl:
		if (currentClass != nullptr)
			throw std::runtime_error("Nested classes are not supported!");

		if (currentMethod != nullptr)
			throw std::runtime_error("Class declaration inside methods is not supported!");

		this->_classDecl->fillTable(classTable, currentClass, currentMethod, currentScope, initialScan);
		break;
	case StmtType::Expr:
		if (currentClass == nullptr)
			throw std::runtime_error("Expr stmt must be associated with a class!");

		if (currentMethod == nullptr)
			throw std::runtime_error("Expr stmt must be inside a method!");

		this->_expr->fillTable(classTable, currentClass, currentMethod, currentScope);
		break;
	case StmtType::VarDeclarationList:
		if (currentClass == nullptr)
			throw std::runtime_error("Expr stmt must be associated with a class!");

		// if currentMethod == null - it is class field
		this->_varDeclList->fillTable(classTable, currentClass, currentMethod, currentScope, initialScan);
		break;
	case StmtType::Assignment:
		{
			if (currentClass == nullptr)
				throw std::runtime_error("Expr stmt must be associated with a class!");

			if (currentMethod == nullptr)
				throw std::runtime_error("Expr stmt must be inside a method!");

			bool isLeftSubscript = this->_assignLeft->_type == ExprType::Subscript;

			if (isLeftSubscript)
			{
				this->_assignArrayElem = true;

				this->_assignArray = this->_assignLeft->_left;

				if (this->_assignArray->_type == ExprType::Id)
				{
					auto localVar = currentScope->findLocalVar(this->_assignArray->_stringValue);
					if (localVar == nullptr)
						throw std::runtime_error("Can't find local var array \"" + this->_assignArray->_stringValue + "\"!" + LINE_AND_FILE);

					bool canAssign = this->_assignmentSkipConstCheck || !localVar->isConst;

					if (!canAssign)
						throw std::runtime_error("Can't assign elem of local var array \"" + this->_assignArray->_stringValue + "\" as it's a const variable!" + LINE_AND_FILE);
				}

				auto assignArrayDesc = this->_assignArray->evaluateType(classTable, currentClass, currentMethod, currentScope)->toDescriptor();
				if (assignArrayDesc[0] != '[')
					throw std::runtime_error("Can't do array assignment for type with descriptor \"" + assignArrayDesc + "\"!" + LINE_AND_FILE);

				this->_assignArrayIndex = this->_assignLeft->_right;
				auto assignIndexDesc = this->_assignArrayIndex->evaluateType(classTable, currentClass, currentMethod, currentScope)->toDescriptor();

				if (assignIndexDesc != "I")
				{
					if (assignIndexDesc[0] != 'L' || classnameFromDescriptor(assignIndexDesc) != RTLHelper::_intC)
						throw std::runtime_error("Index for subcript must be of type int but got type with descriptor \"" + assignIndexDesc + "\"!" + LINE_AND_FILE);

					auto funcCall = FuncCallNode::createFuncCallNoArgs("toInt");
					funcCall->setAsExprAccess(this->_assignArrayIndex);
					auto newRight = ExprNode::createFuncCall(funcCall);
					this->_assignArrayIndex = newRight;
				}

				auto rightDesc = _assignRight->evaluateType(classTable, currentClass, currentMethod, currentScope)->toDescriptor();

				auto arrayElemDesc = assignArrayDesc.substr(1);

				if (arrayElemDesc != rightDesc)
					throw std::runtime_error("Can't assign expr with descriptor \"" + 
						rightDesc + "\" to array element with descriptor \"" + arrayElemDesc + "\"!" + LINE_AND_FILE);

				this->_assignArray->fillTable(classTable, currentClass, currentMethod, currentScope);
				this->_assignArrayIndex->fillTable(classTable, currentClass, currentMethod, currentScope);
				this->_assignRight->fillTable(classTable, currentClass, currentMethod, currentScope);
			}
			else
			{
				this->_assignArrayElem = false;

				bool leftIsId = this->_assignLeft->_type == ExprType::Id;
				bool leftIsField = this->_assignLeft->_type == ExprType::FieldAccess;

				if (!leftIsId && !leftIsField)
					throw std::runtime_error("Assignment can't be used with left side expr of enum type" + std::to_string(this->_assignLeft->_type) + "!" + LINE_AND_FILE);

				if (leftIsField)
				{
					//FIXME check if field is const
					bool canAssign = this->_assignmentSkipConstCheck || true;

					if (!canAssign)
						throw std::runtime_error("Field can't be assigned, it is a constant!" + LINE_AND_FILE);
				}
				else
				{
					if (currentMethod == nullptr)
						throw std::runtime_error("Assignment can't be used outside of a method!" + LINE_AND_FILE);

					auto leftLocalVar = currentScope->findLocalVar(this->_assignLeft->_stringValue);

					if (leftLocalVar == nullptr)
						throw std::runtime_error("Variable \"" + this->_assignLeft->_stringValue + "\" not found!" + LINE_AND_FILE);

					bool canAssign = this->_assignmentSkipConstCheck || !leftLocalVar->isConst;

					if (!canAssign)
						throw std::runtime_error("Variable \"" + this->_assignLeft->_stringValue + "\" is a constant!" + LINE_AND_FILE);
				}				

				this->_assignLeft->fillTable(classTable, currentClass, currentMethod, currentScope);
				this->_assignRight->fillTable(classTable, currentClass, currentMethod, currentScope);

				auto leftDesc = this->_assignLeft->evaluateType(classTable, currentClass, currentMethod, currentScope)->toDescriptor();
				auto rightDesc = _assignRight->evaluateType(classTable, currentClass, currentMethod, currentScope)->toDescriptor();

				if (leftDesc != rightDesc)
				{
					bool isLeftClass = leftDesc[0] == 'L';
					bool isRightClass = rightDesc[0] == 'L';

					if (!isLeftClass || !isRightClass)
					{
						// Check if our can be downcasted to required (for arrays of classes)
						bool isLeftArray = leftDesc[0] == '[';
						bool isRightArray = rightDesc[0] == '[';

						if (isLeftArray && isRightArray)
						{
							bool isLeftArrayOfClass = leftDesc.find('L') != -1;
							bool isRightArrayOfClass = rightDesc.find('L') != -1;

							if (!isLeftArrayOfClass || !isRightArrayOfClass || !classTable->isClassDerivedFromClass(classnameFromDescriptor(rightDesc), classnameFromDescriptor(leftDesc)))
							{
								throw std::runtime_error("Type mismatch in assignment of var \"" + this->_assignLeft->_stringValue + "\"! It's current type has descriptor \"" +
									leftDesc + "\" while trying to assign type with has descriptor \"" + rightDesc + "\"!" + LINE_AND_FILE);
							}
						}
						else
						{
							throw std::runtime_error("Type mismatch in assignment of var \"" + this->_assignLeft->_stringValue + "\"! It's current type has descriptor \"" +
								leftDesc + "\" while trying to assign type with has descriptor \"" + rightDesc + "\"!" + LINE_AND_FILE);
						}
					}
					else
					{
						//Check if expr can be downcasted to type
						if (!classTable->isClassDerivedFromClass(classnameFromDescriptor(rightDesc), classnameFromDescriptor(leftDesc)))
							throw std::runtime_error("Type mismatch in assignment of var \"" + this->_assignLeft->_stringValue + "\"! It's current type has descriptor \"" +
								leftDesc + "\" while trying to assign type with has descriptor \"" + rightDesc + "\"!" + LINE_AND_FILE);
					}
				}

				this->_assignDesc = rightDesc;
			}
		}
		break;
	case StmtType::DefaultConstructor:
	{
		auto class_ = classTable->findClass(this->_defaultConstructorBaseName);
		this->_defaultConstructorClassRef = currentClass->getClassRefForExternalClass(class_);
		auto method = class_->findMethod("<init>", "()", false);

		if (method == nullptr)
			throw std::runtime_error("Can't find default constructor for class \"" + this->_defaultConstructorBaseName + "\"!" + LINE_AND_FILE);

		this->_defaultConstructorMethodRef = currentClass->getMethodRefForExternalMethod(method);
	}
		break;
	case StmtType::Return:
		this->_return->fillTable(classTable, currentClass, currentMethod, currentScope);
		break;
	case StmtType::IfElse:
		this->_ifElse->fillTable(classTable, currentClass, currentMethod, currentScope);
		break;
	case StmtType::Loop:
		this->_loop->fillTable(classTable, currentClass, currentMethod, currentScope);
		break;
	default:
		throw std::runtime_error("Unsupported stmnt with enum type " + std::to_string(this->_type) + "!");
		break;
	}
}

std::vector<char> StmtNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::vector<char> code = {};
	switch (this->_type)
	{
	case StmtType::Return:
		appendVecToVec(code, this->_return->generateCode(currentClass, currentMethod));
		break;
	case StmtType::Expr:
		appendVecToVec(code, this->_expr->generateCode(currentClass, currentMethod));
		break;
	case VarDeclarationList:
		appendVecToVec(code, this->_varDeclList->generateCode(currentClass, currentMethod));
		break;
	case StmtType::Assignment:
		if (this->_assignArrayElem)
		{
			appendVecToVec(code, this->_assignArray->generateCode(currentClass, currentMethod));
			appendVecToVec(code, this->_assignArrayIndex->generateCode(currentClass, currentMethod));

			appendVecToVec(code, this->_assignRight->generateCode(currentClass, currentMethod));

			appendVecToVec(code, jvm::aastore());
		}
		else
		{
			if (this->_assignLeft->_type == ExprType::Id) {
				appendVecToVec(code, this->_assignRight->generateCode(currentClass, currentMethod));
				int varNum = this->_assignLeft->_idLocalVar->localId;
				if (this->_assignDesc == "I")
				{
					appendVecToVec(code, jvm::istore(varNum));
				}
				else if (this->_assignDesc[0] == 'L')
				{
					appendVecToVec(code, jvm::astore(varNum));
				}
				else if (this->_assignDesc[0] == '[')
				{
					appendVecToVec(code, jvm::astore(varNum));
				}
				else
				{
					throw std::runtime_error("Can't assign descriptor " + this->_assignDesc + "!" + LINE_AND_FILE);
				}

			}
			else if (this->_assignLeft->_type == ExprType::FieldAccess) {
				if (!this->_assignLeft->_isStaticFieldAccess)
				{
					appendVecToVec(code, this->_assignLeft->_fieldAccessExpr->generateCode(currentClass, currentMethod));
					appendVecToVec(code, this->_assignRight->generateCode(currentClass, currentMethod));
					appendVecToVec(code, jvm::putfield(this->_assignLeft->_fieldRef));

				}
				else {
					appendVecToVec(code, this->_assignRight->generateCode(currentClass, currentMethod));
					appendVecToVec(code, jvm::putStatic(this->_assignLeft->_fieldRef));
				}


			}
			else {
				throw std::runtime_error("Unsupported assignment with left operand type: " + std::to_string(this->_assignLeft->_type) + "!");
			}
		}
		break;
	case StmtType::DefaultConstructor:
		appendVecToVec(code, jvm::aload(0));
		appendVecToVec(code, jvm::invokespecial(this->_defaultConstructorMethodRef));
		break;
	case StmtType::IfElse:
		appendVecToVec(code, this->_ifElse->generateCode(currentClass, currentMethod));
		break;
	case StmtType::Loop:
		appendVecToVec(code, this->_loop->generateCode(currentClass, currentMethod));
		break;
	case StmtType::FuncDecl:

		
		break;
	default:
		throw std::runtime_error("Can't generate code for stmnt with type " + std::to_string(this->_type) + "!");
		break;
	}
	return code;
}

std::string StmtListNode::getName()
{
	return "StmtList";
}

SemanticsBase* StmtListNode::semanticsTransform(SemanticsStack stack)
{
	return SemanticsBase::semanticsTransformVector<StmtNode>(stack, this, _vec);
}

void StmtListNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope, bool initialScan)
{
	for (auto& elem : _vec)
	{
		elem->fillTable(classTable, currentClass, currentMethod, currentScope, initialScan);
	}
}

std::vector<char> StmtListNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::vector<char> code = {};

	for (auto& elem : _vec)
	{
		appendVecToVec(code, elem->generateCode(currentClass, currentMethod));
	}

	return code;
}
