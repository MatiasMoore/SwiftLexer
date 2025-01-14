#include "tables.h"
#include "../nodes/FuncDeclNode.h"
#include "../nodes/FuncDeclArgNode.h"
#include "../nodes/TypeNode.h"
#include "../generation/generationHelpers.h"

/* --------------------------- Ёлемент таблицы констант --------------------------- */

ConstantTableItem::ConstantTableItem(enum ConstantType type, int id, std::string utf8, int intVal, double dVal, int fRef, int secondRef)
{
    this->id = id;
    this->cnst = type;
    this->string;
    this->Integer = 0;
    this->Double = 0;
    this->firstRef = 0;
    this->secRef = 0;
    if (type == ConstantType::Utf8_C) { this->string = utf8; }
    else if (type == ConstantType::Integer_C) this->Integer = intVal;
    else if (type == ConstantType::Double_C) this->Double = dVal;
    else if (type == ConstantType::Class_C) this->firstRef = fRef;
    else if (type == ConstantType::String_C) this->firstRef = fRef;
    else if (type == ConstantType::NameAndType_C) { this->firstRef = fRef; this->secRef = secondRef; }
    else if (type == ConstantType::FieldRef_C) { this->firstRef = fRef; this->secRef = secondRef; }
    else if (type == ConstantType::MethodRef_C) { this->firstRef = fRef; this->secRef = secondRef; }
}

/* --------------------------- “аблица констант ---------------------------------- */

ConstantTable::ConstantTable()
{
    this->constants = std::map<int, class ConstantTableItem*>();
    this->constants[maxId] = new ConstantTableItem(Utf8_C, maxId, "Code");
    maxId++;
}

int ConstantTable::findOrAddConstant(enum ConstantType type, std::string utf8string, int intVal, double dVal, int fRef, int sRef)
{
    int constant = findConstant(type, utf8string, fRef, sRef, intVal, dVal); // Ќайти константу в списке констант.
    if (constant == -1) // —оздать новую константу, если константа не найдена.
    {
        constant = maxId++;
        if (type == ConstantType::Utf8_C) { constants[constant] = new ConstantTableItem(type, constant, utf8string); }
        else if (type == ConstantType::Class_C) { constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef); }
        else if (type == ConstantType::Integer_C) constants[constant] = new ConstantTableItem(type, constant, "", intVal);
        else if (type == ConstantType::Double_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, dVal);
        else if (type == ConstantType::NameAndType_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef, sRef);
        else if (type == ConstantType::FieldRef_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef, sRef);
        else if (type == ConstantType::MethodRef_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef, sRef);
        else if (type == ConstantType::String_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef);
    }
    // ¬ернуть результат.
    return constant;
}

int ConstantTable::findConstant(enum ConstantType type, std::string utf8string, int fRef, int secondRef, int intVal, double dVal)
{
    auto iterator = constants.cbegin();
    while (iterator != constants.cend()) // ѕока не конец таблицы...
    {
        if (type == iterator->second->cnst) // ≈сли тип константы совпадает с рассматриваемым типом...
        {
            switch (iterator->second->cnst)
            {
            case Utf8_C: // ¬ случае, если контанта - Utf8...
                if (std::string(utf8string) == std::string(iterator->second->string)) // ¬ернуть номер константы, если совпадают значени€ строк-констант Utf-8
                    return iterator->first;
                break;
            case Class_C: // ¬ случае, если константа - класс...
                if (iterator->second->firstRef == fRef) // ¬ернуть номер константы, если совпадают значени€ номеров констант-ссылок.
                    return iterator->first;
                break;
            case String_C:
                if (iterator->second->firstRef == fRef)
                    return iterator->first;
                break;
            case MethodRef_C: // ¬ случае, если константа - ссылка на метод...
                if (iterator->second->firstRef == fRef && iterator->second->secRef == secondRef) // ¬ернуть номер константы, если совпадают значени€ номеров констант-ссылок.
                    return iterator->first;
                break;
            case NameAndType_C: // ¬ случае, если константа - им€ и класс...
                if (iterator->second->firstRef == fRef && iterator->second->secRef == secondRef) // ¬ернуть номер константы, если совпадают значени€ номеров констант-ссылок.
                    return iterator->first;
                break;
            case Integer_C: // ¬ случае, если константа - литерал integer...
                if (iterator->second->Integer == intVal) // ¬ернуть номер константы, если совпадают значени€ номеров констант-ссылок.
                    return iterator->first;
                break;
            case Double_C: // ¬ случае, если константа - литерал double...
                if (iterator->second->Double == dVal) // ¬ернуть номер константы, если совпадают значени€ номеров констант-ссылок.
                    return iterator->first;
                break;
            case FieldRef_C: // ¬ случае, если константа - ссылка на поле...
                if (iterator->second->firstRef == fRef && iterator->second->secRef == secondRef)  // ¬ернуть номер константы, если совпадают значени€ номеров констант-ссылок.
                    return iterator->first;
                break;
            }
        }
        ++iterator; // ѕерейти к следующему элементу.
    }
    return -1;
}

ClassTableElement::ClassTableElement(std::string name, std::string superName)
{
    constants = new ConstantTable();
    methods = new MethodTable();
    auto nameNum = constants->findOrAddConstant(ConstantType::Utf8_C, name);
    auto classNum = constants->findOrAddConstant(ConstantType::Class_C, "", 0, 0, nameNum);

    this->name = nameNum;
    this->thisClass = classNum;

    auto superNameNum = constants->findOrAddConstant(ConstantType::Utf8_C, superName);
    auto superClassNum = constants->findOrAddConstant(ConstantType::Class_C, "", 0, 0, superNameNum);

    this->superName = superNameNum;
    this->superClass = superClassNum;
}

MethodTableElement::MethodTableElement(ConstantTable* constants, FuncDeclNode* funcDecl)
{
    this->strName = funcDecl->_idName;
    this->methodName = constants->findOrAddConstant(Utf8_C, this->strName);
    this->_funcDecl = funcDecl;
    this->strDesc = "(";

    this->varTable = new LocalVariableTable();

    if (funcDecl->_hasArgs)
    {
        for (auto& arg : funcDecl->_argList->_vec)
        {
            this->strDesc += descriptorForType(arg->_argType);
            this->varTable->findOrAddLocalVar(constants, arg->_argName, arg->_argType);
        }
    }
    this->strDesc += ")";

    if (funcDecl->_hasNonVoidReturn)
        this->strDesc += descriptorForType(funcDecl->_returnType);
    else
        this->strDesc += "V";

    this->descriptor = constants->findOrAddConstant(Utf8_C, this->strDesc);
}

LocalVariableElement::LocalVariableElement(ConstantTable* constants, std::string name, TypeNode* type, int isConst, int isInitial)
{
    this->name = name;
    this->nameNum = constants->findOrAddConstant(Utf8_C, this->name);
    this->_type = type;
    this->isConst = isConst;
    this->isInit = isInitial;
}

int LocalVariableTable::findOrAddLocalVar(ConstantTable* constants, std::string name, TypeNode* type, int isConst, int isInitial)
{
    if (items.find(name) == items.cend()) // ≈сли переменна€ с указанным именем не найдена.
    {
        items[name] = new LocalVariableElement(constants, name, type, isConst, isInitial);
        return items[name]->nameNum;
    }
    else
    {
        return items[name]->nameNum;
    }
}