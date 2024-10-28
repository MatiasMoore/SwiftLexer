%{

  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  int yylex(void);
  int yyerror(const char *s);
  using namespace std;
%}

%union {
	int ival;
	float fval;
	double dval;
    char* strval;
}
%locations

%token<dval> LITERAL_FLOAT
%token<ival> LITERAL_INT
%token<strval> LITERAL_STRING
%token<strval> ID

%token 
HI BYE ASSOCIATED_TYPE CLASS DEINIT 
ENUM EXTENSION FILEPRIVATE 
IMPORT INIT INOUT
INTERNAL LET OPEN
OPERATOR PRIVATE PRECEDENCEGROUP
PROTOCOL PUBLIC RETHROWS
STATIC STRUCT SUBSCRIPT
TYPEALIAS FUNC VAR
BREAK CASE CATCH
CONTINUE DEFAULT DEFER

DO ELSE FALLTHROUGH  
FOR GUARD IF  
IN REPEAT RETURN  
THROW SWITCH WHERE  
WHILE ANY AS  
FALSE IS NIL  
SELF SELF_TYPE SUPER  
THROWS TRUE TRY

ASSOCIATIVITY CONVENIENCE DIDSET  
DYNAMIC FINAL GET  
INDIRECT INFIX LAZY  
LEFT MUTATING NONE  
NONMUTATING OPTIONAL OVERRIDE  
POSTFIX PRECEDENCE PREFIX  
PROTOCOL_UPPERCASE REQUIRED RIGHT  
SET SOME TYPE  
UNOWNED WEAK WILLSET 

HASH_AVAILABLE HASH_COLORLITERAL HASH_COLUMN  
HASH_DSOHANDLE HASH_ELSEIF HASH_ELSE  
HASH_ENDIF HASH_ERROR HASH_FILEID  
HASH_FILELITERAL HASH_FILEPATH HASH_FILE  
HASH_FUNCTION HASH_IF HASH_IMAGELITERAL  
HASH_KEYPATH HASH_LINE HASH_SELECTOR  
HASH_SOURCELOCATION HASH_WARNING

BINARY_MINUS
UNARY_MINUS
OP_PLUS_ASSIGN OP_MINUS_ASSIGN OP_DIV_ASSIGN  
OP_MUL_ASSIGN OP_MOD_ASSIGN OP_LSHIFT  
OP_RSHIFT OP_EQ OP_LTE  
OP_GTE OP_NEQ OP_LOG_AND  
OP_LOG_OR OP_NIL_COALESCE OP_INC  
OP_DEC OP_CLOSED_RANGE  
OP_HALF_OPEN_RANGE OP_FUNC_RETURN

TYPE_BOOL TYPE_STRING TYPE_CHARACTER  
TYPE_INT8 TYPE_INT16 TYPE_INT32  
TYPE_INT64 TYPE_INT TYPE_UINT8  
TYPE_UINT16 TYPE_UINT32 TYPE_UINT64  
TYPE_UINT TYPE_FLOAT TYPE_FLOAT80  
TYPE_DOUBLE

%left '=' ID
%right '?' ':' 
%right OP_NIL_COALESCE
%left OP_LOG_OR
%left OP_LOG_AND
%left '|'
%left '^'
%left '&'
%nonassoc OP_EQ OP_NEQ 
%nonassoc '<' '>' OP_LTE OP_GTE OP_RSHIFT OP_LSHIFT
%left OP_CLOSED_RANGE OP_HALF_OPEN_RANGE
%left BINARY_MINUS '+'
%left '*' '/' '%' '.'
%right '~' '!'
%nonassoc IS AS
%left UNARY_PLUS UNARY_MINUS
%left '[' ']'
%right '(' ')'

%start program

%%

program: stmtList {printf("P: program\n");}
    ;


