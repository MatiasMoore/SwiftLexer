#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <set>

class SemanticsBase;

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

	template <typename ElemType>
	static SemanticsBase* semanticsTransformVector(SemanticsStack stack, SemanticsBase* ptrToMe, std::vector<ElemType*>& vecToTransform)
	{
		stack.push(ptrToMe);
		if (ptrToMe->_isAlreadyTransformed)
			return ptrToMe;

		for (int i = 0; i < vecToTransform.size(); i++)
		{
			// We iterate through the vector in this strange way because the element may change the vector
			// we are currently going through, so we make sure to continue onwards with the correct index
			// (e.g. we don't want to go through the same element twice if it added some elems before it to this vector)
			// 
			// ---Before transform of elem 3---
			// 
			//		  | we are at this elem with index 2
			// [1, 2, 3, 4, 5]
			// 
			// ---After transform of elem 3---
			// 
			// it added two new elems before itself using SemanticsStack
			// 
			//		  | we are at this elem with index 2
			// [1, 2, 10, 10, 3, 4, 5]
			// 
			// 
			//		          | but we want to be here, so we change the index to 4
			// [1, 2, 10, 10, 3, 4, 5]

			auto notTransformed = vecToTransform[i];
			auto transformed = vecToTransform[i]->semanticsTransform(stack)->typecast<ElemType>();
			int indexOfNotTransformed = std::find(vecToTransform.cbegin(), vecToTransform.cend(), notTransformed) - vecToTransform.begin();
			i = indexOfNotTransformed;
			vecToTransform[i] = transformed;
		}

		return ptrToMe;
	}
};
