#include <iostream>

int yylex();
extern FILE* yyin;

int main()
{
	fopen_s(&yyin, "test.txt", "r");
	if (!yyin)
		std::cout << "Couldn't open file! Check the path!" << std::endl;
	yylex();
}