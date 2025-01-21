#include "RTLHelper.h"

void RTLHelper::addRTLToClassTable(ClassTable* classTable)
{
	for (auto& [className, methods] : _nonStaticMethods)
	{
		for (auto& method : methods)
		{
			addExternalMethod(classTable, className, method, _publicFlags);
		}
	}

	for (auto& [className, methods] : _staticMethods)
	{
		for (auto& method : methods)
		{
			addExternalMethod(classTable, className, method, _publicStaticFlags);
		}
	}
}

void RTLHelper::addExternalMethod(ClassTable* classTable, std::string className, MethodInfo methodInfo,  std::vector<MethodAccessFlag> flags)
{
	auto class_ = classTable->findClass(className);
	if (class_ == nullptr)
	{
		class_ = classTable->addExternalClass(className, _baseC);
	}
	auto desc = methodInfo.descriptor;
	auto argsDesc = desc.substr(desc.find('(') + 1, desc.find(')') - desc.find('(') - 1);
	auto isStatic = std::find(flags.begin(), flags.end(), MethodAccessFlag::M_ACC_STATIC) != flags.end();
	auto isMethodExist = class_->findMethod(methodInfo.name, argsDesc, isStatic);
	if (isMethodExist == nullptr)
	{
		class_->addMethod(methodInfo.name, methodInfo.descriptor, flags);
		return;
	}

	if (isMethodExist->getDescriptor() == methodInfo.descriptor)
	{
		throw std::runtime_error("RTL error: Method " + methodInfo.name + " with descriptor " + methodInfo.descriptor + " already exists in class " + className + "!");
	}
	else {
		//print warning
		std::cout << "RTL WARNING:" << "Method " << methodInfo.name << " with descriptor " << methodInfo.descriptor << " have same args descriptor but different return type in class " << className << "!" << std::endl;
	}
	class_->addMethod(methodInfo.name, methodInfo.descriptor, flags);

}
