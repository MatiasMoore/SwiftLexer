#include <iostream>
#include <fstream>
#include <string>
#include "allNodes.h"

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

	std::ofstream dotFile;
	dotFile.open("swift.dot");

	dotFile << "digraph swift {\n";
	if (_root != nullptr) {
		_root->generateDot(dotFile);
		dotFile << "}\n";

		dotFile.close();
		system("cd");
		system("Graphviz\\bin\\dot.exe -Tpng swift.dot > swift.png");
		system("swift.png");
	}
	else {
		std::cout << "File empty" << std::endl;
	}
}