type: TYPE_BOOL
    | TYPE_STRING
    | TYPE_CHARACTER
    | TYPE_INT8
    | TYPE_INT16
    | TYPE_INT32
    | TYPE_INT64
    | TYPE_INT
    | TYPE_UINT8
    | TYPE_UINT16
    | TYPE_UINT32
    | TYPE_UINT64
    | TYPE_UINT
    | TYPE_FLOAT
    | TYPE_FLOAT80
    | TYPE_DOUBLE
    | ID
    | '[' type ']'
    ;

semicolonE: %empty
    | ';'
    ;

stmt: varDeclaration semicolonE {printf("P: stmt varDec\n");}
    | funcDeclaration semicolonE {printf("P: stmt funcDec\n");}
    | constructorDeclaration semicolonE {printf("P: stmt constructorDecl\n");}
    | destructorDeclaration semicolonE {printf("P: stmt destructorDecl\n");}
    | exprReturn semicolonE {printf("P: stmt return\n");}
    | classDeclaration semicolonE {printf("P: stmt classDec\n");}
    | assignment semicolonE {printf("P: stmt assignment\n");}
    | expr semicolonE {printf("P: stmt expr\n");}
    | enumDeclaration semicolonE {printf("P: stmt enum\n");}
    | ifElse semicolonE {printf("P: stmt ifElse\n");}
	;

stmtList: stmt {printf("P: stmtList\n");}
	| stmtList stmt {
        if (@1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: stmtList\n");
		}
    }
	;

stmtListE: %empty
    | stmtList
    ;

    /*
Grammar of a statement

statement -> expression ;?
statement -> declaration ;?
statement -> loop-statement ;?
statement -> branch-statement ;?
statement -> labeled-statement ;?
statement -> control-transfer-statement ;?
statement -> defer-statement ;?
statement -> do-statement ;?
statement -> compiler-control-statement
statements -> statement statements?
    */

exprReturn: RETURN expr {printf("P: return\n");}
    ;

funcDeclArg: ID ':' type {printf("P: func arg with value\n");}
    | ID ':' type '=' expr {printf("P: func arg with value\n");}
    ;

funcDeclArgList: funcDeclArg {printf("P: funcDeclArgList\n");}
    | funcDeclArgList ',' funcDeclArg {printf("P: funcDeclArgList\n");}
    ;

funcDeclArgListE: %empty
    | funcDeclArgList
    ;

funcReturnType: OP_FUNC_RETURN type
    ;

funcReturnTypeE: %empty
    | funcReturnType
    ;

accessModifier: OPEN
    | PUBLIC
    | INTERNAL
    | FILEPRIVATE
    | PRIVATE
    ;

funcDecIncomplete: FUNC ID '(' funcDeclArgListE ')' funcReturnTypeE '{' stmtListE '}' {printf("P: func declIncomplete\n");}
    ;

funcDeclaration: funcPrefix funcDecIncomplete {printf("P: func declaration prefix\n");}
    | funcDecIncomplete {printf("P: func declaration default\n");}
    ;

funcPrefix: accessModifier STATIC 
    | STATIC accessModifier 
    | STATIC 
    | accessModifier FINAL 
    | FINAL accessModifier 
    | FINAL 
    | accessModifier 

    | OVERRIDE accessModifier STATIC 
    | accessModifier OVERRIDE STATIC 
    | accessModifier STATIC OVERRIDE 

    | OVERRIDE STATIC accessModifier 
    | STATIC OVERRIDE accessModifier 
    | STATIC accessModifier OVERRIDE 

    | OVERRIDE STATIC 
    | STATIC OVERRIDE 

    | OVERRIDE accessModifier FINAL
    | accessModifier OVERRIDE FINAL
    | accessModifier FINAL OVERRIDE

    | OVERRIDE FINAL accessModifier
    | FINAL OVERRIDE accessModifier
    | FINAL accessModifier OVERRIDE

    | OVERRIDE FINAL 
    | FINAL OVERRIDE 

    | OVERRIDE accessModifier 
    | accessModifier OVERRIDE 

    | OVERRIDE 
    ;

