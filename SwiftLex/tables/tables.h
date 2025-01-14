#pragma once
#include <map>
#include <utility>
#include <string>
#include <vector>

class ConstantTable;
class ConstantTableItem;
class MethodTableElement;
class MethodTable;
class TypeNode;
class FuncDeclNode;
class ClassDeclNode;
class ConstructorDeclNode;

/*! \brief Структура, описывающая таблицу классов. */
class ClassTable
{
public:
    /// Укащатель на контейнер элементов таблицы.
    std::map<std::string, class ClassTableElement*> items;

    ClassTableElement* addMainClass();

    ClassTableElement* addClass(ClassDeclNode* classDecl);

};

class ClassTableElement {
public:
    ClassTableElement(std::string name, std::string superName);

    // Ссылка на константу с именем класса - номер константы.
    // UTF-8?
    int name;

    /// Флаг, показывающий открыт ли класс для наследования.
    int isOpen = 0;

    // Ссылка на константу с именем родительского класса - номер константы.
    int superName = NULL;

    // Ссылка на константу текущего класса.
    // Class??
    int thisClass;

    // Ссылка на константу родительского класса.
    // Class??
    int superClass;

    std::string nameStr;

    ConstantTable* constants;

    MethodTable* methods;

    MethodTableElement* addMethod(FuncDeclNode* funcDecl);

    MethodTableElement* addMethodConstructor(ConstructorDeclNode* constructorDecl);

};

/*! Тип константы в таблице констант. */
enum ConstantType
{
    Utf8_C = 1,
    Class_C = 7,
    MethodRef_C = 10,
    NameAndType_C = 12,
    Integer_C = 3,
    Double_C = 6,
    FieldRef_C = 9,
    String_C = 8
};

/*! Элемент таблицы констант. */
class ConstantTableItem
{
public:
    int id; // Номер константы.
    enum ConstantType cnst; // Тип константы.
    std::string string; // Строка для Utf-8 констант.
    int Integer; // Целое число для целочисленной константы.
    double Double; // Число с плавающий точкой.
    int firstRef; // Сслыка на первую константу.
    int secRef;  // Ссылка на вторую константу.

    /*! Конструктор.
    * \param[in] type тип константы.
    * \param[in] id номер константы в таблице констант.
    * \param[in] utf8 строка-utf8.
    * \param[in] intVal целочисленное значение для цеочисленных констант.
    * \param[in] dVal значение числас плавающей точкой для дробных констант.
    * \param[in] fRef ссылка на первую контанту - номер в таблице констант.
    * \param[in] secondRef ссылка на вторую контанту - номер в таблице констант.
    */
    ConstantTableItem(enum ConstantType type, int id, std::string utf8 = NULL, int intVal = NULL, double dVal = NULL, int fRef = NULL, int secondRef = NULL);
};

/*! Таблица констант. */
class ConstantTable
{
public:
    int maxId = 1; // Текущее максимальное значение номера констант в таблице констант.
    std::map<int, class ConstantTableItem*> constants; // Словарь - таблица констант.
    ConstantTable();

    /*! Найти константу в таблице констант или добавить ее в таблицу, если отсутствуюет.
    * \param[in] type Тип константы.
    * \param[in] utf8string строка константы UTF-8.
    * \return номер константы в таблице констант, если таковая имеется; номер добавленной константы, если таковой не было.
    */
    int findOrAddConstant(enum ConstantType type, std::string utf8string = NULL, int intVal = NULL, double dVal = NULL, int fRef = NULL, int sRef = NULL);

private:

    /*! Найти константу в таблице констант.
    * \param[in] type тип константы.
    * \param[in] fRef номер константы первой ссылки.
    * \param[in] secondRef номер константы второй ссылки.
    * \param[in] intVal Переменная для целочисленных значений.
    * \param[in] dVal Переменная для дробных значений с плавающей точкой.
    * \return номер найденной константы; -1, если константа не найдена.
    */
    int findConstant(enum ConstantType type, std::string utf8string, int fRef = NULL, int secondRef = NULL, int intVal = NULL, double dVal = NULL);

};

/*! \brief Таблица методов класса. */
class MethodTable
{
public:
    std::map<std::string, class MethodTableElement*> methods;
};

/*! \brief Элемент таблицы методов класса. */
class MethodTableElement
{
public:
    /// Ссылка на номер константы с именем метода в таблице констант.
    int methodName;

    /// Ссылка на номер константы с дескриптором в таблице констант.
    int descriptor;

    /// Строковое название метода.
    std::string strName;

    /// Строковый дескриптор метода.
    std::string strDesc;

    // Объявление метода
    class StmtListNode* _body;

    /// Ссылка на таблицу локальных переменных.
    class LocalVariableTable* varTable = NULL;

    /// Флаг показывающий, является ли указанный метод первым кандидатом.
    int isFirst = 0;

    MethodTableElement(ConstantTable* constants, FuncDeclNode* funcDecl);

    MethodTableElement(ConstantTable* constants, ConstructorDeclNode* constructorDecl);
};

class LocalVariableElement
{
public:
    std::string name; // Имя локальной переменной.
    int nameNum; // Идентификатор локальной переменной.
    TypeNode* _type; // Тип локальной переменной.
    int isConst; // Флаг, показывающий, является ли переменная изменяемой.
    int isInit; // Флаг, показывающий, инициализрована переменная.

    LocalVariableElement(ConstantTable* constants, std::string name, TypeNode* type, int isConst = 0, int isInitial = 0);
};

/*! \brief Таблица локальных переменных. */
class LocalVariableTable
{
public:

    /// Текущее максимальное значение идентификатора переменной.
    int maxId = 0;

    /// Контейнер элементов.
    std::map<std::string, class LocalVariableElement*> items = {};

    int findOrAddLocalVar(ConstantTable* constants, std::string name, TypeNode* type, int isConst = 0, int isInitial = 0);
};