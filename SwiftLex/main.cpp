#include <iostream>
#include <fstream>
#include <string>
#include "allNodes.h"
#include "nodes/dotHelpers.h"
#include "generation/classFileGeneration.h"
#include "tables/tables.h"
#include <filesystem>

bool _DRAW_DOT = false;

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern StmtListNode* _root;

void deleteDirectoryContents(const std::filesystem::path& dir);

int main(int argc, const char* argv[])
{
	std::string filename = "";
#ifdef  _DEBUG
	filename = "input.txt";
#else
	if (argc != 2)
	{
		std::cout << "Incorrect amount of args! The only argument is the file name" << std::endl;
		return 1;
	}

	filename = argv[1];
#endif //  _DEBUG
	std::string tempFileName = "temp_" + filename;
	std::ifstream inputFile(filename);
	std::ofstream tempOutput(tempFileName);
	std::string line;

	while (std::getline(inputFile, line)) tempOutput << line << '\n';
	tempOutput << '\n';

	inputFile.close();
	tempOutput.close();

	fopen_s(&yyin, tempFileName.c_str(), "r");
	
	if (!yyin)
	{
		std::cout << "Couldn't open file! Check the path!" << std::endl;
		return 1;
	}

	try {
		yyparse();
	}
	catch (const std::exception& e)
	{
		fclose(yyin);
		std::remove(tempFileName.c_str());
		return 1;
	}
	fclose(yyin);
	std::remove(tempFileName.c_str());

	// Before semantics

	if (_DRAW_DOT)
	{
		std::ofstream dotInitial;
		dotInitial.open("swift.dot");

		if (_root != nullptr) {
			dotInitial << "digraph swift {\n";
			_root->generateDot(dotInitial);
			dotInitial << "}\n";

			dotInitial.close();
			system("cd");
			system("Graphviz\\bin\\dot.exe -Tpng swift.dot > swift.png");
			system("swift.png");
		}
		else {
			std::cout << "File empty" << std::endl;
		}
	}	

	// After semantics
	_root = _root->semanticsTransform();

	if (_DRAW_DOT)
	{
		std::ofstream dotSemantics;
		dotSemantics.open("swiftSem.dot");

		if (_root != nullptr) {
			dotSemantics << "digraph swift {\n";
			clearDotCache();
			_root->generateDot(dotSemantics);
			dotSemantics << "}\n";

			dotSemantics.close();
			system("cd");
			system("Graphviz\\bin\\dot.exe -Tpng swiftSem.dot > swiftSem.png");
			system("swiftSem.png");
		}
		else {
			std::cout << "File empty" << std::endl;
		}
	}

	// Attribution
	auto classTable = ClassTable();
	_root->fillTable(&classTable, nullptr, nullptr);

	// Generation
	auto constTable = classTable.items["MainClass"]->constants;

	auto systemNameNum = constTable->findOrAddConstant(Utf8_C, "java/lang/System");
	auto systemClass = constTable->findOrAddConstant(Class_C, "", 0, 0, systemNameNum);
	auto outNameNum = constTable->findOrAddConstant(Utf8_C, "out");
	auto outTypeNum = constTable->findOrAddConstant(Utf8_C, "Ljava/io/PrintStream;");
	auto outNameandtypeNum = constTable->findOrAddConstant(NameAndType_C, "", 0, 0, outNameNum, outTypeNum);
	auto outFieldrefNum = constTable->findOrAddConstant(FieldRef_C, "", 0, 0, systemClass, outNameandtypeNum);
	auto outClassName = constTable->findOrAddConstant(Utf8_C, "java/io/PrintStream");
	auto outClass = constTable->findOrAddConstant(Class_C, "", 0, 0, outClassName);

	auto printNameNum = constTable->findOrAddConstant(Utf8_C, "println");
	auto printTypeNum = constTable->findOrAddConstant(Utf8_C, "(Ljava/lang/String;)V");
	auto printNameAndType = constTable->findOrAddConstant(NameAndType_C, "", 0, 0, printNameNum, printTypeNum);
	auto printMethodRef = constTable->findOrAddConstant(MethodRef_C, "", 0, 0, outClass, printNameAndType);

	auto strUtfNum = constTable->findOrAddConstant(Utf8_C, "Hello world! Yippee :)");
	auto strNum = constTable->findOrAddConstant(String_C, "", 0, 0, strUtfNum);

	// Delete old .class files
	deleteDirectoryContents("out");

	try
	{
		for (auto& classElem : classTable.items)
		{
			generateClassFile(classElem.second, "out/");
		}
	}
	catch (std::runtime_error error)
	{
		std::cout << "Code generation error: " << error.what() << std::endl;
	}

	//Чтобы запустить: 
	// идешь в папку out
	// выполняешь java MainClass
}

void deleteDirectoryContents(const std::filesystem::path& dir)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir))
		std::filesystem::remove_all(entry.path());
}