varPrefix: accessModifier STATIC 
    | STATIC accessModifier 
    | STATIC 
    | accessModifier FINAL 
    | FINAL accessModifier 
    | FINAL 
    | accessModifier
    ;

classPrefix: accessModifier FINAL 
    | FINAL accessModifier 
    | FINAL 
    | accessModifier 
    ;

constructorDeclaration: INIT '(' funcDeclArgListE ')' '{' stmtListE '}' {printf("P: constructor declaration\n");}
    ;

destructorDeclaration: DEINIT '{' stmtListE '}' {printf("P: destructor declaration\n");}
    ;

funcCallArg: ID ':' expr {printf("P: funcCallArg\n");}
    ;

funcCallArgList: funcCallArg {printf("P: funcCallArgList\n");}
    | funcCallArgList ',' funcCallArg {printf("P: funcCallArgList\n");}
    ;
    
classDeclIncomplete: CLASS ID '{' stmtListE '}' {printf("P: classDeclIncomplete\n");}
    | CLASS ID ':' ID '{' stmtListE '}' {printf("P: classDeclIncomplete\n");}
    ;

classDeclaration: classPrefix classDeclIncomplete {printf("P: class declaration with prefix\n");}
    | classDeclIncomplete {printf("P: class declaration default\n");}
    ;

exprList: expr {printf("P: exprList\n");}
    | exprList ',' expr {printf("P: exprList\n");}
    ;

exprListE: %empty
    | exprList
    ;

funcCall: ID '(' exprListE ')' {printf("P: funcCall exprList\n");}
    | ID '(' funcCallArgList ')' {printf("P: funcCall labelArgs\n");}
    ;

assignment: expr '=' expr {printf("P: assignment\n");}
    ;

varIdWithComma: ID ',' {printf("P: varIdWithComma\n");}
    ;

varIdWithCommaList: varIdWithComma
    | varIdWithCommaList varIdWithComma
    ;

varIdWithType: ID ':' type {printf("P: varIdWithType\n");}
    ;
       
varIdListWithType: varIdWithCommaList varIdWithType {printf("P: varIdListWithType\n");}
    | varIdWithType {printf("P: varIdListWithType\n");}
    ;
            
varList: varIdWithType '=' expr {printf("P: varList\n");}
    | ID '=' expr {printf("P: varList\n");}
    | varIdListWithType {printf("P: varList\n");}
    ;

varVarList: varList
    | varVarList ',' varList
    ;
        
varDeclIncommplete: VAR varVarList {printf("P: varDeclIncommplete\n");}
    | LET varVarList {printf("P: varDeclIncommplete\n");}
    ;

varDeclaration:varPrefix varDeclIncommplete {printf("P: variable declaration with prefix\n");}
    | varDeclIncommplete {printf("P: variable declaration default\n");}
    ;

typeList: type {printf("P: enum: typeList \n");}
    | typeList ',' type {printf("P: enum: typeList \n");}
    ;

enumId: ID '(' typeList ')' {printf("P: enum: enumId \n");}
    | ID {printf("P: enum: enumId \n");}
    ;

enumIdList: enumId {printf("P: enum: enumIdList \n");}
    | enumIdList ',' enumId {printf("P: enum: enumIdList \n");}
    ;

enumDefinition: CASE enumIdList {printf("P: enum: enumDefinition \n");}
    ;

enumDefinitionList: enumDefinition {printf("P: enum: enumDefinitionList \n");}
    | enumDefinitionList ';' enumDefinition  {printf("P: enum: enumDefinitionList \n");}
    | enumDefinitionList enumDefinition  {
        if (@1.last_line == @2.first_line){
            yyerror("Syntax error: consecutive declarations on a line must be separated by ';'");
        } else {
        {printf("P: enum: enumDefinitionList \n");}
        }
    }
    ;

enumDeclaration: ENUM ID '{' enumDefinitionList '}'  {printf("P: enumDeclaration\n");}
    ;

