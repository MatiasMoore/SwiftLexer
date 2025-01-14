#pragma once
#include <vector>
#include <string>

class TypeNode;

std::vector<char> intToByteVector(int num, int arraySize);

void appendArrayToByteVector(std::vector<char>* data, char array[], int arraySize);

std::vector<char> generateBytesForConstantTable(class ConstantTable* table);

std::vector<char> generateBytesForConstantTableItem(class ConstantTableItem* elem);

std::string descriptorForType(TypeNode* typeNode);