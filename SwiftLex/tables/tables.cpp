#include "tables.h"
#include "../nodes/FuncDeclNode.h"
#include "../nodes/FuncDeclArgNode.h"
#include "../nodes/TypeNode.h"
#include "../nodes/ClassDeclNode.h"
#include "../nodes/ConstructorDeclNode.h"
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

    this->findOrAddConstant(Utf8_C, "Code");
    this->findOrAddConstant(Utf8_C, "LocalVariableTable");
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
    this->constants = new ConstantTable();
    this->methods = new MethodTable();
    this->externalMethods = new ExternalMethodTable();
    this->fields = new FieldTable();

    this->nameStr = name;
    auto nameNum = constants->findOrAddConstant(ConstantType::Utf8_C, name);
    auto classNum = constants->findOrAddConstant(ConstantType::Class_C, "", 0, 0, nameNum);

    this->name = nameNum;
    this->thisClass = classNum;

    auto superNameNum = constants->findOrAddConstant(ConstantType::Utf8_C, superName);
    auto superClassNum = constants->findOrAddConstant(ConstantType::Class_C, "", 0, 0, superNameNum);

    this->superName = superNameNum;
    this->superClass = superClassNum;

    this->addRtlExternalMethods();
}

void ClassTableElement::addRtlExternalMethods()
{
    this->externalMethods->addMethod(this->constants, "InputOutput", "print", "(I)V");
}

MethodTableElement* ClassTableElement::addMethod(std::string name, StmtListNode* body, std::string descriptor, std::vector<MethodAccessFlag> flags)
{
    auto newMethod = new MethodTableElement(this->constants, name, body, descriptor, flags);
    this->methods->methods[newMethod->strName] = newMethod;
    return newMethod;
}

FieldElement* ClassTableElement::addField(std::vector<FieldAccessFlag> flags, std::string name, TypeNode* type)
{
    return this->fields->addField(flags, name, type, this->constants);
}

MethodTableElement::MethodTableElement(ConstantTable* constants, std::string name, StmtListNode* body, std::string descriptor, std::vector<MethodAccessFlag> flags)
{
    this->strName = name;
    this->methodName = constants->findOrAddConstant(Utf8_C, this->strName);
    this->_body = body;
    this->strDesc = descriptor;

    this->varTable = new LocalVariableTable();

    this->descriptor = constants->findOrAddConstant(Utf8_C, this->strDesc);
    this->accessFlag = 0;
    for (auto& flag : flags)
    {
        this->accessFlag += (int)flag;
    }
}

LocalVariableElement::LocalVariableElement(int localId, std::string name, TypeNode* type, ConstantTable* constantTable)
{
    this->name = name;
    this->_type = type;
    this->localId = localId;
    this->nameIndex = constantTable->findOrAddConstant(Utf8_C, name);
    this->descriptorIndex = constantTable->findOrAddConstant(Utf8_C, descriptorForType(type));
}

LocalVariableElement* LocalVariableTable::addLocalVar(std::string name, TypeNode* type, ConstantTable* constantTable)
{
    if (items.find(name) != items.cend())
        throw std::runtime_error("Local variable with name " + name + " already exists!");
    
    int newLocalId = this->items.size();
    this->items[name] = new LocalVariableElement(newLocalId, name, type, constantTable);
    return this->items[name];
}

LocalVariableElement* LocalVariableTable::findLocalVar(std::string name)
{
    if (items.find(name) == items.cend())
        throw std::runtime_error("Local variable with name \"" + name + "\" doesn't exist!");

    return this->items[name];
}

ClassTableElement* ClassTable::addClass(std::string name, std::string superName)
{
    auto newClass = new ClassTableElement(name, superName);
    this->items[name] = newClass;
    return newClass;
}

ExternalMethodTableElement::ExternalMethodTableElement(std::string name, int nameRef, std::string descriptor, int descriptorRef, int methodRef)
{
    this->_name = name;
    this->_nameRef = nameRef;
    this->_descriptor = descriptor;
    this->_descriptorRef = descriptorRef;
    this->_methodRef = methodRef;
}

ExternalMethodTableElement* ExternalMethodTable::addMethod(ConstantTable* constants, std::string className, std::string methodName, std::string descriptor)
{
    auto classNameRef = constants->findOrAddConstant(Utf8_C, className);
    auto classRef = constants->findOrAddConstant(Class_C, "", 0, 0, classNameRef, 0);
    auto methodNameRef = constants->findOrAddConstant(Utf8_C, methodName);
    auto descriptorRef = constants->findOrAddConstant(Utf8_C, descriptor);
    auto methodNameAndTypeRef = constants->findOrAddConstant(NameAndType_C, "", 0, 0, methodNameRef, descriptorRef);
    auto methodMethodRefRef = constants->findOrAddConstant(MethodRef_C, "", 0, 0, classRef, methodNameAndTypeRef);

    auto newMethod = new ExternalMethodTableElement(methodName, methodNameRef, descriptor, descriptorRef, methodMethodRefRef);
    this->methods[methodName] = newMethod;
    return newMethod;
}

FieldElement::FieldElement(std::vector<FieldAccessFlag> flags, std::string name, TypeNode* type, ConstantTable* constantTable)
{
    this->name = name;
    this->type = type;
    this->nameIndex = constantTable->findOrAddConstant(Utf8_C, name);
    this->descriptorIndex = constantTable->findOrAddConstant(Utf8_C, descriptorForType(type));
    this->accessFlag = 0;
    for (auto& flag : flags)
    {
        this->accessFlag += (int)flag;
    }
}

FieldElement* FieldTable::addField(std::vector<FieldAccessFlag> flags, std::string name, TypeNode* type, ConstantTable* constantTable)
{
    if (items.find(name) != items.cend())
        throw std::runtime_error("Class field with name " + name + " already exists!");

    this->items[name] = new FieldElement(flags, name, type, constantTable);
    return this->items[name];
}

FieldElement* FieldTable::findField(std::string name)
{
    if (items.find(name) == items.cend())
        throw std::runtime_error("Class field with name \"" + name + "\" doesn't exist!");

    return this->items[name];
}
