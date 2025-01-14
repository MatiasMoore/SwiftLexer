#include "tables.h"

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