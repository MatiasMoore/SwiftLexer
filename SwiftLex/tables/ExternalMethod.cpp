#include "ExternalMethod.h"
#include "../ExceptionHelper.h"

ExternalMethod::ExternalMethod(std::string methodName, std::string descriptor, std::string className, std::vector<MethodAccessFlag> flags)
{
	this->_methodName = methodName;
	this->_descriptor = descriptor;
	this->_className = className;
	this->_flags = flags;
}

int ExternalMethod::addMethodRefToConstTable(ConstantTable* constTable)
{
	if (this->findMethodRef(constTable) != -1)
		throw std::runtime_error("Equal method with name " + this->_methodName + " and descriptor " + this->_descriptor + " already exists in this class!" + LINE_AND_FILE);

	int varNameRef = constTable->findOrAddUTF8(this->_methodName);
	int descriptorRef = constTable->findOrAddUTF8(this->_descriptor);
	int nameAndTypeRef = constTable->findOrAddNameAndType(varNameRef, descriptorRef);

	int classNameRef = constTable->findOrAddUTF8(this->_className);
	int classRef = constTable->findOrAddClassRef(classNameRef);

	int methodRef = constTable->findOrAddMethodRef(classRef, nameAndTypeRef);

	return methodRef;
}

int ExternalMethod::findMethodRef(ConstantTable* constTable)
{
	int nameRef = constTable->findUTF8(this->_methodName);
	int descriptorRef = constTable->findUTF8(this->_descriptor);
	int classNameRef = constTable->findUTF8(this->_className);
	if (nameRef == -1 || descriptorRef == -1 || classNameRef == -1)
		return -1;

	int nameAndTypeRef = constTable->findNameAndType(nameRef, descriptorRef);
	int classRef = constTable->findClassRef(classNameRef);

	if (nameAndTypeRef == -1 || classRef == -1)
		return -1;

	return constTable->findMethodRef(classRef, nameAndTypeRef);
}

int ExternalMethod::findOrAddMethodRef(ConstantTable* constTable)
{
	if (findMethodRef(constTable) == -1)
		return addMethodRefToConstTable(constTable);
	else
		return findMethodRef(constTable);
}

std::string ExternalMethod::getMethodName()
{
	return _methodName;
}

std::string ExternalMethod::getDescriptor()
{
	return _descriptor;
}

std::string ExternalMethod::getArgsDescriptor()
{
	return _descriptor.substr(_descriptor.find('('), _descriptor.find(')') - _descriptor.find('(') + 1);
}

std::string ExternalMethod::getReturnDescriptor()
{
	return _descriptor.substr(_descriptor.find(')') + 1);
}

std::string ExternalMethod::getClassName()
{
	return _className;
}

std::vector<MethodAccessFlag> ExternalMethod::getFlags()
{
	return _flags;
}

int ExternalMethod::accessFlagsToInt()
{
    return methodAccessFlagsToInt(this->_flags);
}

bool ExternalMethod::containsFlag(MethodAccessFlag flag)
{
	return std::find(_flags.begin(), _flags.end(), flag) != _flags.end();
}

std::vector<std::string> ExternalMethod::getArgDescriptorsVector()
{
    std::vector<std::string> descriptors;

    auto argDesc = this->getArgsDescriptor();

    if (argDesc.empty() || argDesc[0] != '(') {
        throw std::runtime_error("Critical error!" + LINE_AND_FILE);
    }

    // Remove the parentheses
    argDesc = argDesc.substr(1, argDesc.size() - 2);

    size_t i = 0;
    while (i < argDesc.size()) {
        if (argDesc[i] == '[') {
            // Array type
            std::string descriptor = "[";
            ++i;
            if (i < argDesc.size() && (argDesc[i] == 'L' || std::isalpha(argDesc[i]))) {
                // Process the array element type
                descriptor += argDesc[i];
                if (argDesc[i] == 'L') {
                    ++i;
                    // Process the rest of the class type until ';'
                    while (i < argDesc.size() && argDesc[i] != ';') {
                        descriptor += argDesc[i++];
                    }
                    if (i < argDesc.size() && argDesc[i] == ';') {
                        descriptor += argDesc[i++];
                    }
                }
                else {
                    ++i; // For primitives (e.g., [I, [Z)
                }
            }
            descriptors.push_back(descriptor);
        }
        else if (argDesc[i] == 'L') {
            // Class type
            std::string descriptor = "L";
            ++i;
            while (i < argDesc.size() && argDesc[i] != ';') {
                descriptor += argDesc[i++];
            }
            if (i < argDesc.size() && argDesc[i] == ';') {
                descriptor += argDesc[i++];
            }
            descriptors.push_back(descriptor);
        }
        else {
            // Primitive type
            descriptors.push_back(std::string(1, argDesc[i]));
            ++i;
        }
    }

    return descriptors;
}

ExternalMethod::~ExternalMethod()
{
}
