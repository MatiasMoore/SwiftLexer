#include "dotHelpers.h"
#include <unordered_set>
#include <unordered_map>

struct pair_hash {
	inline std::size_t operator()(const std::pair<int, int>& v) const {
		return v.first * 31 + v.second;
	}
};
std::unordered_set< std::pair<int, int>, pair_hash> _connections = {};
std::unordered_set<int> _labels = {};

std::string dotConnection(int id1, int id2)
{
	if (_connections.count({ id1, id2 }) != 0)
		return "";
	
	_connections.insert({ id1, id2 });
	return std::to_string(id1) + "->" + std::to_string(id2) + '\n';
}

std::string dotConnectionWithLabel(int id1, int id2, std::string text)
{
	if (_connections.count({ id1, id2 }) != 0)
		return "";

	_connections.insert({ id1, id2 });
	return std::to_string(id1) + "->" + std::to_string(id2) + " [label=\"" + text + "\"]" + '\n';;
}

std::string dotLabel(int id, std::string text)
{
	if (_labels.count(id) > 0)
		return "";

	_labels.emplace(id);
	return std::to_string(id) + " [label=\"" + text + "\"]" + '\n';
}
