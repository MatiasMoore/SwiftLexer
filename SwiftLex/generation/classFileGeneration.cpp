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

std::vector<char> generateFieldConstantAttribute(class FieldElement* fieldElement, ClassTableElement* classElement)
{
	std::vector<char> res;

	int attributeNameIndex = classElement->constants->findOrAddConstant(Utf8_C, "ConstantValue");

	/*
		u2 attribute_name_index;
		u4 attribute_length;
		u2 constantvalue_index;
	*/
	appendVecToVec(res, intToByteVector(attributeNameIndex, 2));
	appendVecToVec(res, intToByteVector(2, 4)); //The value of the attribute_length item must be two.
	appendVecToVec(res, intToByteVector(fieldElement->constantValueIndex, 2));
	return res;
}

std::vector<char> generateFieldCode(class FieldElement* fieldElement, class ClassTableElement* classElement)
{


	std::vector<char> res;

	std::vector<char> accessFlags = intToByteVector(fieldElement->accessFlag, 2);
	

	std::vector<char> nameIndex = intToByteVector(fieldElement->nameIndex, 2);
	std::vector<char> descriptorIndex = intToByteVector(fieldElement->descriptorIndex, 2);


	std::vector<char> attributesCount = intToByteVector(0, 2);
	std::vector<char> attributesBytes = {};

	if (fieldElement->isStatic)
	{
		attributesCount = intToByteVector(1, 2);
		attributesBytes = generateFieldConstantAttribute(fieldElement, classElement);
	}

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
	// Only for constants
	appendVecToVec(res, attributesCount);
	appendVecToVec(res, attributesBytes);
	return res;
}

std::vector<char> generateLocalVariableTable(MethodTableElement* mElem, ClassTableElement* cElem) {
	std::vector<char> tableBytes;

	// Add constant pool index for "LocalVariableTable"
	int localVarTableIndex = cElem->constants->findOrAddConstant(Utf8_C, "LocalVariableTable");
	appendVecToVec(tableBytes, intToByteVector(localVarTableIndex, 2));

	// Calculate the local variable table entries
	std::vector<char> localVariableEntries;
	for (const auto& var : mElem->varTable->items) {
		// Start PC, Length, Name Index, Descriptor Index, Index
		auto localVar = var.second;
		// Assuming variables have start_pc and length initialized properly
		appendVecToVec(localVariableEntries, intToByteVector(localVar->start_pc, 2));
		appendVecToVec(localVariableEntries, intToByteVector(localVar->length, 2));
		appendVecToVec(localVariableEntries, intToByteVector(localVar->nameIndex, 2));
		appendVecToVec(localVariableEntries, intToByteVector(localVar->descriptorIndex, 2));
		appendVecToVec(localVariableEntries, intToByteVector(localVar->localId, 2));
	}
	appendVecToVec(tableBytes, intToByteVector(localVariableEntries.size() + 2, 4));
	// Add the local variable table length
	appendVecToVec(tableBytes, intToByteVector(localVariableEntries.size() / 10, 2));
	appendVecToVec(tableBytes, localVariableEntries);

	return tableBytes;
}

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
		std::vector<char> bytes = stmt->generateCode(cElem, mElem);// generateCodeForStatement(curStatement, cElem, mElem);
		appendVecToVec(codeBytes, bytes);
	}
	printf("Code bytes len: %d\n", codeBytes.size());
	
	// Вычисление локальной таблицы переменных
	std::vector<char> localVariableTable = generateLocalVariableTable(mElem, cElem);

	//Добавление длины атрибута
	// removed local var table 
	// std::vector<char> lengthBytes = intToByteVector(12 + localVariableTable.size() + codeBytes.size(), 4);

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

	// Add attributes count
	// removed local var table 
	//int subAttributeCount = 1; // For LocalVariableTable

	int subAttributeCount = 0;
	std::vector<char> attributesCountBytes = intToByteVector(subAttributeCount, 2);
	appendVecToVec(res, attributesCountBytes);

	// Add LocalVariableTable
	// removed local var table 
	//appendVecToVec(res, localVariableTable);

	return res;
}

std::vector<char> generateMethodCode(class MethodTableElement* mElem, class ClassTableElement* cElem)
{
	std::vector<char> res;

	std::vector<char> accessFlag = intToByteVector(mElem->accessFlag, 2);
	appendVecToVec(res, accessFlag);

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

	//TODO add correct class modifiers
	data.push_back(0x00);
	data.push_back(0x21);

	std::vector<char> thisCls = intToByteVector(classElem->thisClass, 2);
	std::vector<char> parentCls = intToByteVector(classElem->superClass, 2);
	appendVecToVec(data, thisCls);
	appendVecToVec(data, parentCls);
	std::vector<char> interfaceCount = intToByteVector(0, 2);

	int fieldCount = 0;
	std::vector<char> fieldsCodeData = {};

	for (auto fieldItem = classElem->fields->items.begin(); fieldItem != classElem->fields->items.end(); ++fieldItem)
	{
		auto field = fieldItem->second;
		fieldCount++;
		std::vector<char> fieldCodeData = generateFieldCode(field, classElem);
		appendVecToVec(fieldsCodeData, fieldCodeData);
	}

	std::cout << "Fields count: " << fieldCount << std::endl;

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
