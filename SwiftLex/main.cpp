#include <iostream>
#include <fstream>
#include <string>
#include "allNodes.h"
#include "nodes/dotHelpers.h"
#include "generation/classFileGeneration.h"
#include "tables/tables.h"

bool _DRAW_DOT = false;

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern StmtListNode* _root;

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


	// Generation
	auto testClassName = "testClass";
	auto classTable = ClassTable();
	classTable.items[testClassName] = new ClassTableElement(testClassName, "java/lang/Object");
	generateClassFile(classTable, testClassName);
}
