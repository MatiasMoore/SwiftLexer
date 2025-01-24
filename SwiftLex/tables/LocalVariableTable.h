#pragma once
#include <map>
#include <string>
#include "ConstantTable.h"

/*! \brief Таблица локальных переменных. */
class LocalVariableTable
{
public:
    /// Контейнер элементов.
    std::map<std::string, class LocalVariableElement*> items = {};

    LocalVariableElement* addLocalVar(std::string name, std::string descriptor, bool isConst);

    LocalVariableElement* findLocalVar(std::string name);
};

class LocalVariableElement
{
public:
    std::string name; // Имя локальной переменной.
    std::string _descriptor; // Тип локальной переменной.
    int localId;

    int start_pc = 0;
    int length = 9;
    int nameIndex;
    int descriptorIndex;
    bool isConst;

    LocalVariableElement(int localId, std::string name, std::string descriptor, bool isConst);
};
