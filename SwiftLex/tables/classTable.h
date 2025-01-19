#pragma once
#include <string>
#include <map>
#include "constTable.h"
#include "methodTable.h"
#include "../ExceptionHelper.h"
#include "fieldTable.h"

class ExternalClass
{
public:
	ExternalClass(std::string name, std::string baseName);
	
	ExternalMethod* addMethod(std::string name, std::string descriptor);

	ExternalField* addExternalField(std::string varName, std::string descriptor);

	ExternalField* addExternalField(ExternalField* externalField);

	ExternalField* findExternalField(std::string varName);
protected:
	std::string _name;
	std::string _baseName;

	std::map<std::string, class ExternalMethod*> _externalMethodMap = {};
	std::map<std::string, class ExternalField*> _externalFieldMap = {};
};

/* ћне кажетс€ нужно помен€ть назначение InternalClass и External class потому что дл€ External методов и полей нужно знать им€ класса(Int, String, Float и т.д.) чтобы € мог определ€ть где искать методы и пол€.
 ј в ExternalClass хранитс€ только им€ одного класса и его методы. »з за этого у мен€ возникает костыль в виде списка списков External (RTL) классов дл€ полей

 я бы вообще немного назначение вот этих классов (ExternalClass и Internal) помен€л, чтобы RTL классы не отличались от наших обычных сгенерированных

 “огда поиск methodRef или fieldRef преобразитс€ и станет проще
 —ейчас это проиходит так:
	1.1 ≈сли тип пол€ - €, то ищу в себе

	1.2 ≈сли тип пол€ другой класс, то ищу в наших классах

	1.3 ≈сли не нашел, то ищу в RTL классах
	(в этом пункте не вижу смысла потому что мы не сможем иметь классы с одинаковыми именами, чтобы они замен€ли RTL)

 ѕроще было бы если бы все классы лежали в одном списке
 ѕредлагаю вот так:
	1.1 ≈сли тип пол€ - €, то ищу в себе
	1.2 ≈сли тип пол€ другой класс, то ищу в общем списке других классов

 Ћибо 3 вариант - € могу хер забить на сохранение используемых переменных и если нужно будет что-то найти искать онли в constant_pool, но тогда возможно вылезут какие-то проблемы (например с пол€ми с одинаковым названием)
 */
class InternalClass : public ExternalClass
{
public:
	InternalClass(std::string name, std::string baseName, class ClassTable* _classTable);

    std::map<std::string, class InternalMethod*> _internalMethodMap = {};
	std::map<std::string, class InternalField*> _currentInternalFieldMap = {};

	// std::map<CLASS_NAME, std::map<FIELD_NAME, class InternalField*>> _usedInternalFieldMap = {};
	std::map<std::string, std::map<std::string, class InternalField*>> _usedCompilatedClassFieldMap = {}; // ƒл€ наших классов

	// std::map<CLASS_NAME, std::map<FIELD_NAME, class InternalField*>>
	std::map<std::string, std::map<std::string, class ExternalField*>> _usedExternalClassFieldMap = {}; // ƒл€ RTL классов вот этот костыль _externalFieldMap не используетс€ потому что нужно знать им€ класса

	ConstantTable* _constTable;

	InternalMethod* addMethod(int accessFlag, std::string name, std::string descriptor, class StmtListNode* body);
	
	/*! Add internal field to the class.
		* \return fieldRef: int
	*/
	InternalField* addInternalField(std::string varName, std::string descriptor, std::vector<FieldAccessFlag> flags, ExprNode* constValue = nullptr, std::string className = "");
	InternalField* addCompilatedClassInternalField(InternalField* internalField);

	InternalField* findInternalField(std::string varName, std::string className = "");

	ExternalField* addExternalField(std::string varName, std::string descriptor, std::string className);
	ExternalField* addExternalField(ExternalField* externalField);

	ExternalField* findCurrentExternalField(std::string varName, std::string className);

	int findOrAddFieldRef(std::string varName, std::string className = "");

private:
	class ClassTable* _classTable;

	int _classRef;
	int _nameRef;
	int _baseRef;

	InternalField* findCurrentInternalField(std::string varName);
	InternalField* findUsedCompilatedClassInternalField(std::string varName, std::string className);
};

class ClassTable
{
public:
	std::map<std::string, ExternalClass*> _externalClasses = {};

	std::map<std::string, InternalClass*> _internalClasses = {};

	InternalClass* addInternalClass(std::string name, std::string baseName);

	ExternalClass* addExternalClass(std::string name, std::string baseName);

	InternalClass* findInternalClass(std::string name);

	ExternalClass* findExternalClass(std::string name);
};

