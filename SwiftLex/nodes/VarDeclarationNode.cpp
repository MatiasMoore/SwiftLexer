#include "VarDeclarationNode.h"
#include "TypeNode.h"
#include "ExprNode.h"
#include "AccessModifierNode.h"
#include "StmtNode.h"
#include "../generation/generationHelpers.h"
#include "../ExceptionHelper.h"
#include "ClassDeclNode.h"
#include "ConstructorDeclNode.h"
#include "AccessModifierNode.h"

VarDeclarationNode* VarDeclarationNode::createFromValue(std::string varName, ExprNode* value)
{
	auto node = new VarDeclarationNode();
	node->_type = ValueKnown;
	node->_valueNode = value;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::createFromType(std::string varName, TypeNode* type)
{
	auto node = new VarDeclarationNode();
	node->_type = TypeKnown;
	node->_typeNode = type;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::createFromValueAndType(std::string varName, ExprNode* value, TypeNode* type)
{
	auto node = new VarDeclarationNode();
	node->_type = ValueAndTypeKnown;
	node->_valueNode = value;
	node->_typeNode = type;
	node->_varName = varName;
	return node;
}

VarDeclarationNode* VarDeclarationNode::addModifiers(AccessModifierListNode* modifiers)
{
	this->_hasModifiers = true;
	this->_modifiers = modifiers;
	return this;
}

VarDeclarationNode* VarDeclarationNode::setAsFieldDecl(bool flag)
{
	this->_isFieldDecl = flag;
	return this;
}

void VarDeclarationNode::generateDot(std::ofstream& file)
{
	file << dotLabel(this->_id, "VarDecl: " + _varName);
	if (this->_hasModifiers)
	{
		file << dotConnectionWithLabel(this->_id, this->_modifiers->_id, "modifiers");
		this->_modifiers->generateDot(file);
	}
	if (this->_type == VarDeclType::TypeKnown || this->_type == VarDeclType::ValueAndTypeKnown)
	{
		file << dotConnectionWithLabel(this->_id, this->_typeNode->_id, "type");
		this->_typeNode->generateDot(file);
	}
	if (this->_type == VarDeclType::ValueKnown || this->_type == VarDeclType::ValueAndTypeKnown)
	{
		file << dotConnectionWithLabel(this->_id, this->_valueNode->_id, "value");
		this->_valueNode->generateDot(file);
	}
}

void VarDeclarationNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, bool initialScan)
{
	if (initialScan)
	{
		// Class Field
		int constantValueIndex;
		if (this->_isFieldDecl)
		{
			if (_modifiers == nullptr)
			{
				throw std::runtime_error("Modifier missing for field: \"" + _varName + "\" for class \"" + currentClass->getClassName() + "\"");
			}

			if (this->_type == ValueAndTypeKnown)
			{
				// All done, required to add assign to <clinit> (if static) or <init> (if not static)
				this->_valueNode->fillTable(classTable, currentClass, currentMethod);

				// Check if var type and assignable type is equal
				auto varTypeDescriptor = this->_typeNode->toDescriptor();
				auto assignableValueType = this->_valueNode->evaluateType(classTable, currentClass, currentMethod);
				auto assignableValueDescriptor = assignableValueType->toDescriptor();
				if (varTypeDescriptor != assignableValueDescriptor) //maybe override "==" for TypeNode???
					throw std::runtime_error("Variable descriptor\"" + varTypeDescriptor + "\" does not match with assignable descriptor \"" + assignableValueDescriptor + "\" for field \"" + this->_varName + "\" in class \"" + currentClass->getClassName() + "\"" + LINE_AND_FILE);

				// Create field
				currentClass->addInternalFieldToConstantTable(
					this->_varName,
					this->_typeNode->toDescriptor(),
					this->_modifiers->getFieldAccessFlags()
				);
			}
			else if (this->_type == TypeKnown)
			{

				//Check if field not final (needs to be initialized)
				auto accessFlags = this->_modifiers->getMethodAccessFlags();
				bool isFinal = std::find(accessFlags.begin(), accessFlags.end(), M_ACC_FINAL) != accessFlags.end();
				bool isStatic = std::find(accessFlags.begin(), accessFlags.end(), M_ACC_STATIC) != accessFlags.end();
				if (isFinal)
					throw std::runtime_error("Final field needs to be initialized for field \"" + this->_varName + "\" in class \"" + currentClass->getClassName() + "\"");
				//TODO: remove after testing
				if (isStatic)
				{
					std::string warning = "WARNING: Field declaration for field \"" + this->_varName + "\" in class \"" + currentClass->getClassName() + "\" is missing initialization and not possible in swift. Remove after testing" + LINE_AND_FILE;
					std::cout << warning;
				}
				// Create field
				currentClass->addInternalFieldToConstantTable(
					this->_varName,
					this->_typeNode->toDescriptor(),
					this->_modifiers->getFieldAccessFlags()
				);
			}
			else if (this->_type == ValueKnown)
			{

				// Get type from assignable value
				this->_typeNode = this->_valueNode->evaluateType(classTable, currentClass, currentMethod);
					auto varTypeDescriptor = this->_typeNode->toDescriptor();

					// Create field
				currentClass->addInternalFieldToConstantTable(
						this->_varName,
						this->_typeNode->toDescriptor(),
					this->_modifiers->getFieldAccessFlags()
				);

				this->_type = TypeKnown;
			}
			else
			{
				throw std::runtime_error("Declaration usupported for field \"" + this->_varName + "\" in class \"" + currentClass->getClassName() + "\"" + LINE_AND_FILE);
			}
		}
	}
	else
	{

		// Local Variable
		if (currentMethod != nullptr)
		{
			switch (this->_type)
			{
			case ValueAndTypeKnown:
			{
				currentMethod->getVarTable()->addLocalVar(this->_varName, this->_typeNode->toDescriptor());

				//FIXME not sure if this must be called before evaluate type
				this->_valueNode->fillTable(classTable, currentClass, currentMethod);

				//Check if types are the same
				auto typeDesc = this->_typeNode->toDescriptor();
				auto exprDesc = this->_valueNode->evaluateType(classTable, currentClass, currentMethod)->toDescriptor();

				if (typeDesc != exprDesc)
				{
					bool isTypeClass = typeDesc[0] == 'L';
					bool isExprCLass = exprDesc[0] == 'L';

					if (!isTypeClass || !isExprCLass)
						throw std::runtime_error("Type mismatch in declaration of var \"" + this->_varName + "\"! Written type has descriptor \"" +
							typeDesc + "\" while expr has descriptor \"" + exprDesc + "\"!" + LINE_AND_FILE);

					//Check if expr can be downcasted to type
					if (!classTable->isClassDerivedFromClass(classnameFromDescriptor(exprDesc), classnameFromDescriptor(typeDesc)))
						throw std::runtime_error("Type mismatch in declaration of var \"" + this->_varName + "\"! Written type has descriptor \"" +
							typeDesc + "\" while expr has descriptor \"" + exprDesc + "\"!" + LINE_AND_FILE);
				}

				this->_type = VarDeclType::TypeKnown;
			}
				break;
			case TypeKnown:
				currentMethod->getVarTable()->addLocalVar(this->_varName, this->_typeNode->toDescriptor());
				break;
			case ValueKnown:
			{
				//FIXME not sure if this must be called before evaluate type
				this->_valueNode->fillTable(classTable, currentClass, currentMethod);

				auto evaluatedType = this->_valueNode->evaluateType(classTable, currentClass, currentMethod);
				currentMethod->getVarTable()->addLocalVar(this->_varName, evaluatedType->toDescriptor());
				this->_typeNode = evaluatedType;
				this->_type = VarDeclType::TypeKnown;
			}
			break;
			default:
				throw std::runtime_error("Critical error!" + LINE_AND_FILE);
				break;
			}
		}

	}
}

std::vector<char> VarDeclarationNode::generateCode(InternalClass * currentClass, InternalMethod * currentMethod)
{
	std::vector<char> code = {};

	if (this->_isFieldDecl)
	{
		switch (this->_type)
		{
		case TypeKnown:
		case ValueKnown:
		case ValueAndTypeKnown:
			//No code required for this type
			break;
		default:
			throw std::runtime_error("Var decl with enum type " + std::to_string(this->_type) + " is not supported!");
			break;
		}
	}
	else
	{
		switch (this->_type)
		{
		case TypeKnown:
			//No code required for this type
			break;
		default:
			throw std::runtime_error("Var decl with enum type " + std::to_string(this->_type) + " is not supported!");
			break;
		}
	}

	return code;
}

SemanticsBase* VarDeclarationNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	auto valueKnown = this->_type == ValueKnown || this->_type == ValueAndTypeKnown;
	auto typeKnown = this->_type == TypeKnown || this->_type == ValueAndTypeKnown;

	if (this->_isFieldDecl)
	{
		// Add default modiffiers
		if (!this->_hasModifiers)
		{
			this->_modifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(Internal));
			this->_hasModifiers = true;
		}
			
		auto modiffiersList = this->_modifiers->getFieldAccessFlags();
		bool isStatic = std::find(
			modiffiersList.begin(),
			modiffiersList.end(),
			FieldAccessFlag::F_ACC_STATIC
		) != modiffiersList.end();

		if (typeKnown)
		{
			this->_typeNode = this->_typeNode->semanticsTransform(stack)->typecast<TypeNode>();
		}

		if (valueKnown)
		{
			if (isStatic)
				throw std::runtime_error("Unsupported static var declaration for var" + _varName + LINE_AND_FILE);

			auto myClassDecl = stack.getClosest<ClassDeclNode>();
			auto myClassBody = myClassDecl->_body;
			if (myClassBody == nullptr)
				throw std::runtime_error("Critical error! Body of class" + myClassDecl->_name + " do not exist" + LINE_AND_FILE);

			this->_valueNode = this->_valueNode->semanticsTransform(stack)->typecast<ExprNode>();

			for (auto& stmt : myClassBody->_vec)
			{
				if (stmt->_type != StmtType::ConstructorDecl)
					continue;

				auto constructorDecl = stmt->_constructorDecl;

				auto assignStmt = StmtNode::createStmtAssignment(
					ExprNode::createFieldAccessExpr(
						ExprNode::createId("self"),
						this->_varName
					),
					_valueNode
				);

				if (constructorDecl->_hasBody)
				{
					constructorDecl->_body = constructorDecl->_body->appendNodeAtIndex(assignStmt, 0);
				}
				else
				{
					constructorDecl->_body = StmtListNode::createListNode(assignStmt);
					constructorDecl->_hasBody = true;
				}
			}
			
		}
	}
	else
	{
		if (this->_hasModifiers)
			throw std::runtime_error("Only field declaration can have modifiers!" + LINE_AND_FILE);

		// Create assignment if we know the value
		if (valueKnown)
		{
			this->_valueNode = this->_valueNode->semanticsTransform(stack)->typecast<ExprNode>();

			auto thisStmtList = stack.getClosest<StmtListNode>();
			if (thisStmtList == nullptr)
				throw std::runtime_error("Critical error! Unable to find stmt list for var decl!");

			auto thisStmt = stack.getClosest<StmtNode>();
			if (thisStmt == nullptr)
				throw std::runtime_error("Critical error! Unable to find stmt for var decl!");

			auto assignmentNode = StmtNode::createStmtAssignment(ExprNode::createId(this->_varName), this->_valueNode);

			thisStmtList->appendNodeAfterNode(assignmentNode, thisStmt);
		}

		if (typeKnown)
		{
			this->_typeNode = this->_typeNode->semanticsTransform(stack)->typecast<TypeNode>();
		}
	}

	return this;
}

