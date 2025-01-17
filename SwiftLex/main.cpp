#include <iostream>
#include <fstream>
#include <string>
#include "allNodes.h"
#include "nodes/dotHelpers.h"
#include "generation/classFileGeneration.h"
#include "tables/tables.h"
#include <filesystem>

bool _DRAW_DOT = true;
bool _EXEC_MAINCLASS = true;

std::string generatedClassFilesDirectory = "out/";
std::string rtlSourceDirectory = "rtl/";

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
	auto newStack = SemanticsStack();

	try
	{
		_root = _root->semanticsTransform(newStack)->typecast<StmtListNode>();
	}
	catch (std::runtime_error error)
	{
		std::cout << "Semantics transform error: " << error.what() << std::endl;
		return 1;
	}

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

	try
	{
		_root->fillTable(&classTable, nullptr, nullptr);
	}
	catch (std::runtime_error error)
	{
		std::cout << "Table filling error: " << error.what() << std::endl;
		return 1;
	}


	// Generation

	// Delete old .class files
	deleteDirectoryContents(generatedClassFilesDirectory);
	
	// Generate rtl .class files
	for (const auto& entry : std::filesystem::directory_iterator(rtlSourceDirectory))
	{
		auto test = entry.path().generic_string();
		std::string compilationCommand = "javac " + entry.path().generic_string() + " -d " + generatedClassFilesDirectory;
		system(compilationCommand.c_str());
	}

	// Generate .class files based on _root
	try
	{
		for (auto& classElem : classTable.items)
		{
			generateClassFile(classElem.second, generatedClassFilesDirectory);
		}
	}
	catch (std::runtime_error error)
	{
		std::cout << "Code generation error: " << error.what() << std::endl;
		return 1;
	}
	std::cout << std::endl;
	
	//Execute

	if (_EXEC_MAINCLASS) {
		std::cout << "//---------Program exec---------\\\\" << std::endl;
		std::string javaExecutionCommand = "java -cp " + generatedClassFilesDirectory + " MainClass";
		system(javaExecutionCommand.c_str());
	}
}

void deleteDirectoryContents(const std::filesystem::path& dir)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir))
		std::filesystem::remove_all(entry.path());
}
