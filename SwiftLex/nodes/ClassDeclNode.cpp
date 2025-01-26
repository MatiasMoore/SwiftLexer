#include "ClassDeclNode.h"
#include "AccessModifierNode.h"
#include "StmtNode.h"
#include "ConstructorDeclNode.h"
#include "../ExceptionHelper.h"

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

void ClassDeclNode::fillTable(ClassTable* classTable, InternalClass* currentClass, InternalMethod* currentMethod, VariableScope* currentScope, bool initialScan)
{
    if (initialScan)
    {
        currentClass = classTable->addInternalClass(this->_name, this->_type == ClassDeclType::HasBaseClass ? this->_baseClassName : "java/lang/Object");

        if (!this->_hasBody)
            throw std::runtime_error("Class decl can't have an empty body!" + LINE_AND_FILE);

        this->_scannedClass = currentClass;

        this->_body->fillTable(classTable, currentClass, currentMethod, currentScope, initialScan);
    }
    else
    {
        if (!this->_hasBody)
            throw std::runtime_error("Class decl can't have an empty body!" + LINE_AND_FILE);

        if (this->_scannedClass == nullptr)
            throw std::runtime_error("Critical error! Initial scan failed for class \"" + this->_name + "\"!" + LINE_AND_FILE);

        currentClass = this->_scannedClass;

        this->_body->fillTable(classTable, currentClass, currentMethod, currentScope, initialScan);
    }    
}

SemanticsBase* ClassDeclNode::semanticsTransform(SemanticsStack stack)
{
    stack.push(this);
    if (this->_isAlreadyTransformed)
        return this;

    bool hasConstructor = false;
    if (this->_hasBody)
    {
        for (auto& stmt : this->_body->_vec)
        {
            if (stmt->_type == StmtType::ConstructorDecl)
            {
                hasConstructor = true;
                break;
            }
        }
    }

    if (!hasConstructor)
    {
        auto constructorStmt = StmtNode::createStmtConstructorDecl(ConstructorDeclNode::createConstructor(nullptr, nullptr, false));

        if (this->_hasBody)
        {
            this->_body->appendNode(constructorStmt);
        }
        else
        {
            this->_body = StmtListNode::createListNode(constructorStmt);
            this->_hasBody = true;
        }
    }

    this->_body = this->_body->semanticsTransform(stack)->typecast<StmtListNode>();
    return this;
}
