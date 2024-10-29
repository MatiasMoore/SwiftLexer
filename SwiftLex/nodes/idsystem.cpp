#include "idsystem.h"
int _newId = 0;

int getNewId()
{
	auto id = _newId;
	_newId++;
	return id;
}
