%{

  #include "swift.tab.h"
  int yyerror(char *errormsg);
  int yyparse();
%}
  
%%

hi { printf("hi\n"); return HI;  }
("tchau"|"bye")"\n"   { return BYE; }
[-[]+.,><]            { return yytext[0]; }
.                     { printf("Unknow char");  }

%%

int main(void)
{
     fopen_s(&yyin, "input.txt", "r");
     yyparse();
     return 0;
}

int yywrap(void)
{
     return 0;
}

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}