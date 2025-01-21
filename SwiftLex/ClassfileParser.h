#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "tables/tables.h"

namespace JavaClassFileParser
{
	uint16_t readUint16(std::istream& file);

	uint32_t readUint32(std::ifstream& file);

	std::string readUTF8(std::ifstream& file, uint16_t length);

	std::vector<MethodAccessFlag> getMethodAccessFlagsFromInt(int flagsInt);

	std::vector<FieldAccessFlag> getFieldAccessFlagsFromInt(int flagsInt);

	void addFromFileToClassTable(std::string filename, ClassTable* classTable);
}

