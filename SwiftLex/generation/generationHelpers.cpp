#include "generationHelpers.h"
#include "../tables/tables.h"

std::vector<char> intToByteVector(int num, int arraySize)
{
	std::vector<char> res(arraySize);
	for (int i = 0; i < arraySize; i++)
	{
		res[arraySize - i - 1] = (num >> (i * 8));
	}
	return res;
}

void appendArrayToByteVector(std::vector<char>* data, char array[], int arraySize)
{
	for (int i = 0; i < arraySize; i++)
	{
		data->push_back(array[i]);
	}
}

std::vector<char> generateBytesForConstantTable(class ConstantTable* table)
{
	std::vector <char> res;
	for (auto iter = table->constants.cbegin(); iter != table->constants.cend(); ++iter) {
		std::vector<char> bytes = generateBytesForConstantTableItem(iter->second);
		appendArrayToByteVector(&res, bytes.data(), bytes.size());
	}

	return res;
}

std::vector<char> generateBytesForConstantTableItem(class ConstantTableItem* elem)
{
	std::vector<char> res = intToByteVector(elem->cnst, 1);

	// Добавление типа константы
	switch (elem->cnst)
	{
	case Utf8_C: {
		std::vector<char> len = intToByteVector(elem->string.length(), 2); //Размер строки в баи?тах
		char* str = new char[elem->string.length() + 1]; //Строка
		strcpy(str, elem->string.c_str());
		appendArrayToByteVector(&res, len.data(), len.size());
		appendArrayToByteVector(&res, str, elem->string.length());
		delete[] str;
	}
	break;
	case Integer_C: {
		std::vector<char> num = intToByteVector(elem->Integer, 4); //Число в баи?тах
		appendArrayToByteVector(&res, num.data(), num.size());
	}
	break;
	case String_C:
	case Class_C: {
		std::vector<char> ref = intToByteVector(elem->firstRef, 2); //Номер константы UTF-8
		appendArrayToByteVector(&res, ref.data(), ref.size());
	}
	break;
	case NameAndType_C: {
		std::vector<char> firstRef = intToByteVector(elem->firstRef, 2); //Номер константы UTF-8 с именем
		std::vector<char> secondRef = intToByteVector(elem->secRef, 2); //Номер константы UTF-8 с типом
		appendArrayToByteVector(&res, firstRef.data(), firstRef.size());
		appendArrayToByteVector(&res, secondRef.data(), secondRef.size());
	}
	break;
	case FieldRef_C:
	case MethodRef_C: {
		std::vector<char> firstRef = intToByteVector(elem->firstRef, 2); //Номер константы Class с классом
		std::vector<char> secondRef = intToByteVector(elem->secRef, 2); //Номер константы NameAndType с типом и именем
		appendArrayToByteVector(&res, firstRef.data(), firstRef.size());
		appendArrayToByteVector(&res, secondRef.data(), secondRef.size());
	}
	break;
	default:
		break;
	}

	return res;
}