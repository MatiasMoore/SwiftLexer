#pragma once

enum FieldAccessFlag {
    F_ACC_PUBLIC = MethodAccessFlag::M_ACC_PUBLIC,         //	Declared public; may be accessed from outside its package.
    F_ACC_PRIVATE = MethodAccessFlag::M_ACC_PRIVATE,       //	Declared private; accessible only within the defining class.
    F_ACC_PROTECTED = MethodAccessFlag::M_ACC_PROTECTED,   //	Declared protected; may be accessed within subclasses.
    F_ACC_STATIC = MethodAccessFlag::M_ACC_STATIC,         //	Declared static.
    F_ACC_FINAL = MethodAccessFlag::M_ACC_FINAL,           //	Declared final; must not be overridden(§5.4.5).
    F_ACC_VOLATILE = 0x0040,                               //   Declared volatile; cannot be cached.
    F_ACC_TRANSIENT = 0x0080,                              //   Declared transient; not written or read by a persistent object manager.
    F_ACC_SYNTHETIC = MethodAccessFlag::M_ACC_SYNTHETIC,   //	Declared synthetic; not present in the source code.
    F_ACC_ENUM = 0x4000                                    //   Declared as an element of an enum class.
};

