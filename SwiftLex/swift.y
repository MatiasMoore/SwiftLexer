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

BINARY_PLUS UNARY_PLUS BINARY_MINUS
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
%left '+' '-'
%left '*' '/' '%'
%right '~' '!'
%nonassoc IS AS
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
    ;

stmt: varDeclaration {printf("P: stmt\n");}
    | funcDeclaration {printf("P: stmt\n");}
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
    | stmtList ';' stmt {printf("P: stmtList\n");}
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

exprReturn: RETURN expr
    ;

funcArg: ID ':' type
    ;

funcArgList: funcArg
    | funcArgList ',' funcArg
    ;

funcArgListE: %empty
    | funcArgList
    ;

funcReturnType: OP_FUNC_RETURN type
    ;

funcDeclaration: FUNC ID '(' funcArgListE ')' funcReturnType '{' stmtListE exprReturn'}' {printf("P: func declaration with return\n");}
    | FUNC ID '(' funcArgListE ')' '{' stmtListE '}' {printf("P: func declaration without return\n");}
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
        
varDeclaration: VAR varVarList {printf("P: varDeclaration\n");}
    | LET varVarList {printf("P: varDeclaration\n");}
    ;

expr: LITERAL_INT
    | LITERAL_FLOAT
    | LITERAL_STRING
    | ID
    | TRUE
    | FALSE
    | '~' expr
    | '!' expr
    | '-' expr
    | expr '+' expr
    | expr '-' expr
    | expr '/' expr
    | expr '*' expr
    | expr '%' expr
    | expr '<' expr
    | expr '>' expr
    | expr OP_GTE expr
    | expr OP_LTE expr
    | expr OP_EQ expr
    | expr OP_NEQ expr
    | expr '&' expr
    | expr '|' expr
    | expr '^' expr
    | expr OP_LOG_AND expr
    | expr OP_LOG_OR expr
    | expr OP_LSHIFT expr
    | expr OP_RSHIFT expr
    | expr OP_CLOSED_RANGE expr
    | expr OP_HALF_OPEN_RANGE expr
    | expr OP_NIL_COALESCE expr
    | expr IS type
    | expr AS type
    | expr AS '?' type
    | expr AS '!' type
    | expr '?' expr ':' expr
    | '(' expr ')'
    //function call result
    //field access using dot
    //array access
    ;
%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}