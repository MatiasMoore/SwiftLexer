#include <iostream>

int yylex();
extern FILE* yyin;

int main()
{
	setlocale(LC_ALL, "russian");
	fopen_s(&yyin, "input.txt", "r");
	if (!yyin)
		std::cout << "Couldn't open file! Check the path!" << std::endl;
	yylex();
}