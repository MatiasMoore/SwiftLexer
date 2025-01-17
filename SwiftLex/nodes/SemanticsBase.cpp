#include "SemanticsBase.h"

void SemanticsStack::push(SemanticsBase* elem)
{
	this->_vec.push_back(elem);
}
