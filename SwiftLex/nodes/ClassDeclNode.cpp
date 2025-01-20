#include "ClassDeclNode.h"
#include "AccessModifierNode.h"
#include "StmtNode.h"

ClassDeclNode* ClassDeclNode::createClass(std::string name, StmtListNode* body)
{
    auto node = new ClassDeclNode();
    node->_type = ClassDeclType::NoBaseClass;
    node->_name = name;
    
    if (body == nullptr)
    {
        node->_hasBody = false;
    }
    else
    {
        node->_hasBody = true;
        node->_body = body;
    }

    return node;
}

ClassDeclNode* ClassDeclNode::createClassWithBaseClass(std::string name, StmtListNode* body, std::string baseClass)
{
    auto node = new ClassDeclNode();
    node->_type = ClassDeclType::HasBaseClass;
    node->_name = name;
    node->_baseClassName = baseClass;

    if (body == nullptr)
    {
        node->_hasBody = false;
    }
    else
    {
        node->_hasBody = true;
        node->_body = body;
    }

    return node;
}

ClassDeclNode* ClassDeclNode::addModifiers(AccessModifierListNode* modifiers)
{
    this->_hasModifiers = true;
    this->_modifiers = modifiers;
    return this;
}

void ClassDeclNode::generateDot(std::ofstream& file)
{
    if (this->_hasModifiers)
    {
        file << dotConnectionWithLabel(this->_id, this->_modifiers->_id, "modifiers");
        this->_modifiers->generateDot(file);
    }

    std::string extraInfo = "";
    if (!this->_hasBody)
        extraInfo += "\nno body";


    if (this->_type == ClassDeclType::NoBaseClass)
    {
        file << dotLabel(this->_id, "ClassDecl\nidName: " + this->_name + "\nno baseClass" + extraInfo);
    }
    else
    {
        file << dotLabel(this->_id, "ClassDecl\nidName: " + this->_name + "\nbaseClass: " + this->_baseClassName + extraInfo);
    }

    if (this->_hasBody)
    {
        file << dotConnectionWithLabel(this->_id, this->_body->_id, "body");
        this->_body->generateDot(file);
    }

}

SemanticsBase* ClassDeclNode::semanticsTransform(SemanticsStack stack)
{
    stack.push(this);
    this->_body = this->_body->semanticsTransform(stack)->typecast<StmtListNode>();
    return this;
}
