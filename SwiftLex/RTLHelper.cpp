#include "RTLHelper.h"

int lastInternalVarId = 0;

std::string RTLHelper::getUniqueInternalVarName()
{
    lastInternalVarId++;
    return "<" + std::to_string(lastInternalVarId) + ">";
}
