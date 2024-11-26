#include "ClassDeclNode.h"
#include "AccessModifierNode.h"
#include "StmtNode.h"
#include "TypeForGenericNode.h"

ClassDeclNode* ClassDeclNode::createClass(std::string name, StmtListNode* body, TypeForGenericListNode* genericTypes)
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

    if (genericTypes == nullptr)
    {
        node->_isGeneric = false;
    }
    else
    {
        node->_isGeneric = true;
        node->_typesForGenericList = genericTypes;
    }

    return node;
}

ClassDeclNode* ClassDeclNode::createClassWithBaseClass(std::string name, StmtListNode* body, TypeForGenericListNode* genericTypes, std::string baseClass)
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

    if (genericTypes == nullptr)
    {
        node->_isGeneric = false;
    }
    else
    {
        node->_isGeneric = true;
        node->_typesForGenericList = genericTypes;
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

    if (this->_isGeneric)
    {
        file << dotConnectionWithLabel(this->_id, this->_typesForGenericList->_id, "generic types");
        this->_typesForGenericList->generateDot(file);
    }

    if (this->_hasBody)
    {
        file << dotConnectionWithLabel(this->_id, this->_body->_id, "body");
        this->_body->generateDot(file);
    }

}
