%option noyywrap
%option never-interactive

%{
#include <stdio.h>
#include <iostream>

%}

%%

[0-9]+		{std::cout << "Found number " << yytext << std::endl; }

%%