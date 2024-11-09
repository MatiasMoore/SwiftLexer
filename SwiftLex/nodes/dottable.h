#pragma once
#include <iostream>
#include "idsystem.h"
#include "dotHelpers.h"

class Dottable
{
public:
	int _id = getNewId();
	virtual void generateDot(std::ofstream& file) = 0;
};

