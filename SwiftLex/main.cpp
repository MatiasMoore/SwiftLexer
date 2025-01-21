#include <iostream>
#include <fstream>
#include <string>
#include "allNodes.h"
#include "nodes/dotHelpers.h"
#include "generation/classFileGeneration.h"
#include "tables/tables.h"
#include <filesystem>
#include "RTLHelper.h"
#include "GlobalSettings.h"
#include "ClassfileParser.h"

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

	if (GlobalSettings::_DRAW_DOT)
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
		// Add main class and method for free stmts
		// Main method
		auto mainFuncArgType = TypeNode::createArrayType(TypeNode::createType(TypeType::StringT));
		auto mainFuncArg = FuncDeclArgNode::createPositionalArg("args", mainFuncArgType, nullptr);
		auto mainFuncArgs = FuncDeclArgListNode::createListNode(mainFuncArg);
		auto mainFuncReturn = StmtNode::createStmtReturn(ReturnNode::createVoidReturn());
		auto mainFuncBody = StmtListNode::createListNode(mainFuncReturn);
		auto mainFuncDecl = FuncDeclNode::createRegular("main", mainFuncArgs, mainFuncBody, nullptr, false);
		auto mainFuncModifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(Public));
		mainFuncModifiers->appendNode(AccessModifierNode::createModifier(Static));
		mainFuncDecl->addModifiers(mainFuncModifiers);
		auto mainFuncDeclStmt = StmtNode::createStmtFuncDecl(mainFuncDecl);

		// Main class
		auto mainClassBody = StmtListNode::createListNode(mainFuncDeclStmt);
		auto mainClassDecl = ClassDeclNode::createClass("MainClass", mainClassBody);
		auto mainClassDeclStmt = StmtNode::createClassDecl(mainClassDecl);

		StmtListNode* newRoot = StmtListNode::createListNode(mainClassDeclStmt);

		if (_root != nullptr)
		{
			for (auto& stmt : _root->_vec)
			{
				// For classes
				if (stmt->_type == StmtType::ClassDecl)
				{
					newRoot->appendNodeBeforeNode(stmt, mainClassDeclStmt);
				}
				// For functions
				else if (stmt->_type == StmtType::FuncDecl)
				{
					// Make all free methods public and static
					auto funcDecl = stmt->_funcDecl;
					if (!funcDecl->_hasModifiers)
					{
						auto defaultModifiers = AccessModifierListNode::createListNode(AccessModifierNode::createModifier(Public));
						defaultModifiers->appendNode(AccessModifierNode::createModifier(Static));
						funcDecl->addModifiers(defaultModifiers);
					}

					mainClassBody->appendNodeBeforeNode(stmt, mainFuncDeclStmt);
				}
				else // All other stmts
				{
					mainFuncBody->appendNodeBeforeNode(stmt, mainFuncReturn);
				}
			}
		}
		
		_root = newRoot;

		_root = _root->semanticsTransform(newStack)->typecast<StmtListNode>();
	}
	catch (std::runtime_error error)
	{
		std::cout << "Semantics transform error: " << error.what() << std::endl;
		return 1;
	}

	if (GlobalSettings::_DRAW_DOT)
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

	// Add default java object
	auto javaObj = classTable.addExternalClass("java/lang/Object", "");
	javaObj->addMethod("<init>", "()V", { MethodAccessFlag::M_ACC_PUBLIC });

	try
	{
		// Delete old .class files
		deleteDirectoryContents(generatedClassFilesDirectory);
	}
	catch (...)
	{
		std::cout << "Failed to delete previous files! Make sure the files in directory \"" << 
			generatedClassFilesDirectory << "\" are not use by another process!" << std::endl;
		return 1;
	}
	

	// Generate rtl .class files and add them as external
	for (const auto& entry : std::filesystem::directory_iterator(rtlSourceDirectory))
	{
		auto pathToJavaFile = entry.path().generic_string();
		std::string compilationCommand = "javac " + pathToJavaFile + " -d " + generatedClassFilesDirectory;
		int result = system(compilationCommand.c_str());
		if (result != 0)
		{
			std::cout << "Failed to compile rtl file \"" << pathToJavaFile << "\"!" << std::endl;
			return 1;
		}

		auto pathToCompiledFile = generatedClassFilesDirectory + pathToJavaFile;
		
		// Replace .java with .class
		pathToCompiledFile = pathToCompiledFile.substr(0, pathToCompiledFile.find_last_of('.')) + ".class";

		try
		{
			JavaClassFileParser::addFromFileToClassTable(pathToCompiledFile, &classTable);
		}
		catch (std::runtime_error error)
		{
			std::cout << "Failed to add external class from file \"" << pathToCompiledFile << "\"! " << error.what() << std::endl;
			return 1;
		}
	}

	try
	{
		_root->fillTable(&classTable, nullptr, nullptr, true);
	}
	catch (std::runtime_error error)
	{
		std::cout << "Initial table filling error: " << error.what() << std::endl;
		return 1;
	}

	try
	{
		_root->fillTable(&classTable, nullptr, nullptr);
	}
	catch (std::runtime_error error)
	{
		std::cout << "Table filling error: " << error.what() << std::endl;
		return 1;
	}


	// Generate .class files based on _root
	try
	{
		for (auto& classElem : classTable.getInternalClasses())
		{
			generateClassFile(classElem, generatedClassFilesDirectory);
		}
	}
	catch (std::runtime_error error)
	{
		std::cout << "Code generation error: " << error.what() << std::endl;
		return 1;
	}
	std::cout << std::endl;
	
	//Execute

	if (GlobalSettings::_EXEC_MAINCLASS) {
		std::cout << "//---------Program exec---------\\\\" << std::endl;
		//TODO maybe implement proper stackmap frames
		std::string javaExecutionCommand = "java -noverify -cp " + generatedClassFilesDirectory + " MainClass";
		system(javaExecutionCommand.c_str());
	}
}

void deleteDirectoryContents(const std::filesystem::path& dir)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir))
		std::filesystem::remove_all(entry.path());
}
