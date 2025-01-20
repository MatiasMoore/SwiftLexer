#include "generationHelpers.h"
#include "../tables/tables.h"
#include "../nodes/TypeNode.h"
#include "../ExceptionHelper.h"

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

void appendVecToVec(std::vector<char>& vecA, const std::vector<char>& vecB)
{
	for (auto& byte : vecB)
	{
		vecA.push_back(byte);
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

std::string classnameFromDescriptor(std::string descriptor)
{
	if (descriptor.size() <= 3)
		throw std::runtime_error("Can't get a classname from primitive type descriptor!");

	auto temp = std::string(descriptor.c_str() + 1);
	temp.pop_back();
	return temp;
}

std::string getArgDescFromFullDesc(std::string descriptor)
{
	std::string argDesc;

	size_t openBracket = descriptor.find('(');
	size_t closeBracket = descriptor.find(')', openBracket);

	if (openBracket != std::string::npos && closeBracket != std::string::npos) {
		argDesc = descriptor.substr(openBracket, closeBracket - openBracket + 1);
	}
	else {
		throw std::runtime_error("Can't get arg descriptor from descriptor \"" + descriptor + "\"!" + LINE_AND_FILE);
	}

	return argDesc;
}

std::vector<char> jvm::iconstBipushSipush(int num)
{
	std::vector<char> res;

	// iconst
	if (num == -1) {
		res.push_back(0x2); //iconst_m1
	}
	else if (num == 0) {
		res.push_back(0x3); //iconst_0
	}
	else if (num == 1) {
		res.push_back(0x4); //iconst_1
	}
	else if (num == 2) {
		res.push_back(0x5); //iconst_2
	}
	else if (num == 3) {
		res.push_back(0x6); //iconst_3
	}
	else if (num == 4) {
		res.push_back(0x7); //iconst_4
	}
	else if (num == 5) {
		res.push_back(0x8); //iconst_5
	}

	if (num <= 5 && num >= -1) {
		return res;
	}

	if (num >= -128 && num <= 127) {
		// bipush
		res.push_back(0x10); //bipush
		res.push_back(num);
		return res;
	}

	if (num >= -32768 && num <= 32767) {
		// sipush
		res.push_back(0x11); //sipush
		std::vector <char> temp = intToByteVector(num, 2);
		appendArrayToByteVector(&res, temp.data(), temp.size());
		return res;
	}
	else {
		throw std::runtime_error("Error in iconstBipushSipush: Invalid number");
	}
}


std::vector<char> jvm::iload(int num)
{
	std::vector<char> res;
	if (num >= 0 && num <= 255) { // Использовать iload, если значение помещается в 1 байт.
		res.push_back(0x15); //iload
		res.push_back(num);
	}
	else if (num >= 256 && num <= 65535) { // Иначе испльзовать двухбайтовую команду.
		// wide iload
		res.push_back(0xC4); //wide
		res.push_back(0x15); //iload
		std::vector <char> temp = intToByteVector(num, 2);
		appendArrayToByteVector(&res, temp.data(), temp.size());
	}
	else { // Иначе сообщить об ошибке.
		throw std::runtime_error("Error in iload: Invalid number");
	}

	return res;
}

std::vector<char> jvm::aload(int num)
{
	std::vector<char> res;
	if (num >= 0 && num <= 255) { // Использовать aload, если значение помещается в 1 байт.
		res.push_back(0x19); //aload
		res.push_back(num);
	}
	else if (num >= 256 && num <= 65535) { // Иначе испльзовать двухбайтовую команду.
		// wide aload
		res.push_back(0xC4); //wide
		res.push_back(0x19); //aload
		std::vector <char> temp = intToByteVector(num, 2);
		appendArrayToByteVector(&res, temp.data(), temp.size());
	}
	else { // Иначе сообщить об ошибке.
		throw std::runtime_error("Error in aload: Invalid number");
	}

	return res;
}

std::vector<char> jvm::istore(int num)
{
	std::vector<char> res;
	if (num >= 0 && num <= 255) { // Использовать istore, если значение помещается в 1 байт.
		res.push_back(0x36); //istore
		res.push_back(num);
	}
	else if (num >= 256 && num <= 65535) { // Иначе испльзовать двухбайтовую команду.
		// wide istore
		res.push_back(0xC4); //wide
		res.push_back(0x36); //istore
		std::vector <char> temp = intToByteVector(num, 2);
		appendArrayToByteVector(&res, temp.data(), temp.size());
	}
	else { // Иначе сообщить об ошибке.
		throw std::runtime_error("Error in istore: Invalid number");
	}

	return res;
}

std::vector<char> jvm::astore(int num)
{
	std::vector<char> res;
	if (num >= 0 && num <= 255) { // Использовать astore, если значение помещается в 1 байт.
		res.push_back(0x3A); //astore
		res.push_back(num);
	}
	else if (num >= 256 && num <= 65535) { // Иначе испльзовать двухбайтовую команду.
		// wide astore
		res.push_back(0xC4); //wide
		res.push_back(0x3A); //astore
		std::vector <char> temp = intToByteVector(num, 2);
		appendArrayToByteVector(&res, temp.data(), temp.size());
	}
	else { // Иначе сообщить об ошибке.
		throw std::runtime_error("Error in astore: Invalid number");
	}

	return res;
}

std::vector<char> jvm::ldc(int num)
{
	std::vector<char> res;
	if (num >= 0 && num <= 255) { // Использовать ldc, если значение помещается в 1 байт.
		//ldc
		res.push_back(0x12); //ldc
		res.push_back(num);
	}
	else if (num >= 256 && num <= 65535) { // Иначе испльзовать двухбайтовую команду.
		//ldc_w
		res.push_back(0x13); //ldc_w
		std::vector <char> temp = intToByteVector(num, 2);
		appendArrayToByteVector(&res, temp.data(), temp.size());
	}
	else { // Иначе сообщить об ошибке.
		throw std::runtime_error("Error in ldc, ldc_w: Invalid number");
	}

	return res;
}

std::vector<char> jvm::pop()
{
	std::vector <char> res;
	res.push_back(0x57); //pop
	return res;
}

std::vector<char> jvm::dup()
{
	std::vector <char> res;
	res.push_back(0x59); //dup
	return res;
}

std::vector<char> jvm::dup2()
{
	std::vector <char> res;
	res.push_back(0x5C); //dup2
	return res;
}

std::vector<char> jvm::go_to(int offset)
{
	std::vector<char> res;
	res.push_back(0xA7); //goto
	std::vector <char> temp;
	if (offset < 0) // Если смещение отрицательно...
		temp = intToByteVector(offset, 2);
	else
		temp = intToByteVector(offset + 3, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());
	return res;
}

std::vector<char> jvm::if_icmp(IfCommandType type, int offset)
{
	std::vector<char> res;
	switch (type)
	{
	case EQ_if:
		res.push_back(0x9F); //if_icmpeq
		break;
	case NE_if:
		res.push_back(0xA0); //if_icmpne
		break;
	case LT_if:
		res.push_back(0xA1); //if_icmplt
		break;
	case LE_if:
		res.push_back(0xA4); //if_icmple
		break;
	case GT_if:
		res.push_back(0xA3); //if_icmpgt
		break;
	case GE_if:
		res.push_back(0xA2); //if_icmpge
		break;
	default:
		break;
	}

	std::vector <char> temp;
	if (offset < 0)
		temp = intToByteVector(offset, 2);
	else
		temp = intToByteVector(offset + 3, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());

	return res;
}

// ---------- if ----------
std::vector<char> jvm::if_(IfCommandType type, int offset)
{
	std::vector<char> res;

	switch (type)
	{
	case EQ_if:
		res.push_back(0x99); //ifeq
		break;
	case NE_if:
		res.push_back(0x9A); //ifne
		break;
	case LT_if:
		res.push_back(0x9B); //iflt
		break;
	case LE_if:
		res.push_back(0x9E); //ifle
		break;
	case GT_if:
		res.push_back(0x9D); //ifgt
		break;
	case GE_if:
		res.push_back(0x9C); //ifge
		break;
	default:
		break;
	}

	std::vector <char> temp;
	if (offset < 0)
		temp = intToByteVector(offset, 2);
	else
		temp = intToByteVector(offset + 3, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());

	return res;
}

// ---------- if_acmp ----------
std::vector<char> jvm::if_acmp(IfCommandType type, int offset)
{
	std::vector<char> res;

	switch (type)
	{
	case EQ_if:
		res.push_back(0xA5); //if_acmpeq
		break;
	case NE_if:
		res.push_back(0xA6); //if_acmpne
		break;
	default:
	{
		throw std::runtime_error("Error in if_acmp: Invalid type");
	}
	break;
	}
	std::vector <char> temp;
	if (offset < 0)
		temp = intToByteVector(offset, 2);
	else
		temp = intToByteVector(offset + 3, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());

	return res;
}

std::vector<char> jvm::_new(int constant)
{
	std::vector<char> res;
	res.push_back(0xBB); //new
	std::vector <char> temp = intToByteVector(constant, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());
	return res;
}

std::vector<char> jvm::invokespecial(int constant)
{
	std::vector<char> res;
	res.push_back(0xB7); //invokespecial
	std::vector <char> temp = intToByteVector(constant, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());
	return res;
}

std::vector<char> jvm::invokevirtual(int constant)
{
	std::vector <char> res;
	res.push_back(0xB6); //invokevirtual
	std::vector <char> temp = intToByteVector(constant, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());
	return res;
}

std::vector<char> jvm::invokestatic(int constant)
{
	std::vector<char> res;
	res.push_back(0xB8); //invokestatic
	std::vector <char> temp = intToByteVector(constant, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());
	return res;
}

std::vector<char> jvm::ireturn()
{
	std::vector<char> res;
	res.push_back(0xAC); //ireturn
	return res;
}

std::vector<char> jvm::areturn()
{
	std::vector<char> res;
	res.push_back(0xB0); //areturn
	return res;
}

std::vector<char> jvm::_return()
{
	std::vector<char> res;
	res.push_back(0xB1); //return
	return res;
}

std::vector<char> jvm::getfield(int constant)
{
	std::vector <char> res;
	res.push_back(0xB4); //getfield
	std::vector <char> temp = intToByteVector(constant, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());
	return res;
}

std::vector<char> jvm::putfield(int constant)
{
	std::vector<char> res;
	res.push_back(0xB5); //putfield
	std::vector <char> temp = intToByteVector(constant, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());
	return res;
}

std::vector<char> jvm::anewarray(int constant)
{
	std::vector<char> res;
	res.push_back(0xBD); //anewarray
	std::vector <char> temp = intToByteVector(constant, 2);
	appendArrayToByteVector(&res, temp.data(), temp.size());
	return res;
}

std::vector<char> jvm::arraylength()
{
	std::vector<char> res;
	res.push_back(0xBE); //arraylength
	return res;
}

// ---------- aastore ----------
std::vector<char> jvm::aastore()
{
	std::vector<char> res;
	res.push_back(0x53); //aastore
	return res;
}

// ---------- iadd ----------
std::vector<char> jvm::iadd()
{
	std::vector<char> res;
	res.push_back(0x60); //iadd
	return res;
}

std::vector<char> jvm::getstatic(int fieldRef)
{
	std::vector<char> res;
	res.push_back(0xb2); 
	appendVecToVec(res, intToByteVector(fieldRef, 2));
	return res;
}

std::vector<char> jvm::putStatic(int fieldRef)
{
	std::vector<char> res;
	res.push_back(0xb3);
	appendVecToVec(res, intToByteVector(fieldRef, 2));
	return res;
}

// ---------- aaload ----------
std::vector<char> jvm::aaload()
{
	std::vector<char> res;
	res.push_back(0x32); //aaload
	return res;
}

// ---------- iconst_0 ----------
std::vector<char> jvm::iconst_0()
{
	std::vector<char> res;
	res.push_back(0x3); //aaload
	return res;
}

// ---------- iconst_1 ----------
std::vector<char> jvm::iconst_1()
{
	std::vector<char> res;
	res.push_back(0x4); //aaload
	return res;
}

// ---------- iconst_m1 ----------
std::vector<char> jvm::iconst_m1()
{
	std::vector<char> res;
	res.push_back(0x2); //aaload
	return res;
}