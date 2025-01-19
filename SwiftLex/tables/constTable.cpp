#include "constTable.h"
#include "../ExceptionHelper.h"
/* --------------------------- “аблица констант ---------------------------------- */

ConstantTable::ConstantTable()
{
    this->constants = std::map<int, class ConstantTableItem*>();

    this->findOrAddConstant(Utf8_C, "Code");
    this->findOrAddConstant(Utf8_C, "LocalVariableTable");
    this->findOrAddConstant(Utf8_C, "ConstantValue");

}

int ConstantTable::addConstant(ConstantType type, std::string utf8string, int intVal, double dVal, int fRef, int sRef)
{
    int constant = findConstant(type, utf8string, fRef, sRef, intVal, dVal);
    if (constant != -1)
        throw std::runtime_error("Constant with type " + std::to_string(type) + " utf8string " + utf8string + " intVal " + std::to_string(intVal) + " dVal " + std::to_string(dVal) + " fRef " + std::to_string(fRef) + " sRef " + std::to_string(sRef) + " already exists!" + LINE_AND_FILE);

    constant = maxId++;
    if (type == ConstantType::Utf8_C) { constants[constant] = new ConstantTableItem(type, constant, utf8string); }
    else if (type == ConstantType::Class_C) { constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef); }
    else if (type == ConstantType::Integer_C) constants[constant] = new ConstantTableItem(type, constant, "", intVal);
    else if (type == ConstantType::Double_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, dVal);
    else if (type == ConstantType::NameAndType_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef, sRef);
    else if (type == ConstantType::FieldRef_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef, sRef);
    else if (type == ConstantType::MethodRef_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef, sRef);
    else if (type == ConstantType::String_C) constants[constant] = new ConstantTableItem(type, constant, "", NULL, NULL, fRef);

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

int ConstantTable::findOrAddConstant(enum ConstantType type, std::string utf8string, int intVal, double dVal, int fRef, int sRef)
{
    int constant = findConstant(type, utf8string, fRef, sRef, intVal, dVal);

    if (constant == -1)
    {
        constant = addConstant(type, utf8string, intVal, dVal, fRef, sRef);
    }

    return constant;
}

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

int ConstantTable::findOrAddUTF8(std::string utf8string)
{
    return this->findOrAddConstant(Utf8_C, utf8string);
}

int ConstantTable::findOrAddClassRef(int utf8Ref)
{
    return this->findOrAddConstant(Class_C, "", 0, 0, utf8Ref);
}

int ConstantTable::findOrAddMethodRef(int classRef, int nameAndTypeRef)
{
    return this->findOrAddConstant(MethodRef_C, "", 0, 0, classRef, nameAndTypeRef);
}

int ConstantTable::findOrAddNameAndType(int utf8Ref, int descriptorRef)
{
    return this->findOrAddConstant(NameAndType_C, "", 0, 0, utf8Ref, descriptorRef);
}

int ConstantTable::findOrAddInteger(int intValue)
{
    return this->findOrAddConstant(Integer_C, "", intValue);
}

int ConstantTable::findOrAddDouble(double doubleValue)
{
    return this->findOrAddConstant(Double_C, "", 0, doubleValue);
}

int ConstantTable::findOrAddFieldRef(int classRef, int NameAndTypeRef)
{
    return this->findOrAddConstant(FieldRef_C, "", 0, 0, classRef, NameAndTypeRef);
}

int ConstantTable::findOrAddString(int utf8Ref)
{
    return this->findOrAddConstant(String_C, "", 0, 0, utf8Ref);
}

int ConstantTable::findUTF8(std::string utf8string)
{
    return this->findConstant(Utf8_C, utf8string);
}

int ConstantTable::findClassRef(int utf8Ref)
{
	return this->findConstant(Class_C, "", utf8Ref);
}

int ConstantTable::findMethodRef(int classRef, int nameAndTypeRef)
{
	return this->findConstant(MethodRef_C, "", classRef, nameAndTypeRef);
}

int ConstantTable::findNameAndType(int utf8Ref, int descriptorRef)
{
	return this->findConstant(NameAndType_C, "", utf8Ref, descriptorRef);
}

int ConstantTable::findIntegerRef(int intValue)
{
	return this->findConstant(Integer_C, "", 0, 0, intValue);
}

int ConstantTable::findDoubleRef(double doubleValue)
{
	return this->findConstant(Double_C, "", 0, 0, 0, doubleValue);
}

int ConstantTable::findFieldRef(int classRef, int NameAndTypeRef)
{
	return this->findConstant(FieldRef_C, "", classRef, NameAndTypeRef);
}

int ConstantTable::findStringRef(int utf8Ref)
{
	return this->findConstant(String_C, "", utf8Ref);
}

int ConstantTable::addUTF8(std::string utf8string)
{
	return this->addConstant(Utf8_C, utf8string);
}

int ConstantTable::addClassRef(int utf8Ref)
{
	return this->addConstant(Class_C, "", 0, 0, utf8Ref);
}

int ConstantTable::addMethodRef(int classRef, int nameAndTypeRef)
{
	return this->addConstant(MethodRef_C, "", 0, 0, classRef, nameAndTypeRef);
}

int ConstantTable::addNameAndType(int utf8Ref, int descriptorRef)
{
	return this->addConstant(NameAndType_C, "", 0, 0, utf8Ref, descriptorRef);
}

int ConstantTable::addIntegerRef(int intValue)
{
	return this->addConstant(Integer_C, "", intValue);
}

int ConstantTable::addDoubleRef(double doubleValue)
{
	return this->addConstant(Double_C, "", 0, doubleValue);
}

int ConstantTable::addFieldRef(int classRef, int NameAndTypeRef)
{
	return this->addConstant(FieldRef_C, "", 0, 0, classRef, NameAndTypeRef);
}

int ConstantTable::addStringRef(int utf8Ref)
{
	return this->addConstant(String_C, "", 0, 0, utf8Ref);
}

int ConstantTable::size()
{
    return this->constants.size();
}