ifElse: IF exprList '{' stmtList '}' {printf("P: ifElse\n");}
    | IF exprList '{' '}' {printf("P: ifElse\n");}
    | IF exprList '{' stmtList '}' ELSE '{' stmtList '}' {printf("P: ifElse with else\n");}
    | IF exprList '{' '}' ELSE '{' stmtList '}' {printf("P: ifElse with else\n");}
    | IF exprList '{' stmtList '}' ELSE '{' '}' {printf("P: ifElse with else\n");}
    | IF exprList '{' '}' ELSE '{' '}' {printf("P: ifElse with else\n");}
    | IF exprList '{' stmtList '}' ELSE ifElse {printf("P: ifElse else if\n");}
    | IF exprList '{' '}' ELSE ifElse {printf("P: ifElse else if\n");}
    ;

expr: LITERAL_INT {printf("P: expr int\n");}
    | LITERAL_FLOAT {printf("P: expr float\n");}
    | LITERAL_STRING {printf("P: expr string\n");}
    | ID {printf("P: expr ID\n");}
    | TRUE {printf("P: expr TRUE\n");}
    | FALSE {printf("P: expr FALSE\n");}
    | '~' expr {printf("P: expr ~\n");}
    | '!' expr {printf("P: expr !\n");}
    | UNARY_MINUS expr {printf("P: expr unary -\n");}
    | expr '+' expr {printf("P: expr +\n");}
    | expr BINARY_MINUS expr {printf("P: expr -\n");}
    | expr '/' expr {printf("P: expr /\n");}
    | expr '*' expr {printf("P: expr *\n");}
    | expr '%' expr {printf("P: expr %\n");}
    | expr '<' expr {printf("P: expr <\n");}
    | expr '>' expr {printf("P: expr >\n");}
    | expr OP_GTE expr {printf("P: expr >=\n");}
    | expr OP_LTE expr {printf("P: expr <=\n");}
    | expr OP_EQ expr {printf("P: expr ==\n");}
    | expr OP_NEQ expr {printf("P: expr !=\n");}
    | expr '&' expr {printf("P: expr &\n");}
    | expr '|' expr {printf("P: expr |\n");}
    | expr '^' expr {printf("P: expr ^\n");}
    | expr OP_LOG_AND expr {printf("P: expr &&\n");}
    | expr OP_LOG_OR expr {printf("P: expr ||\n");}
    | expr OP_LSHIFT expr {printf("P: expr <<\n");}
    | expr OP_RSHIFT expr {printf("P: expr >>\n");}
    | expr OP_CLOSED_RANGE expr {printf("P: expr ...\n");}
    | expr OP_HALF_OPEN_RANGE expr {printf("P: expr ..<\n");}
    | expr OP_NIL_COALESCE expr {printf("P: expr ??\n");}
    | expr IS type {printf("P: expr is\n");}
    | expr AS type {printf("P: expr as\n");}
    | expr AS '?' type {printf("P: expr as ?\n");}
    | expr AS '!' type {printf("P: expr as !\n");}
    | expr '?' expr ':' expr {printf("P: expr ternary ? :\n");}
    | '(' expr ')' {printf("P: expr brackets\n");}
    | funcCall {printf("P: expr funcCall\n");}
    | SUPER '.' funcCall {printf("P: expr super funcCall\n");}
    | expr '.' funcCall {printf("P: expr func access\n");}
    | SELF '.' funcCall {printf("P: expr self func access\n");}
    | expr '.' ID {printf("P: expr field access\n");}
    | SELF '.' ID {printf("P: expr self fieldAccess\n");}
    // WARNING THIS CAUSES 2 CONFLICTS 
    // BUT THEY ARE RESOLVED CORRECTLY BY DEFAULT
    // TODO: RESOLVE CONFLICT EXPLICITLY
    | '[' exprList ']' {printf("P: expr array\n");}
    | expr '[' expr ']' {printf("P: expr array indexing\n");}
    ;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}