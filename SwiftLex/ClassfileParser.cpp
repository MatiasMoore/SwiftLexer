#include "ClassfileParser.h"
#include <sstream>
#include <map>
#include <fstream>
#include "ExceptionHelper.h"

uint16_t JavaClassFileParser::readUint16(std::istream& file)
{
    uint8_t bytes[2];
    file.read(reinterpret_cast<char*>(bytes), 2);
    return (bytes[0] << 8) | bytes[1];
}

uint32_t JavaClassFileParser::readUint32(std::ifstream& file)
{
    uint8_t bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

std::string JavaClassFileParser::readUTF8(std::ifstream& file, uint16_t length)
{
    std::vector<char> buffer(length);
    file.read(buffer.data(), length);
    return std::string(buffer.begin(), buffer.end());
}

std::vector<MethodAccessFlag> JavaClassFileParser::getMethodAccessFlagsFromInt(int flagsInt)
{
    std::vector<MethodAccessFlag> flags = {};

    if (flagsInt & M_ACC_PUBLIC) flags.push_back(M_ACC_PUBLIC);
    if (flagsInt & M_ACC_PRIVATE) flags.push_back(M_ACC_PRIVATE);
    if (flagsInt & M_ACC_PROTECTED) flags.push_back(M_ACC_PROTECTED);
    if (flagsInt & M_ACC_STATIC) flags.push_back(M_ACC_STATIC);
    if (flagsInt & M_ACC_FINAL) flags.push_back(M_ACC_FINAL);
    if (flagsInt & M_ACC_SYNCHRONIZED) flags.push_back(M_ACC_SYNCHRONIZED);
    if (flagsInt & M_ACC_BRIDGE) flags.push_back(M_ACC_BRIDGE);
    if (flagsInt & M_ACC_VARARGS) flags.push_back(M_ACC_VARARGS);
    if (flagsInt & M_ACC_NATIVE) flags.push_back(M_ACC_NATIVE);
    if (flagsInt & M_ACC_ABSTRACT) flags.push_back(M_ACC_ABSTRACT);
    if (flagsInt & M_ACC_STRICT) flags.push_back(M_ACC_STRICT);
    if (flagsInt & M_ACC_SYNTHETIC) flags.push_back(M_ACC_SYNTHETIC);

    return flags;
}

std::vector<FieldAccessFlag> JavaClassFileParser::getFieldAccessFlagsFromInt(int flagsInt)
{
    std::vector<FieldAccessFlag> flags;

    if (flagsInt & F_ACC_PUBLIC) flags.push_back(F_ACC_PUBLIC);
    if (flagsInt & F_ACC_PRIVATE) flags.push_back(F_ACC_PRIVATE);
    if (flagsInt & F_ACC_PROTECTED) flags.push_back(F_ACC_PROTECTED);
    if (flagsInt & F_ACC_STATIC) flags.push_back(F_ACC_STATIC);
    if (flagsInt & F_ACC_FINAL) flags.push_back(F_ACC_FINAL);
    if (flagsInt & F_ACC_VOLATILE) flags.push_back(F_ACC_VOLATILE);
    if (flagsInt & F_ACC_TRANSIENT) flags.push_back(F_ACC_TRANSIENT);
    if (flagsInt & F_ACC_SYNTHETIC) flags.push_back(F_ACC_SYNTHETIC);
    if (flagsInt & F_ACC_ENUM) flags.push_back(F_ACC_ENUM);

    return flags;
}

void JavaClassFileParser::addFromFileToClassTable(std::string filename, ClassTable* classTable)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) 
        throw std::runtime_error("Failed to open file \"" + filename + "\"!" + LINE_AND_FILE);
    

    // Read and verify the magic number
    uint32_t magic = readUint32(file);
    if (magic != 0xCAFEBABE) 
        throw std::runtime_error("Invalid .class file! Magic number mismatch!" + LINE_AND_FILE);

    // Skip minor and major version
    file.ignore(4);

    // Read constant pool count
    uint16_t constantPoolCount = readUint16(file);
    std::vector<std::string> constantPool(constantPoolCount);
    std::map<int, int> classToUtf8Index = {};

    // Parse constant pool
    for (uint16_t i = 1; i < constantPoolCount; i++)
    {
        uint8_t tag;
        file.read(reinterpret_cast<char*>(&tag), 1);

        switch (tag) 
        {
        case 1: 
        { // UTF-8
            uint16_t length = readUint16(file);
            constantPool[i] = readUTF8(file, length);
            break;
        }
        case 3: // Integer
        case 4: // Float
            file.ignore(4);
            break;
        case 5: // Long
        case 6: // Double
            file.ignore(8);
            ++i; // Long and Double take two slots
            break;
        case 7: // Class reference
        {
            uint16_t utfRefNum = readUint16(file);
            classToUtf8Index[i] = utfRefNum;
        }
        break;
        case 8: // String reference
            file.ignore(2);
            break;
        case 9:  // Field reference
        case 10: // Method reference
        case 11: // Interface Method reference
        case 12: // Name and Type
            file.ignore(4);
            break;
        case 15: // Method Handle
            file.ignore(3);
            break;
        case 16: // Method Type
            file.ignore(2);
            break;
        case 18: // Invoke Dynamic
            file.ignore(4);
            break;
        default:
            std::cerr << "Unknown constant pool tag: " << static_cast<int>(tag) << std::endl;
            return;
        }
    }

    // Read access flags
    file.ignore(2);

    // The indexes are for class reference not the utf8 that we need
    uint16_t thisClassIndex = readUint16(file);
    uint16_t superClassIndex = readUint16(file);

    // Find out the utf8 for the class references
    thisClassIndex = classToUtf8Index[thisClassIndex];
    superClassIndex = classToUtf8Index[superClassIndex];

    // The indices are 1-based for the constant pool, but the array is 0-based

    if (thisClassIndex < 1 || thisClassIndex >= constantPool.size())
        throw std::runtime_error("Invalid thisClass index!" + LINE_AND_FILE);

    std::string className = constantPool[thisClassIndex];

    if (superClassIndex < 1 || superClassIndex >= constantPool.size())
        throw std::runtime_error("Invalid superClass index!" + LINE_AND_FILE);

    std::string superClassName = constantPool[superClassIndex];

    auto thisClass = classTable->addExternalClass(className, superClassName);

    // Read interfaces count and skip interfaces
    uint16_t interfacesCount = readUint16(file);
    file.ignore(interfacesCount * 2);

    // Read fields count
    uint16_t fieldsCount = readUint16(file);
    for (uint16_t i = 0; i < fieldsCount; i++) 
    {
        uint16_t accessFlags = readUint16(file);
        uint16_t nameIndex = readUint16(file);
        uint16_t descriptorIndex = readUint16(file);

        auto fieldName = constantPool[nameIndex];
        auto fieldDesc = constantPool[descriptorIndex];

        thisClass->addField(fieldName, fieldDesc, getFieldAccessFlagsFromInt(accessFlags));

        // Skip the attributes
        uint16_t attributesCount = readUint16(file);
        for (uint16_t j = 0; j < attributesCount; j++) 
        {
            file.ignore(2); // Attribute name index
            uint32_t attributeLength = readUint32(file);
            file.ignore(attributeLength);
        }
    }

    // Read methods count
    uint16_t methodsCount = readUint16(file);
    for (uint16_t i = 0; i < methodsCount; i++) {
        uint16_t accessFlags = readUint16(file);
        uint16_t nameIndex = readUint16(file);
        uint16_t descriptorIndex = readUint16(file);

        auto methodName = constantPool[nameIndex];
        auto methodDesc = constantPool[descriptorIndex];

        thisClass->addMethod(methodName, methodDesc, getMethodAccessFlagsFromInt(accessFlags));

        // Skip the attributes
        uint16_t attributesCount = readUint16(file);
        for (uint16_t j = 0; j < attributesCount; j++) 
        {
            file.ignore(2); // Attribute name index
            uint32_t attributeLength = readUint32(file);
            file.ignore(attributeLength);
        }
    }

    file.close();
}
