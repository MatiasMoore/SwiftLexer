#pragma once
#include <fstream>
#include <string>

std::string dotConnection(int id1, int id2);

std::string dotConnectionWithLabel(int id1, int id2, std::string text);

std::string dotLabel(int id, std::string text);

