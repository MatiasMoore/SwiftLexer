#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <set>

class SemanticsStack;

class SemanticsBase
{
protected:
	bool _isAlreadyTransformed = false;

	virtual SemanticsBase* semanticsTransform(SemanticsStack stack) = 0;

public:
	void setIsAlreadyTransformed(bool flag);

	template <typename SemChild>
	SemChild* typecast()
	{
		auto semBase = this;
		auto semChild = dynamic_cast<SemChild*>(semBase);
		if (semChild == nullptr)
			throw std::runtime_error("Unable to convert " + std::string(typeid(SemanticsBase).name()) + " to " + std::string(typeid(SemChild).name()) + " during semantics transform!");
		return semChild;
	}
};

class SemanticsStack
{
public:
	void push(SemanticsBase* elem);

	template <typename SemChild>
	SemChild* getClosest(std::set<SemanticsBase*> ignore = {})
	{
		for (auto i = _vec.rbegin(); i != _vec.rend(); i++)
		{
			auto elem = *i;

			bool shouldIgnore = ignore.count(elem) != 0;
			if (shouldIgnore)
				continue;

			auto semChild = dynamic_cast<SemChild*>(elem);
			if (semChild != nullptr)
				return semChild;
		}
		return nullptr;
	}

	template <typename SemChild>
	SemChild* getFurthest()
	{
		for (auto i = _vec.begin(); i != _vec.end(); i++)
		{
			auto elem = *i;
			auto semChild = dynamic_cast<SemChild*>(elem);
			if (semChild != nullptr)
				return semChild;
		}
		return nullptr;
	}

private:
	std::vector<SemanticsBase*> _vec = {};
};