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
    class ExprListNode* exprListNode;
    class TypeNode* typeNode;
    class VarDeclarationNode* varDeclNode;
    class VarDeclarationListNode* varDeclListNode;
    class StringList* strList;
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
%type<exprListNode> exprList
%type<stmtNode> stmt
%type<stmtNode> assignment
%type<stmtListNode> stmtList
%type<stmtListNode> program
%type<typeNode> type

%type<varDeclNode> varIdWithType
%type<varDeclListNode> varList
%type<varDeclListNode> varIdListWithType
%type<strval> varIdWithComma
%type<strList> varIdWithCommaList
%type<varDeclListNode> varVarList
%type<varDeclListNode> varDeclIncommplete 
//TODO: TEMPORARY!!!! SHOULD BE EXPANDED TO INCLUDE ACCESS MODIFIERS
%type<varDeclListNode> varDeclaration

// Start
%start program

%%

program: stmtList {printf("P: program\n"); $$ = $1; _root = $$;}
    ;


type: TYPE_BOOL {$$ = TypeNode::createType(TypeType::BoolT);}
    | TYPE_STRING {$$ = TypeNode::createType(TypeType::StringT);}
    | TYPE_CHARACTER {$$ = TypeNode::createType(TypeType::CharacterT);}
    | TYPE_INT8 {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_INT16 {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_INT32 {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_INT64 {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_INT {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_UINT8 {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_UINT16 {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_UINT32 {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_UINT64 {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_UINT {$$ = TypeNode::createType(TypeType::IntT);}
    | TYPE_FLOAT {$$ = TypeNode::createType(TypeType::FloatT);}
    | TYPE_FLOAT80 {$$ = TypeNode::createType(TypeType::FloatT);}
    | TYPE_DOUBLE {$$ = TypeNode::createType(TypeType::FloatT);}
    | ID {$$ = TypeNode::createIdType($1);}
    | '[' type ']' {$$ = TypeNode::createArrayType($2);}
    ;

stmt: varDeclaration {printf("P: stmt varDec\n"); $$ = StmtNode::createStmtVarDeclaration($1);}
    | varDeclaration ';' {printf("P: stmt varDec\n"); $$ = StmtNode::createStmtVarDeclaration($1); $$->_hasSemicolon = true;}
    | funcDeclaration {printf("P: stmt funcDec\n");}
    | constructorDeclaration {printf("P: stmt constructorDecl\n");}
    | destructorDeclaration {printf("P: stmt destructorDecl\n");}
    | exprReturn {printf("P: stmt return\n");}
    | classDeclaration {printf("P: stmt classDec\n");}
    | assignment {printf("P: stmt assignment\n"); $$ = $1;}
    | assignment ';' {printf("P: stmt assignment\n"); $$ = $1; $$->_hasSemicolon = true;}
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

stmtList: stmt {printf("P: stmtList start\n"); $$ = StmtListNode::createListNode($1);}
	| stmtList stmt {
        if (!($1->_vec.back()->_hasSemicolon) && @1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: stmtList\n");
            $$ = $1->appendNode($2);
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
    | FUNC ID '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' funcReturnTypeE '{' stmtListE '}' {printf("P: func declIncomplete generic\n");}
    | FUNC ID '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' funcReturnTypeE whereClause '{' stmtListE '}' {printf("P: func declIncomplete generic where\n");}

    | FUNC ID anyRoundBracket funcDeclArgListE ')' THROWS funcReturnTypeE '{' stmtListE '}' {printf("P: func declIncomplete throws\n");}
    | FUNC ID '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' THROWS funcReturnTypeE '{' stmtListE '}' {printf("P: func declIncomplete generic throws\n");}
    | FUNC ID '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' THROWS funcReturnTypeE whereClause '{' stmtListE '}' {printf("P: func declIncomplete generic where throws\n");}
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
    | INIT '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' '{' stmtListE '}' {printf("P: constructor declaration generic\n");}
    | INIT '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' whereClause '{' stmtListE '}' {printf("P: constructor declaration generic\n");}

    | INIT anyRoundBracket funcDeclArgListE ')' '{' stmtListE '}' THROWS {printf("P: constructor declaration\n");}
    | INIT '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' THROWS '{' stmtListE '}' {printf("P: constructor declaration generic\n");}
    | INIT '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' THROWS whereClause '{' stmtListE '}' {printf("P: constructor declaration generic\n");}
    ;

destructorDeclaration: DEINIT '{' stmtListE '}' {printf("P: destructor declaration\n");}
    ;

funcCallArg: ID ':' expr {printf("P: funcCallArg\n");}
    ;

funcCallArgList: funcCallArg {printf("P: funcCallArgList\n");}
    | funcCallArgList ',' funcCallArg {printf("P: funcCallArgList\n");}
    ;

genericIdList:  type {printf("P: genericIdList\n");}
    | ID ':' type {printf("P: genericIdList\n");}
    | genericIdList ',' ID {printf("P: genericIdList\n");}
    | genericIdList ',' ID ':' type  {printf("P: genericIdList\n");}
    ;

classDeclIncomplete: CLASS ID '{' stmtListE '}' {printf("P: classDeclIncomplete\n");}
    | CLASS ID ':' ID '{' stmtListE '}' {printf("P: classDeclIncomplete\n");}
    | CLASS ID '<' genericIdList '>' '{' stmtListE '}' {printf("P: classDeclIncomplete generic\n");}
    | CLASS ID '<' genericIdList '>' whereClause '{' stmtListE '}' {printf("P: classDeclIncomplete generic\n");}
    ;

classDeclaration: modifiersWordsList classDeclIncomplete {printf("P: class declaration with prefix\n");}
    | classDeclIncomplete {printf("P: class declaration default\n");}
    ;

structDeclIncomplete: STRUCT ID '{' stmtListE '}' {printf("P: structDeclIncomplete\n");}
	| STRUCT ID ':' ID '{' stmtListE '}' {printf("P: structDeclIncomplete\n");}
    | STRUCT ID '<' genericIdList '>' '{' stmtListE '}' {printf("P: structDeclIncomplete generic\n");}
    | STRUCT ID '<' genericIdList '>' whereClause '{' stmtListE '}' {printf("P: structDeclIncomplete generic\n");}
	;

structDeclaration: modifiersWordsList structDeclIncomplete {printf("P: struct declaration with prefix\n");}
	| structDeclIncomplete {printf("P: struct declaration default\n");}
	;

exprList: expr {printf("P: exprList\n"); $$ = ExprListNode::createListNode($1);}
    | exprList ',' expr {printf("P: exprList\n"); $$ = $$->appendNode($3);}
    ;

exprListE: %empty
    | exprList
    ;

genericIdType:  ID '<' typeList '>' {printf("P: genericIdType\n"); switchStateToSubscript();}
	;

funcCall: ID SUBSCRIPT_ROUND_BRACKET exprListE ')' {printf("P: funcCall exprList\n");}
    | genericIdType SUBSCRIPT_ROUND_BRACKET exprListE ')' {printf("P: funcCall generic exprList\n");}

    | ID SUBSCRIPT_ROUND_BRACKET funcCallArgList ')' {printf("P: funcCall labelArgs\n");}
    | genericIdType SUBSCRIPT_ROUND_BRACKET funcCallArgList ')' {printf("P: funcCall generic labelArgs\n");}
    ;

assignment: expr '=' expr {printf("P: assignment\n"); $$ = StmtNode::createStmtAssignment($1, $3); }
    ;

varIdWithComma: ID ',' {printf("P: varIdWithComma\n"); $$ = $1;}
    ;

varIdWithCommaList: varIdWithComma {$$ = StringList::createList($1);}
    | varIdWithCommaList varIdWithComma {$$ = $$->appendElem($2);}
    ;

varIdWithType: ID ':' type {printf("P: varIdWithType\n"); $$ = VarDeclarationNode::createFromType($1, $3);}
    ;
       
varIdListWithType: varIdWithCommaList varIdWithType 
{
    printf("P: varIdListWithType\n"); 

    //Type that other ids will use
    auto type = $2->_typeNode;

    //Create first element
    $$ = VarDeclarationListNode::createListNode(VarDeclarationNode::createFromType($1->_vec[0], type));

    //All other elements
    for (int i = 1; i < $1->_vec.size(); i++)
    {
        auto id = $1->_vec[i];
        $$ = $$->appendNode(VarDeclarationNode::createFromType(id, type));
    }

    //And the final one that already has the type
    $$ = $$->appendNode($2);
}
    | varIdWithType {printf("P: varIdListWithType\n"); $$ = VarDeclarationListNode::createListNode($1);}
    ;
            
varList: varIdWithType '=' expr {printf("P: varList\n"); $$ = VarDeclarationListNode::createListNode(VarDeclarationNode::createFromValueAndType($1->_varName, $3, $1->_typeNode));}
    | ID '=' expr {printf("P: varList\n"); $$ = VarDeclarationListNode::createListNode(VarDeclarationNode::createFromValue($1, $3));}
    | varIdListWithType {printf("P: varList\n"); $$ = $1;}
    ;

varVarList: varList {$$ = $1;}
    | varVarList ',' varList { $$ = $$->appendNodeList($3);}
    ;
        
varDeclIncommplete: VAR varVarList {printf("P: varDeclIncommplete\n"); $$ = $2;}
    | LET varVarList {printf("P: varDeclIncommplete\n"); $$ = $2;}
    ;

varDeclaration: modifiersWordsList varDeclIncommplete {printf("P: variable declaration with prefix\n"); $$ = $2;}
    | varDeclIncommplete {printf("P: variable declaration default\n"); $$ = $1;}
    ;

typeList: type {printf("P: typeList \n");}
    | typeList ',' type {printf("P: typeList \n");}
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

    //TODO: move this to expr
whereIdTypes: ID ':' type {printf("P: whereIdTypes\n");}
	| whereIdTypes ',' ID ':' type {printf("P: whereIdTypes\n");}
	;

whereClause: WHERE exprList {printf("P: whereClause\n");}
    | WHERE whereIdTypes {printf("P: whereClause ID type\n");}
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
    | '~' expr {printf("P: expr ~\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::BitNot, $2);}
    | '!' expr {printf("P: expr !\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::LogNot, $2);}
    | UNARY_MINUS expr {printf("P: expr unary -\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::UnaryMinus, $2);}
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
    | expr OP_CLOSED_RANGE expr {printf("P: expr ...\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::ClosedRange, $1, $3);}
    | expr OP_HALF_OPEN_RANGE expr {printf("P: expr ..<\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::HalfOpenRange, $1, $3);}
    | expr OP_NIL_COALESCE expr {printf("P: expr ??\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::NilCoalesce, $1, $3);}
    | expr IS type {printf("P: expr is\n"); switchStateToSubscript();}
    | expr AS type {printf("P: expr as\n"); switchStateToSubscript();}
    | expr AS '?' type {printf("P: expr as ?\n"); switchStateToSubscript();}
    | expr AS '!' type {printf("P: expr as !\n"); switchStateToSubscript();}
    | expr '?' expr ':' expr {printf("P: expr ternary ? :\n"); switchStateToSubscript(); $$ = ExprNode::createTernary($1, $3, $5);}
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
    | '[' exprList ']' {printf("P: expr array\n"); switchStateToSubscript(); $$ = ExprNode::createArray($2);}
    | expr SUBSCRIPT_SQUARE_BRACKET expr ']' {printf("P: expr array indexing\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Subscript, $1, $3);}
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