std::string VarDeclarationListNode::getName()
{
	return "VarDeclList";
}

VarDeclarationListNode* VarDeclarationListNode::addModifiers(AccessModifierListNode* modifiers)
{
	for (auto& varDecl : this->_vec)
	{
		varDecl->addModifiers(modifiers);
	}
	return this;
}

VarDeclarationListNode* VarDeclarationListNode::setAsFieldDecl(bool flag)
{
	for (auto& elem : _vec)
	{
		elem->setAsFieldDecl(flag);
	}
	return this;
}

bool VarDeclarationListNode::isFieldDecl()
{
	for (auto& elem : _vec)
	{
		if (elem->_isFieldDecl)
			return true;
	}

	return false;
}

void VarDeclarationListNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, bool initialScan)
{
	for (auto& elem : _vec)
	{
		elem->fillTable(classTable, currentClass, currentMethod, initialScan);
	}
}

std::vector<char> VarDeclarationListNode::generateCode(InternalClass* currentClass, InternalMethod* currentMethod)
{
	std::vector<char> code = {};
	for (auto& elem : _vec)
	{
		appendVecToVec(code, elem->generateCode(currentClass, currentMethod));
	}
	return code;
}

SemanticsBase* VarDeclarationListNode::semanticsTransform(SemanticsStack stack)
{
	return SemanticsBase::semanticsTransformVector<VarDeclarationNode>(stack, this, _vec);
}
