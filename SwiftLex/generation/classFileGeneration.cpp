#include "classFileGeneration.h"
#include "../tables/tables.h"
#include "generationHelpers.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

char _magicConstant[4] = { 0xCA, 0xFE, 0xBA, 0xBE };
char _smallVersion[2] = { 0x00, 0x00 };
char _bigVersion[2] = { 0x00, 0x3E };

void generateClassFile(ClassTable classTable, std::string className)
{
	// Инициализация.
	std::string fileName = className + ".class";
	std::ofstream out(fileName, std::ios::out | std::ios::binary);

	// Ввод магических констант.
	out.write(_magicConstant, 4);
	out.write(_smallVersion, 2);
	out.write(_bigVersion, 2);
	ClassTableElement* elem = classTable.items[className];
	std::stringstream sstream;
	printf("Constants count :%d\n", elem->constants->constants.size());


	std::vector<char> tableLen = intToByteVector(elem->constants->constants.size() + 1, 2);

	out.write(tableLen.data(), tableLen.size());
	sstream.clear();

	std::vector<char> data = generateBytesForConstantTable(elem->constants);
	data.push_back(0x00);
	data.push_back(0x21);

	std::vector<char> thisCls = intToByteVector(elem->thisClass, 2);
	std::vector<char> parentCls = intToByteVector(elem->superClass, 2);
	appendArrayToByteVector(&data, thisCls.data(), thisCls.size());
	appendArrayToByteVector(&data, parentCls.data(), parentCls.size());
	std::vector<char> interfaceCount = intToByteVector(0, 2);
	std::vector<char> fieldCount = intToByteVector(0, 2);

	// Посчитать количество методов.
	int mCount = 0;
	std::vector<char> codeData;

	/*
	for (auto i = elem->methods->methods.begin(); i != elem->methods->methods.end(); ++i)
	{
		for (auto it = i->second.begin(); it != i->second.end(); ++it)
		{
			mCount++;
			std::vector<char> methodCodeData = generateMethodCode(it->second, elem);
			appendArrayToByteVector(&codeData, methodCodeData.data(), methodCodeData.size());
		}

	}

	if (elem->methods->methods.count("main") != 0)
	{
		if (elem->methods->methods["main"].count("()") != 0)
		{
			mCount++;
			std::vector<char> mainCodeData = generateMain(elem, elem->methods->methods["main"]["()"]);
			appendArrayToByteVector(&codeData, mainCodeData.data(), mainCodeData.size());
		}
	}
	*/

	std::vector<char> methodCount = intToByteVector(mCount, 2);

	std::vector<char> attributeCount = intToByteVector(0, 2);
	appendArrayToByteVector(&data, interfaceCount.data(), interfaceCount.size());
	appendArrayToByteVector(&data, fieldCount.data(), fieldCount.size());
	appendArrayToByteVector(&data, methodCount.data(), methodCount.size());
	appendArrayToByteVector(&data, codeData.data(), codeData.size());
	appendArrayToByteVector(&data, attributeCount.data(), attributeCount.size());

	out.write(data.data(), data.size());
	// Закрытие файла.
	out.close();
}