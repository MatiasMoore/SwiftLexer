#include "classFileGeneration.h"
#include "../tables/tables.h"
#include "generationHelpers.h"
#include "../nodes/StmtNode.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../nodes/ExprNode.h"

char _magicConstant[4] = { 0xCA, 0xFE, 0xBA, 0xBE };
char _smallVersion[2] = { 0x00, 0x00 };
char _bigVersion[2] = { 0x00, 0x3E };
int _stackSize = 1000;

std::vector<char> generateFieldCode(InternalField* fieldElement, InternalClass* classElement)
{
	std::vector<char> res;
	std::vector<char> accessFlags = intToByteVector(fieldElement->accessFlagsToInt(), 2);
	std::vector<char> nameIndex = intToByteVector(fieldElement->_nameRef, 2);
	std::vector<char> descriptorIndex = intToByteVector(fieldElement->_descriptorRef, 2);
	std::vector<char> attributesCount = intToByteVector(0, 2);
	/*
		u2             access_flags;
		u2             name_index;
		u2             descriptor_index;
		u2             attributes_count;
		attribute_info attributes[attributes_count];
	*/
	appendVecToVec(res, accessFlags);
	appendVecToVec(res, nameIndex);
	appendVecToVec(res, descriptorIndex);
	appendVecToVec(res, attributesCount);
	return res;
}

std::vector<char> generateMethodAttribute(InternalMethod* mElem, InternalClass* cElem)
{
	std::vector<char> res;

	int cd = cElem->getConstTable()->findOrAddUTF8("Code");
	std::vector<char> codeAttributeSizeBytes = intToByteVector(cd, 2);
	appendVecToVec(res, codeAttributeSizeBytes);

	std::vector<char> codeBytes = {};

	if (mElem->_body == nullptr)
		throw std::runtime_error("Method \"" + mElem->getMethodName() + "\" must have a body!");

	for (auto& stmt : mElem->_body->_vec)
	{
		//FIXME
		std::vector<char> bytes = stmt->generateCode(cElem, mElem);
		appendVecToVec(codeBytes, bytes);
	}
	printf("Code bytes len: %d\n", codeBytes.size());
	
	std::vector<char> lengthBytes = intToByteVector(12 + codeBytes.size(), 4);
	appendVecToVec(res, lengthBytes);

	//Добавление размера стека операндов
	std::vector<char> stackSizeBytes = intToByteVector(_stackSize, 2);
	appendVecToVec(res, stackSizeBytes);

	//Добавление количества локальных переменных
	int localsSize = mElem->getVarTable()->getTotalVarCount();
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

	int subAttributeCount = 0;
	std::vector<char> attributesCountBytes = intToByteVector(subAttributeCount, 2);
	appendVecToVec(res, attributesCountBytes);

	return res;
}

std::vector<char> generateMethodCode(InternalMethod* mElem, InternalClass* cElem)
{
	std::vector<char> res;

	std::vector<char> accessFlag = intToByteVector(mElem->accessFlagsToInt(), 2);
	appendVecToVec(res, accessFlag);

	//Добавление имени метода
	std::vector<char> nameBytes = intToByteVector(mElem->_nameRef, 2);
	appendVecToVec(res, nameBytes);

	// Добавление дескриптора метода
	std::vector<char> typeBytes = intToByteVector(mElem->_descriptorRef, 2);
	appendVecToVec(res, typeBytes);

	//Добавление атрибутов TODO:Code
	std::vector<char> codeAttributeSizeBytes = intToByteVector(1, 2);
	appendVecToVec(res, codeAttributeSizeBytes);
	std::vector<char> codeAttributeBytes = generateMethodAttribute(mElem, cElem);
	appendVecToVec(res, codeAttributeBytes);

	return res;
}

void generateClassFile(InternalClass* classElem, std::string pathToFolder)
{
	/*
    u4             magic;
    u2             minor_version;
    u2             major_version;
    u2             constant_pool_count;
    cp_info        constant_pool[constant_pool_count-1];
    u2             access_flags;
    u2             this_class;
    u2             super_class;
    u2             interfaces_count;
    u2             interfaces[interfaces_count];
    u2             fields_count;
    field_info     fields[fields_count];
    u2             methods_count;
    method_info    methods[methods_count];
    u2             attributes_count;
    attribute_info attributes[attributes_count];
	*/

	// Инициализация.
	std::string fileName = pathToFolder + classElem->getClassName() + ".class";

	std::cout << "Generating class \"" + classElem->getClassName() + "\" to file \"" + fileName + "\"" << std::endl;

	std::ofstream out(fileName, std::ios::out | std::ios::binary);

	// Ввод магических констант.
	out.write(_magicConstant, 4);
	out.write(_smallVersion, 2);
	out.write(_bigVersion, 2);

	auto constantCount = classElem->getConstTable()->size();

	std::cout << "Constants count: " << constantCount << std::endl;

	std::vector<char> tableLen = intToByteVector(constantCount + 1, 2);

	out.write(tableLen.data(), tableLen.size());

	std::vector<char> data = generateBytesForConstantTable(classElem->getConstTable());

	//TODO add correct class modifiers
	data.push_back(0x00);
	data.push_back(0x21);

	std::vector<char> thisCls = intToByteVector(classElem->getClassRef(), 2);
	std::vector<char> parentCls = intToByteVector(classElem->getBaseClassRef(), 2);
	appendVecToVec(data, thisCls);
	appendVecToVec(data, parentCls);
	std::vector<char> interfaceCount = intToByteVector(0, 2);

	int fieldCount = 0;
	std::vector<char> fieldsCodeData = {};

	for (auto& field : classElem->getInternalFields())
	{
		fieldCount++;
		std::vector<char> fieldCodeData = generateFieldCode(field, classElem);
		appendVecToVec(fieldsCodeData, fieldCodeData);
	}

	std::cout << "Fields count: " << fieldCount << std::endl;

	// Посчитать количество методов.
	int mCount = 0;
	std::vector<char> codeData;

	// Для каждого метода
	for (auto& method : classElem->getInternalMethods())
	{
		mCount++;
		std::vector<char> methodCodeData = generateMethodCode(method, classElem);
		appendVecToVec(codeData, methodCodeData);
	}

	std::cout << "Method count: " << mCount << std::endl;

	std::vector<char> methodCount = intToByteVector(mCount, 2);
	std::vector<char> attributeCount = intToByteVector(0, 2);
	std::vector<char> fieldCountBytes = intToByteVector(fieldCount, 2);

	appendVecToVec(data, interfaceCount);
	appendVecToVec(data, fieldCountBytes);
	appendVecToVec(data, fieldsCodeData);
	appendVecToVec(data, methodCount);
	appendVecToVec(data, codeData);
	appendVecToVec(data, attributeCount);

	out.write(data.data(), data.size());
	// Закрытие файла.
	out.close();
}
