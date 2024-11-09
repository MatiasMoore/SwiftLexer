#pragma once
#include <vector>
#include "dottable.h"

template <typename ListElem, typename List>
class GenericListNode : public Dottable {
public:
	std::vector<ListElem*> _vec = {};

	virtual std::string getName() = 0;

	static List* createListNode(ListElem* node)
	{
		auto list = new List();
		list->_vec.push_back(node);
		return (List*)list;
	}

	List* appendNode(ListElem* node)
	{
		_vec.push_back(node);
		return (List*)this;
	}

	List* appendNodeList(GenericListNode* list)
	{
		for (auto& node : list->_vec)
			_vec.push_back(node);
		return (List*)this;
	}

	void generateDot(std::ofstream& file) override
	{
		file << dotLabel(this->_id, this->getName());
		for (auto& child : this->_vec)
		{
			file << dotConnection(this->_id, child->_id);
			child->generateDot(file);
		}
	}
};

