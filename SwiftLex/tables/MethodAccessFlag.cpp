#include "MethodAccessFlag.h"

int methodAccessFlagsToInt(std::vector<MethodAccessFlag> flags)
{
    int accessFlag = 0;
    for (auto& flag : flags)
    {
        // Skip custom flags
        if (flag != M_ACC_CUSTOM_OVERRIDE)
            accessFlag += (int)flag;
    }
    return accessFlag;
}
