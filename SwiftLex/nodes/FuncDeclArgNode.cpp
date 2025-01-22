#include "FuncDeclArgNode.h"
#include "TypeNode.h"
#include "ExprNode.h"

FuncDeclArgNode* FuncDeclArgNode::createPositionalArg(std::string argName, TypeNode* argType, ExprNode* argValue)
{
	auto node = new FuncDeclArgNode();
	node->_type = FuncDeclArgType::positional;
	node->_argName = argName;
	node->_argType = argType;
	if (argValue == nullptr)
	{
		node->_hasDefaultValue = false;
	}
	else
	{
		node->_hasDefaultValue = true;
		node->_argDefaultValue = argValue;
	}
	return node;
}

FuncDeclArgNode* FuncDeclArgNode::createLabeledArg(std::string argName, TypeNode* argType, ExprNode* argValue)
{
	auto node = new FuncDeclArgNode();
	node->_type = FuncDeclArgType::requiresLabel;
	node->_argName = argName;
	node->_argType = argType;
	if (argValue == nullptr)
	{
		node->_hasDefaultValue = false;
	}
	else
	{
		node->_hasDefaultValue = true;
		node->_argDefaultValue = argValue;
	}
	return node;
}

void FuncDeclArgNode::generateDot(std::ofstream& file)
{
	if (_type == FuncDeclArgType::positional)
	{
		file << dotLabel(this->_id, "FuncDeclArg\npositional\nargName: " + this->_argName + (std::string)(this->_hasDefaultValue ? "\nwith deault value" : "\nno default value"));
		file << dotConnectionWithLabel(this->_id, this->_argType->_id, "type");
		this->_argType->generateDot(file);
		if (this->_hasDefaultValue)
		{
			file << dotConnectionWithLabel(this->_id, this->_argDefaultValue->_id, "default value");
			this->_argDefaultValue->generateDot(file);
		}
	}
	else
	{
		file << dotLabel(this->_id, "FuncDeclArg\nrequiresLabel\nargName: " + this->_argName + (std::string)(this->_hasDefaultValue ? "\nwith deault value" : "\nno default value"));
		file << dotConnectionWithLabel(this->_id, this->_argType->_id, "type");
		this->_argType->generateDot(file);
		if (this->_hasDefaultValue)
		{
			file << dotConnectionWithLabel(this->_id, this->_argDefaultValue->_id, "default value");
			this->_argDefaultValue->generateDot(file);
		}
	}
}

SemanticsBase* FuncDeclArgNode::semanticsTransform(SemanticsStack stack)
{
	stack.push(this);
	if (this->_isAlreadyTransformed)
		return this;

	this->_argType = this->_argType->semanticsTransform(stack)->typecast<TypeNode>();
	if (this->_hasDefaultValue)
		this->_argDefaultValue = this->_argDefaultValue->semanticsTransform(stack)->typecast<ExprNode>();

	return this;
}

std::string FuncDeclArgListNode::getName()
{
	return "FuncDeclArgList";
}

SemanticsBase* FuncDeclArgListNode::semanticsTransform(SemanticsStack stack)
{
	return SemanticsBase::semanticsTransformVector<FuncDeclArgNode>(stack, this, _vec);
}
