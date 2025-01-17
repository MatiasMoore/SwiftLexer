#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <typeinfo>

class SemanticsStack;

class SemanticsBase
{
protected:
	virtual SemanticsBase* semanticsTransform(SemanticsStack stack) = 0;

public:
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
	SemChild* getClosest()
	{
		for (auto i = _vec.rbegin(); i != _vec.rend(); i++)
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