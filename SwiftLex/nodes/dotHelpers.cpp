#include "dotHelpers.h"

std::string dotConnection(int id1, int id2)
{
	return std::to_string(id1) + "->" + std::to_string(id2) + '\n';
}

std::string dotLabel(int id, std::string text)
{
	return std::to_string(id) + " [label=\"" + text + "\"]" + '\n';
}
