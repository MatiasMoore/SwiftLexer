#pragma once
#include <vector>

std::vector<char> intToByteVector(int num, int arraySize);

void appendArrayToByteVector(std::vector<char>* data, char array[], int arraySize);

std::vector<char> generateBytesForConstantTable(class ConstantTable* table);

std::vector<char> generateBytesForConstantTableItem(class ConstantTableItem* elem);