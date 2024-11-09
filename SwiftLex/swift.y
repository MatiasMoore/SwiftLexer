%{

  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  #include "allNodes.h"
  #include "globalVars.h"
  int yylex(void);
  int yyerror(const char *s);
  using namespace std;
  void switchStateToSubscript();
%}

%union {
	int ival;
	float fval;
	double dval;
    char* strval;
    class ExprNode* exprNode;
    class StmtNode* stmtNode;
    class StmtListNode* stmtListNode;
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

SUBSCRIPT_SQUARE_BRACKET SUBSCRIPT_ROUND_BRACKET

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
%left '[' ']' SUBSCRIPT_SQUARE_BRACKET
%right '(' ')' SUBSCRIPT_ROUND_BRACKET

// Nodes
%type<exprNode> expr
%type<stmtNode> stmt
%type<stmtListNode> stmtList
%type<stmtListNode> program

// Start
%start program

%%

program: stmtList {printf("P: program\n"); $$ = $1; _root = $$;}
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

stmt: varDeclaration {printf("P: stmt varDec\n");}
    | funcDeclaration {printf("P: stmt funcDec\n");}
    | constructorDeclaration {printf("P: stmt constructorDecl\n");}
    | destructorDeclaration {printf("P: stmt destructorDecl\n");}
    | exprReturn {printf("P: stmt return\n");}
    | classDeclaration {printf("P: stmt classDec\n");}
    | assignment {printf("P: stmt assignment\n");}
    | expr {printf("P: stmt expr\n"); $$ = StmtNode::createStmtExpr($1);}
    | expr ';' {printf("P: stmt expr\n"); $$ = StmtNode::createStmtExpr($1); $$->_hasSemicolon = true; }
    | enumDeclaration {printf("P: stmt enum\n");}
    | ifElse {printf("P: stmt ifElse\n");}
    | whileLoop {printf("P: stmt whileLoop\n");}
    | repeatWhileLoop {printf("P: stmt repeatWhileLoop\n");}
    | forInLoop {printf("P: stmt forInLoop\n");}
    | switchCase {printf("P: stmt switch\n");}
    | structDeclaration {printf("P: stmt struct\n");}
	;

stmtList: stmt {printf("P: stmtList start\n"); $$ = StmtListNode::createStmtList($1);}
	| stmtList stmt {
        if (!($1->_stmtVec.back()->_hasSemicolon) && @1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: stmtList\n");
            $$ = $1->appendStmt($2);
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

funcDecIncomplete: FUNC ID anyRoundBracket funcDeclArgListE ')' funcReturnTypeE '{' stmtListE '}' {printf("P: func declIncomplete\n");}
    ;

    
overloadableOperators: '+'
    | BINARY_MINUS
    | UNARY_MINUS
    | '*'
    | '/'
    | '%'
    | '<'
    | '>'
    | OP_GTE
    | OP_LTE
    | OP_EQ
    | OP_NEQ
    | '&'
    | '|'
    | '^'
    | OP_LOG_AND
    | OP_LOG_OR
    | OP_LSHIFT
    | OP_RSHIFT
    | OP_CLOSED_RANGE
    | OP_HALF_OPEN_RANGE
    | OP_NIL_COALESCE
    ;

funcOverloadOperatorIncomplete: FUNC overloadableOperators anyRoundBracket funcDeclArgListE ')' funcReturnTypeE '{' stmtListE '}' {printf("P: func overload Operator Incomplete\n");}
	;

funcDeclaration: modifiersWordsList funcDecIncomplete {printf("P: func declaration prefix\n");}
    | funcDecIncomplete {printf("P: func declaration default\n");}
    
    // operator overloading
    | funcOverloadOperatorIncomplete {printf("P: func overload Operator\n");}
    | modifiersWordsList funcOverloadOperatorIncomplete {printf("P: func overload Operator with prefix\n");}
    ;

modifiersWords: STATIC 
	| FINAL 
	| OVERRIDE 
	| OPEN
    | PUBLIC
    | INTERNAL
    | FILEPRIVATE
    | PRIVATE
    | PREFIX
	| POSTFIX
	;

modifiersWordsList: modifiersWords {printf("P: modifiersWordsList\n");}
	| modifiersWordsList modifiersWords {printf("P: modifiersWordsList\n");}
	;

constructorDeclaration: INIT anyRoundBracket funcDeclArgListE ')' '{' stmtListE '}' {printf("P: constructor declaration\n");}
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

classDeclaration: modifiersWordsList classDeclIncomplete {printf("P: class declaration with prefix\n");}
    | classDeclIncomplete {printf("P: class declaration default\n");}
    ;

structDeclIncomplete: STRUCT ID '{' stmtListE '}' {printf("P: structDeclIncomplete\n");}
	| STRUCT ID ':' ID '{' stmtListE '}' {printf("P: structDeclIncomplete\n");}
	;

structDeclaration: modifiersWordsList structDeclIncomplete {printf("P: struct declaration with prefix\n");}
	| structDeclIncomplete {printf("P: struct declaration default\n");}
	;

exprList: expr {printf("P: exprList\n");}
    | exprList ',' expr {printf("P: exprList\n");}
    ;

exprListE: %empty
    | exprList
    ;

funcCall: ID SUBSCRIPT_ROUND_BRACKET exprListE ')' {printf("P: funcCall exprList\n");}
    | ID SUBSCRIPT_ROUND_BRACKET funcCallArgList ')' {printf("P: funcCall labelArgs\n");}
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

varDeclaration:modifiersWordsList varDeclIncommplete {printf("P: variable declaration with prefix\n");}
    | varDeclIncommplete {printf("P: variable declaration default\n");}
    ;

typeList: type {printf("P: enum: typeList \n");}
    | typeList ',' type {printf("P: enum: typeList \n");}
    ;

enumId: ID anyRoundBracket typeList ')' {printf("P: enum: enumId \n");}
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

enumDeclarationIncomplete: ENUM ID '{' enumDefinitionList '}'  {printf("P: enumDeclaration\n");}
    ;

enumDeclaration: modifiersWordsList enumDeclarationIncomplete {printf("P: enumDeclaration\n");}
    | enumDeclarationIncomplete {printf("P: enumDeclaration\n");}
	;

whileLoop: WHILE exprList '{' stmtList '}' {printf("P: whileLoop\n");}
    | WHILE exprList '{' '}' {printf("P: whileLoop\n");}
    ;

repeatWhileLoop: REPEAT '{' stmtList '}' WHILE exprList {printf("P: repeatWhileLoop\n");}
    | REPEAT '{' '}' WHILE exprList {printf("P: repeatWhileLoop\n");}
    ;

whereClause: WHERE expr
    ;

forInLoop: FOR expr IN expr whereClause '{' stmtList '}' {printf("P: forInLoop\n");}
    | FOR expr IN expr '{' stmtList '}' {printf("P: forInLoop\n");}

    | FOR expr IN expr whereClause '{' '}' {printf("P: forInLoop\n");}
    | FOR expr IN expr '{' '}' {printf("P: forInLoop\n");}
    ;

ifElse: IF exprList '{' stmtList '}' {printf("P: ifElse\n");}
    | IF exprList '{' stmtList '}' ELSE '{' stmtList '}' {printf("P: ifElse with else\n");}
    | IF exprList '{' stmtList '}' ELSE ifElse {printf("P: ifElse else if\n");}

    | IF exprList '{' '}' {printf("P: ifElse\n");}
    | IF exprList '{' '}' ELSE '{' stmtList '}' {printf("P: ifElse with else\n");}
    | IF exprList '{' stmtList '}' ELSE '{' '}' {printf("P: ifElse with else\n");}
    | IF exprList '{' '}' ELSE '{' '}' {printf("P: ifElse with else\n");}
    | IF exprList '{' '}' ELSE ifElse {printf("P: ifElse else if\n");}
    ;

switchCase: SWITCH expr '{'caseList defaultCase '}' {printf("P: switch\n");}
    | SWITCH expr '{'defaultCase '}' {printf("P: switch\n");}
    | SWITCH expr '{'caseList'}' {printf("P: switch\n");}
	;

caseElement: CASE caseElementExpr ':' stmtList {printf("P: case\n");}
    ;

caseElementExpr: exprList
    | exprList whereClause {printf("P: case where\n");}
    | LET ID whereClause {printf("P: case let\n");}
    // TODO: add exprlist in round brackets
    ;

caseList: caseElement {printf("P: caseList\n");}
	| caseList caseElement {printf("P: caseList\n");}
	;

defaultCase: DEFAULT ':' stmtList {printf("P: defaultCase\n");}
	;

expr: LITERAL_INT {printf("P: expr int\n"); switchStateToSubscript(); $$ = ExprNode::createInt($1);}
    | LITERAL_FLOAT {printf("P: expr float\n"); switchStateToSubscript(); $$ = ExprNode::createFloat($1);}
    | LITERAL_STRING {printf("P: expr string\n"); switchStateToSubscript(); $$ = ExprNode::createString($1);}
    | ID {printf("P: expr ID\n"); switchStateToSubscript(); $$ = ExprNode::createId($1);}
    | TRUE {printf("P: expr TRUE\n"); switchStateToSubscript(); $$ = ExprNode::createBool(true);}
    | FALSE {printf("P: expr FALSE\n"); switchStateToSubscript(); $$ = ExprNode::createBool(false);}
    | '~' expr {printf("P: expr ~\n"); switchStateToSubscript();}
    | '!' expr {printf("P: expr !\n"); switchStateToSubscript();}
    | UNARY_MINUS expr {printf("P: expr unary -\n"); switchStateToSubscript();}
    | expr '+' expr {printf("P: expr +\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Sum, $1, $3);}
    | expr BINARY_MINUS expr {printf("P: expr -\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Sub, $1, $3);}
    | expr '/' expr {printf("P: expr /\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Div, $1, $3);}
    | expr '*' expr {printf("P: expr *\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Mul, $1, $3);}
    | expr '%' expr {printf("P: expr %\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Modulus, $1, $3);}
    | expr '<' expr {printf("P: expr <\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::LT, $1, $3);}
    | expr '>' expr {printf("P: expr >\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::GT, $1, $3);}
    | expr OP_GTE expr {printf("P: expr >=\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::GTE, $1, $3);}
    | expr OP_LTE expr {printf("P: expr <=\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::LTE, $1, $3);}
    | expr OP_EQ expr {printf("P: expr ==\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::EQ, $1, $3);}
    | expr OP_NEQ expr {printf("P: expr !=\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::NEQ, $1, $3);}
    | expr '&' expr {printf("P: expr &\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::BitAnd, $1, $3);}
    | expr '|' expr {printf("P: expr |\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::BitOr, $1, $3);}
    | expr '^' expr {printf("P: expr ^\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::BitXor, $1, $3);}
    | expr OP_LOG_AND expr {printf("P: expr &&\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::LogAnd, $1, $3);}
    | expr OP_LOG_OR expr {printf("P: expr ||\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::LogOr, $1, $3);}
    | expr OP_LSHIFT expr {printf("P: expr <<\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Lshift, $1, $3);}
    | expr OP_RSHIFT expr {printf("P: expr >>\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Rshift, $1, $3);}
    | expr OP_CLOSED_RANGE expr {printf("P: expr ...\n"); switchStateToSubscript();}
    | expr OP_HALF_OPEN_RANGE expr {printf("P: expr ..<\n"); switchStateToSubscript();}
    | expr OP_NIL_COALESCE expr {printf("P: expr ??\n"); switchStateToSubscript();}
    | expr IS type {printf("P: expr is\n"); switchStateToSubscript();}
    | expr AS type {printf("P: expr as\n"); switchStateToSubscript();}
    | expr AS '?' type {printf("P: expr as ?\n"); switchStateToSubscript();}
    | expr AS '!' type {printf("P: expr as !\n"); switchStateToSubscript();}
    | expr '?' expr ':' expr {printf("P: expr ternary ? :\n"); switchStateToSubscript();}
    | anyRoundBracket expr ')' {printf("P: expr brackets\n"); $$ = $2; switchStateToSubscript();}
    | funcCall {printf("P: expr funcCall\n"); switchStateToSubscript(); switchStateToSubscript();}
    | SUPER '.' funcCall {printf("P: expr super funcCall\n"); switchStateToSubscript();}
    | expr '.' funcCall {printf("P: expr func access\n"); switchStateToSubscript();}
    | SELF '.' funcCall {printf("P: expr self func access\n"); switchStateToSubscript();}
    | expr '.' ID {printf("P: expr field access\n"); switchStateToSubscript();}
    | SELF '.' ID {printf("P: expr self fieldAccess\n"); switchStateToSubscript();}
    // WARNING THIS CAUSES 2 CONFLICTS 
    // BUT THEY ARE RESOLVED CORRECTLY BY DEFAULT
    // TODO: RESOLVE CONFLICT EXPLICITLY
    | '[' exprList ']' {printf("P: expr array\n"); switchStateToSubscript();}
    | expr SUBSCRIPT_SQUARE_BRACKET expr ']' {printf("P: expr array indexing\n"); switchStateToSubscript();}
    ;

anyRoundBracket: '('
	| SUBSCRIPT_ROUND_BRACKET
	;
        
%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}