#pragma once
#include <map>
#include <string>

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
