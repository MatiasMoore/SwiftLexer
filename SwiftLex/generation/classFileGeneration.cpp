#include "classFileGeneration.h"
#include "../tables/tables.h"
#include "generationHelpers.h"
#include "../nodes/StmtNode.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

char _magicConstant[4] = { 0xCA, 0xFE, 0xBA, 0xBE };
char _smallVersion[2] = { 0x00, 0x00 };
char _bigVersion[2] = { 0x00, 0x3E };
int _stackSize = 1000;

std::vector<char> generateMethodAttribute(class MethodTableElement* mElem, ClassTableElement* cElem)
{
	std::vector<char> res;

	int cd = cElem->constants->findOrAddConstant(Utf8_C, "Code");
	std::vector<char> codeAttributeSizeBytes = intToByteVector(cd, 2);
	appendVecToVec(res, codeAttributeSizeBytes);

	std::vector<char> codeBytes = {};

	//TODO WARNING REMOVE LATER THIS IS JUST A PLACEHOLDER
	//codeBytes.push_back(0xB1); //just return

	//TODO generate code from body
	
	if (mElem->_body == nullptr)
		throw std::runtime_error("Method \"" + mElem->strName + "\" must have a body!");

	for (auto& stmt : mElem->_body->_vec)
	{
		std::vector<char> bytes = stmt->generateCodeForStmt();// generateCodeForStatement(curStatement, cElem, mElem);
		appendVecToVec(codeBytes, bytes);
	}
	printf("Code bytes len: %d\n", codeBytes.size());
	

	//Добавление длины атрибута
	std::vector<char> lengthBytes = intToByteVector(12 + codeBytes.size(), 4);
	appendVecToVec(res, lengthBytes);

	//Добавление размера стека операндов
	std::vector<char> stackSizeBytes = intToByteVector(_stackSize, 2);
	appendVecToVec(res, stackSizeBytes);

	//Добавление количества локальных переменных
	int localsSize = mElem->varTable->items.size();
	std::vector<char> localsSizeBytes = intToByteVector(localsSize, 2);
	appendVecToVec(res, localsSizeBytes);

	//Добавление длины байт-кода TODO: сделать
	std::vector<char> codeSizeBytes = intToByteVector(codeBytes.size(), 4);
	appendVecToVec(res, codeSizeBytes);

	//Добавление байт-кода
	appendVecToVec(res, codeBytes);

	//Добавление количества записей в таблице исключений
	std::vector<char> exceptionTableSizeBytes = intToByteVector(0, 2);
	appendVecToVec(res, exceptionTableSizeBytes);

	//Добавление количества атрибутов
	std::vector<char> attributesCountBytes = intToByteVector(0, 2);
	appendVecToVec(res, attributesCountBytes);

	return res;
}

std::vector<char> generateMethodCode(class MethodTableElement* mElem, class ClassTableElement* cElem)
{
	std::vector<char> res;

	std::vector<char> publicStaticFlag = { 0x00, 0x09 }; //ACC_PUBLIC + ACC_STATIC
	appendVecToVec(res, publicStaticFlag);

	//Добавление имени метода
	std::vector<char> nameBytes = intToByteVector(mElem->methodName, 2);
	appendVecToVec(res, nameBytes);

	// Добавление дескриптора метода
	std::vector<char> typeBytes = intToByteVector(mElem->descriptor, 2);
	appendVecToVec(res, typeBytes);

	//Добавление атрибутов TODO:Code
	std::vector<char> codeAttributeSizeBytes = intToByteVector(1, 2);
	appendVecToVec(res, codeAttributeSizeBytes);
	std::vector<char> codeAttributeBytes = generateMethodAttribute(mElem, cElem);
	appendVecToVec(res, codeAttributeBytes);

	return res;
}

void generateClassFile(ClassTableElement* classElem, std::string pathToFolder)
{
	// Инициализация.
	std::string fileName = pathToFolder + classElem->nameStr + ".class";

	std::cout << "Generating class \"" + classElem->nameStr + "\" to file \"" + fileName + "\"" << std::endl;

	std::ofstream out(fileName, std::ios::out | std::ios::binary);

	// Ввод магических констант.
	out.write(_magicConstant, 4);
	out.write(_smallVersion, 2);
	out.write(_bigVersion, 2);

	auto constantCount = classElem->constants->constants.size();

	std::cout << "Constants count: " << constantCount << std::endl;

	std::vector<char> tableLen = intToByteVector(constantCount + 1, 2);

	out.write(tableLen.data(), tableLen.size());

	std::vector<char> data = generateBytesForConstantTable(classElem->constants);
	data.push_back(0x00);
	data.push_back(0x21);

	std::vector<char> thisCls = intToByteVector(classElem->thisClass, 2);
	std::vector<char> parentCls = intToByteVector(classElem->superClass, 2);
	appendVecToVec(data, thisCls);
	appendVecToVec(data, parentCls);
	std::vector<char> interfaceCount = intToByteVector(0, 2);
	std::vector<char> fieldCount = intToByteVector(0, 2);

	// Посчитать количество методов.
	int mCount = 0;
	std::vector<char> codeData;

	// Для каждого метода
	for (auto i = classElem->methods->methods.begin(); i != classElem->methods->methods.end(); ++i)
	{
		auto method = i->second;
		mCount++;
		std::vector<char> methodCodeData = generateMethodCode(method, classElem);
		appendVecToVec(codeData, methodCodeData);
	}
	/*
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

	std::cout << "Method count: " << mCount << std::endl;

	std::vector<char> methodCount = intToByteVector(mCount, 2);

	std::vector<char> attributeCount = intToByteVector(0, 2);
	appendVecToVec(data, interfaceCount);
	appendVecToVec(data, fieldCount);
	appendVecToVec(data, methodCount);
	appendVecToVec(data, codeData);
	appendVecToVec(data, attributeCount);

	out.write(data.data(), data.size());
	// Закрытие файла.
	out.close();
}
