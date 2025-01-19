#pragma once
#include <string>
#include <map>
#include <vector>
#include "constTable.h"
#include "../nodes/VarDeclarationNode.h"
#include "../nodes/AccessModifierNode.h"

enum FieldAccessFlag {
    F_ACC_PUBLIC = 0x0001,         //	Declared public; may be accessed from outside its package.
    F_ACC_PRIVATE = 0x0002,        //	Declared private; accessible only within the defining class.
    F_ACC_PROTECTED = 0x0004,      //	Declared protected; may be accessed within subclasses.
    F_ACC_STATIC = 0x0008,         //	Declared static.
    F_ACC_FINAL = 0x0010,          //	Declared final; must not be overridden(§5.4.5).
    F_ACC_VOLATILE = 0x0040,       //   Declared volatile; cannot be cached.
    F_ACC_TRANSIENT = 0x0080,      //   Declared transient; not written or read by a persistent object manager.
    F_ACC_SYNTHETIC = 0x1000,      //	Declared synthetic; not present in the source code.
    F_ACC_ENUM = 0x4000            //   Declared as an element of an enum class.
};

class ExternalField
{
public:
    ExternalField(std::string varName, std::string descriptor, std::string className);

    int addFieldRefToConstTable(ConstantTable* constTable);
    int findFieldRef(ConstantTable* constTable);
    std::string getVarName();
    std::string getDescriptor();
    std::string getClassName();

protected:
    std::string _varName;

    std::string _descriptor;

    std::string _className;
};

class InternalField : public ExternalField
{
public:
    InternalField(std::string name, std::string descriptorr, std::string className, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr);
    
    int accessFlagsToInt(std::vector<FieldAccessFlag> flags);

    bool isStatic();

private:
    std::vector<FieldAccessFlag> _flags;

    ExprNode* _constValue;
};
