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
    class FuncCallArgNode* funcCallArgNode;
    class FuncCallArgListNode* funcCallArgListNode;
    class FuncCallNode* funcCallNode;
    class ReturnNode* returnNode;
    class LoopNode* loopNode;
    class IfElseNode* ifElseNode;
    class FuncDeclArgNode* funcDeclArgNode;
    class FuncDeclArgListNode* funcDeclArgListNode;
    class TypeForGenericListNode* typeForGenericListNode;
    class FuncDeclNode* funcDeclNode;
    class ThrowNode* throwNode;
    class TryNode* tryNode;
    class DoCatchNode* doCatchNode;
    class CatchNode* catchNode;
    class CaseElementNode* caseElementNode;
    class CaseElementListNode* caseElementListNode;
    class SwitchNode* switchNode;
    class AccessModifierNode* accessModifierNode;
    class AccessModifierListNode* accessModifierListNode;
    class IdNode* idNode;
    class IdListNode* idListNode;
    class EnumCaseNode* enumCaseNode;
    class EnumDeclarationNode* enumDeclarationNode;
    class StructDeclarationNode* structDeclarationNode;
    class ConstructorDeclNode* constructorDeclNode;
    class DestructorDeclNode* destructorDeclNode;
    class ClassDeclNode* classDeclNode;
    enum OverloadableOperatorType overloadOperatorType;
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

BINARY_MINUS PREFIX_MINUS POSTFIX_MINUS
BINARY_PLUS PREFIX_PLUS POSTFIX_PLUS
BINARY_DIV PREFIX_DIV POSTFIX_DIV
BINARY_MUL PREFIX_MUL POSTFIX_MUL
BINARY_MOD PREFIX_MOD POSTFIX_MOD
BINARY_LOG_AND PREFIX_LOG_AND POSTFIX_LOG_AND
BINARY_LOG_OR PREFIX_LOG_OR POSTFIX_LOG_OR
BINARY_NOT PREFIX_NOT POSTFIX_NOT
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

SUBSCRIPT_SQUARE_BRACKET FUNC_CALL_ROUND_BRACKET

%left '=' ID

%right '?' ':'  

%right OP_NIL_COALESCE  

%left BINARY_LOG_OR  
%left BINARY_LOG_AND 

%left '^' 

%nonassoc OP_EQ OP_NEQ  
%nonassoc '<' '>' OP_LTE OP_GTE OP_RSHIFT OP_LSHIFT  

%left OP_CLOSED_RANGE OP_HALF_OPEN_RANGE  
%left BINARY_NOT
%left BINARY_MINUS  
%left BINARY_PLUS   
%left '|'           

%left BINARY_MUL BINARY_DIV BINARY_MOD '&' 
%left '.'  

%nonassoc IS AS  

%right '~'  

%right PREFIX_NOT PREFIX_PLUS PREFIX_MINUS PREFIX_DIV PREFIX_MUL PREFIX_MOD PREFIX_LOG_AND PREFIX_LOG_OR
%left POSTFIX_LOG_AND POSTFIX_LOG_OR POSTFIX_NOT POSTFIX_MINUS POSTFIX_PLUS POSTFIX_DIV POSTFIX_MUL POSTFIX_MOD

%left '[' ']' SUBSCRIPT_SQUARE_BRACKET  

%right '(' ')' FUNC_CALL_ROUND_BRACKET  

// Nodes
%type<exprNode> expr
%type<exprListNode> exprList
%type<returnNode> return
%type<stmtNode> returnStmt

%type<stmtNode> stmtClassInnerIncomplete
%type<stmtNode> stmtClassInner
%type<stmtListNode> stmtClassInnerList
%type<stmtListNode> stmtClassInnerListE

%type<stmtNode> stmtStructInnerIncomplete
%type<stmtNode> stmtStructInner
%type<stmtListNode> stmtStructInnerList
%type<stmtListNode> stmtStructInnerListE

%type<stmtNode> stmtEnumInnerIncomplete
%type<stmtNode> stmtEnumInner
%type<stmtListNode> stmtEnumInnerList
%type<stmtListNode> stmtEnumInnerListE

%type<stmtNode> lowLevelStmtIncomplete
%type<stmtNode> lowLevelStmt
%type<stmtListNode> lowLevelStmtList
%type<stmtListNode> lowLevelStmtListE

%type<stmtListNode> lowLevelStmtListNoReturn

%type<stmtNode> topLevelStmtIncomplete
%type<stmtNode> topLevelStmt
%type<stmtListNode> topLevelStmtList
%type<stmtListNode> topLevelStmtListE

%type<stmtNode> assignment
%type<stmtNode> stmtOperators
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

%type<funcCallArgNode> funcCallArg
%type<funcCallArgListNode> funcCallArgList
%type<funcCallNode> funcCall

%type<loopNode> forInLoop
%type<loopNode> repeatWhileLoop
%type<loopNode> whileLoop

%type<ifElseNode> ifElse

// Function declaration
%type<funcDeclArgNode> funcDeclArg
%type<funcDeclArgListNode> funcDeclArgList
%type<funcDeclArgListNode> funcDeclArgListE
%type<typeNode> funcReturnType
%type<typeNode> funcReturnTypeE
%type<typeForGenericListNode> genericIdList
%type<funcDeclNode> funcDecIncomplete
%type<funcDeclNode> funcDeclaration
%type<overloadOperatorType> overloadableOperators 
%type<funcDeclNode> funcOverloadOperatorIncomplete

// Error handling
%type<throwNode> exprThrow
%type<tryNode> tryStmt
%type<doCatchNode> doCatchStmt

// Switch case
%type<caseElementNode> caseElement
%type<caseElementListNode> caseList
%type<caseElementNode> defaultCase
%type<switchNode> switchCase

// Access modifiers
%type<accessModifierNode> modifiersWords;
%type<accessModifierListNode> modifiersWordsList;

// Enum
%type<idListNode> enumIdList
%type<enumCaseNode> enumCase
%type<enumDeclarationNode> enumDeclarationIncomplete
%type<enumDeclarationNode> enumDeclaration

// Struct
%type<structDeclarationNode> structDeclIncomplete
%type<structDeclarationNode> structDeclaration

// Class
%type<constructorDeclNode> constructorDeclIncomplete
%type<constructorDeclNode> constructorDeclaration
%type<destructorDeclNode> destructorDeclaration
%type<classDeclNode> classDeclIncomplete
%type<classDeclNode> classDeclaration

// Start
%start program

%%

program: topLevelStmtListE {printf("P: program\n"); $$ = $1; _root = $$;}
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

    /* CLASS STMT */
stmtClassInnerIncomplete: funcDeclaration {printf("P: stmtClassInnerIncomplete funcdecl\n"); $$ = StmtNode::createStmtFuncDecl($1);}
    | varDeclaration {printf("P: stmtClassInnerIncomplete varDec\n"); $$ = StmtNode::createStmtVarDeclaration($1);}
    | constructorDeclaration {printf("P: stmtClassInnerIncomplete constructor\n"); $$ = StmtNode::createStmtConstructorDecl($1);}
    | destructorDeclaration {printf("P: stmtClassInnerIncomplete destructor\n"); $$ = StmtNode::createStmtDestructorDecl($1);}
    ;

stmtClassInner: stmtClassInnerIncomplete { $$ = $1; }
    | stmtClassInnerIncomplete ';' { $$ = $1; $$->_hasSemicolon = true; }
    ;

stmtClassInnerList: stmtClassInner {printf("P: stmtClassInnerList\n"); $$ = StmtListNode::createListNode($1);}
	| stmtClassInnerList stmtClassInner {
        if (!($1->_vec.back()->_hasSemicolon) && @1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: stmtClassInnerList\n");
            $$ = $1->appendNode($2);
		}
    }
	;

stmtClassInnerListE: stmtClassInnerList { $$ = $1; }
    | %empty { $$ = nullptr; }
    ;
   
    /* STRUCT STMT */
stmtStructInnerIncomplete: varDeclaration {printf("P: stmtStructInnerIncomplete varDec\n"); $$ = StmtNode::createStmtVarDeclaration($1);}
	;

stmtStructInner: stmtStructInnerIncomplete { $$ = $1; }
	| stmtStructInnerIncomplete ';' { $$ = $1; $$->_hasSemicolon = true; }
	;

stmtStructInnerList: stmtStructInner {printf("P: stmtStructInnerList\n"); $$ = StmtListNode::createListNode($1);}
	| stmtStructInnerList stmtStructInner {
        if (!($1->_vec.back()->_hasSemicolon) && @1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: stmtStructInnerList\n");
            $$ = $1->appendNode($2);
		}
    }
	;

stmtStructInnerListE: stmtStructInnerList { $$ = $1; }
	| %empty { $$ = nullptr; }
	;

    /* ENUM STMT */
stmtEnumInnerIncomplete: enumCase {printf("P: stmtEnumInnerIncomplete enumCase\n"); $$ = StmtNode::createStmtEnumCase($1);}
	;

stmtEnumInner: stmtEnumInnerIncomplete { $$ = $1; }
    | stmtEnumInnerIncomplete ';' { $$ = $1; $$->_hasSemicolon = true; } 
	;

stmtEnumInnerList: stmtEnumInner {printf("P: stmtEnumInnerList\n"); $$ = StmtListNode::createListNode($1);}
	| stmtEnumInnerList stmtEnumInner {
        if (!($1->_vec.back()->_hasSemicolon) && @1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: stmtEnumInnerList\n");
            $$ = $1->appendNode($2);
		}
    }
	;

stmtEnumInnerListE: stmtEnumInnerList {$$ = $1;}
    | %empty { $$ = nullptr; }
	;

    /* LOW LEVEL STMT */
lowLevelStmtIncomplete: varDeclaration {printf("P: lowLevelStmtIncomplete varDec\n"); $$ = StmtNode::createStmtVarDeclaration($1);}
    | exprThrow {printf("P: lowLevelStmtIncomplete throw\n"); $$ = StmtNode::createStmtThrow($1);}
    | assignment {printf("P: lowLevelStmtIncomplete assignment\n"); $$ = $1;}
    | expr {printf("P: lowLevelStmtIncomplete expr\n"); $$ = StmtNode::createStmtExpr($1);}
    | ifElse {printf("P: lowLevelStmtIncomplete ifElse\n"); $$ = StmtNode::createStmtIfElse($1);}
    | whileLoop {printf("P: lowLevelStmtIncomplete whileLoop\n"); $$ = StmtNode::createStmtLoop($1);}
    | repeatWhileLoop {printf("P: lowLevelStmtIncomplete repeatWhileLoop\n"); $$ = StmtNode::createStmtLoop($1);}
    | forInLoop {printf("P: lowLevelStmtIncomplete forInLoop\n"); $$ = StmtNode::createStmtLoop($1);}
    | switchCase {printf("P: lowLevelStmtIncomplete switch\n"); $$ = StmtNode::createStmtSwitch($1);}
    | tryStmt {printf("P: lowLevelStmtIncomplete try\n"); $$ = StmtNode::createStmtTry($1);}
    | doCatchStmt {printf("P: lowLevelStmtIncomplete doCatch\n"); $$ = StmtNode::createStmtDo($1);}
    | stmtOperators {printf("P: lowLevelStmtIncomplete operators\n"); $$ = $1;}
    ;

lowLevelStmt: lowLevelStmtIncomplete { $$ = $1; }
	| lowLevelStmtIncomplete ';' { $$ = $1; $$->_hasSemicolon = true; }
	;

lowLevelStmtListNoReturn: lowLevelStmt {printf("P: lowLevelStmtList\n"); $$ = StmtListNode::createListNode($1);}
	| lowLevelStmtListNoReturn lowLevelStmt {
        if (!($1->_vec.back()->_hasSemicolon) && @1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: lowLevelStmtList\n");
            $$ = $1->appendNode($2);
		}
    }
	;

lowLevelStmtList:  lowLevelStmtListNoReturn returnStmt {
        if (!($1->_vec.back()->_hasSemicolon) && @1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: END funcStmtList lowLevelStmtList + RETURN\n");
            $$ = $1->appendNode($2);
		}
    }
    | lowLevelStmtListNoReturn {printf("P: END funcStmtList lowLevelStmtList only\n"); $$ = $1;}
    | returnStmt {printf("P: END funcStmtList RETURN only\n"); $$ = StmtListNode::createListNode($1);}
	;

lowLevelStmtListE: lowLevelStmtList {$$ = $1;}
	| %empty { $$ = nullptr; }
    ;

    /* TOP LEVEL STMT */
topLevelStmtIncomplete: funcDeclaration {printf("P: topLevelStmtIncomplete funcDec\n"); $$ = StmtNode::createStmtFuncDecl($1);}
    | classDeclaration {printf("P: topLevelStmtIncomplete classDec\n");}
    | enumDeclaration {printf("P: topLevelStmtIncomplete enum\n"); $$ = StmtNode::createStmtEnumDeclaration($1);}
    | structDeclaration {printf("P: topLevelStmtIncomplete struct\n"); $$ = StmtNode::createStmtStructDeclaration($1);}
	| lowLevelStmtIncomplete {printf("P: topLevelStmtIncomplete toplevel\n"); $$ = $1;}
    ;

topLevelStmt: topLevelStmtIncomplete { $$ = $1; }
	| topLevelStmtIncomplete ';' { $$ = $1; $$->_hasSemicolon = true; }
	;

topLevelStmtList: topLevelStmt {printf("P: topLevelStmtList start\n"); $$ = StmtListNode::createListNode($1);}
	| topLevelStmtList topLevelStmt {
        if (!($1->_vec.back()->_hasSemicolon) && @1.last_line == @2.first_line){
            yyerror("Syntax error: two statements in one line must be separated with a ';'");
        }
        else {
			printf("P: topLevelStmtList\n");
            $$ = $1->appendNode($2);
		}
    }
	;  

topLevelStmtListE: topLevelStmtList {$$ = $1;}
    | %empty {$$ =  nullptr;}
    ;

returnStmt: return {printf("P: stmt return\n"); $$ = StmtNode::createStmtReturn($1);}
    | return ';' {printf("P: stmt return\n"); $$ = StmtNode::createStmtReturn($1); $$->_hasSemicolon = true;}
    ;

return: RETURN expr  {printf("P: return\n"); $$ = ReturnNode::createExprReturn($2);}
    | RETURN {printf("P: return empty\n"); $$ = ReturnNode::createVoidReturn();}
    ;

exprThrow: THROW expr {printf("P: throw\n"); $$ = ThrowNode::createThrowExpr($2);}
	;

funcDeclArg: ID ':' type {printf("P: func arg with value\n"); $$ = FuncDeclArgNode::createLabeledArg($1, $3, nullptr);}
    | '_' ID ':' type {printf("P: func arg with value\n"); $$ = FuncDeclArgNode::createPositionalArg($2, $4, nullptr);}
    | ID ':' type '=' expr {printf("P: func arg with value\n"); $$ = FuncDeclArgNode::createLabeledArg($1, $3, $5);}
    | '_' ID ':' type '=' expr {printf("P: func arg with value\n"); $$ = FuncDeclArgNode::createPositionalArg($2, $4, $6);}
    ;

funcDeclArgList: funcDeclArg {printf("P: funcDeclArgList\n"); $$ = FuncDeclArgListNode::createListNode($1);}
    | funcDeclArgList ',' funcDeclArg {printf("P: funcDeclArgList\n"); $$ = $$->appendNode($3);}
    ;

funcDeclArgListE: %empty { $$ = nullptr; }
    | funcDeclArgList { $$ = $1; }
    ;

funcReturnType: OP_FUNC_RETURN type { $$ = $2; }
    ;

funcReturnTypeE: %empty { $$ = nullptr; }
    | funcReturnType { $$ = $1; }
    ;

    //TODO add where clause
funcDecIncomplete: FUNC ID anyRoundBracket funcDeclArgListE ')' funcReturnTypeE '{' lowLevelStmtListE '}' {
    printf("P: func declIncomplete\n");
    $$ = FuncDeclNode::createRegular($2, $4, $8, $6, false);
    }
    | FUNC ID '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' funcReturnTypeE '{' lowLevelStmtListE '}' {
    printf("P: func declIncomplete generic\n");
    $$ = FuncDeclNode::createGeneric($2, $4, $7, $11, $9, false);
    }
    | FUNC ID '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' funcReturnTypeE whereClause '{' lowLevelStmtListE '}' {
    printf("P: func declIncomplete generic where\n");
    }

    | FUNC ID anyRoundBracket funcDeclArgListE ')' THROWS funcReturnTypeE '{' lowLevelStmtListE '}' {
    printf("P: func declIncomplete throws\n");
    $$ = FuncDeclNode::createRegular($2, $4, $9, $7, true);
    }
    | FUNC ID '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' THROWS funcReturnTypeE '{' lowLevelStmtListE '}' {
    printf("P: func declIncomplete generic throws\n");
    $$ = FuncDeclNode::createGeneric($2, $4, $7, $12, $10, true);
    }
    | FUNC ID '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' THROWS funcReturnTypeE whereClause '{' lowLevelStmtListE '}' {
    printf("P: func declIncomplete generic where throws\n");
    }
    ;

    
overloadableOperators: BINARY_PLUS {$$ = OverloadableOperatorType::OpPLUS;}
    | PREFIX_PLUS {$$ = OverloadableOperatorType::OpPLUS;}
    | POSTFIX_PLUS {$$ = OverloadableOperatorType::OpPLUS;}
    | BINARY_MINUS {$$ = OverloadableOperatorType::OpMINUS;}
    | PREFIX_MINUS {$$ = OverloadableOperatorType::OpMINUS;}
    | POSTFIX_MINUS {$$ = OverloadableOperatorType::OpMINUS;}
    | BINARY_MUL {$$ = OverloadableOperatorType::OpMUL;}
    | PREFIX_MUL {$$ = OverloadableOperatorType::OpMUL;}
    | POSTFIX_MUL {$$ = OverloadableOperatorType::OpMUL;}
    | BINARY_DIV {$$ = OverloadableOperatorType::OpDIV;}
    | PREFIX_DIV {$$ = OverloadableOperatorType::OpDIV;}
    | POSTFIX_DIV {$$ = OverloadableOperatorType::OpDIV;}
    | BINARY_MOD {$$ = OverloadableOperatorType::OpMOD;}
    | PREFIX_MOD {$$ = OverloadableOperatorType::OpMOD;}
    | POSTFIX_MOD {$$ = OverloadableOperatorType::OpMOD;}
    | '<' {$$ = OverloadableOperatorType::OpLT;}
    | '>' {$$ = OverloadableOperatorType::OpGT;}
    | OP_GTE {$$ = OverloadableOperatorType::OpGTE;}
    | OP_LTE {$$ = OverloadableOperatorType::OpLTE;}
    | OP_EQ {$$ = OverloadableOperatorType::OpEQ;}
    | OP_NEQ {$$ = OverloadableOperatorType::OpNEQ;}
    | '&' {$$ = OverloadableOperatorType::OpBitAND;}
    | '|' {$$ = OverloadableOperatorType::OpBitOR;}
    | '^' {$$ = OverloadableOperatorType::OpBitXOR;}
    | BINARY_LOG_AND {$$ = OverloadableOperatorType::OpLogAND;}
    | PREFIX_LOG_AND {$$ = OverloadableOperatorType::OpLogAND;}
    | POSTFIX_LOG_AND {$$ = OverloadableOperatorType::OpLogAND;}
    | BINARY_LOG_OR {$$ = OverloadableOperatorType::OpLogOR;}
    | PREFIX_LOG_OR {$$ = OverloadableOperatorType::OpLogOR;}
    | POSTFIX_LOG_OR {$$ = OverloadableOperatorType::OpLogOR;}
    | OP_LSHIFT {$$ = OverloadableOperatorType::OpLSHIFT;}
    | OP_RSHIFT {$$ = OverloadableOperatorType::OpRSHIFT;}
    | OP_CLOSED_RANGE {$$ = OverloadableOperatorType::OpCLOSEDRANGE;}
    | OP_HALF_OPEN_RANGE {$$ = OverloadableOperatorType::OpHALFOPENRANGE;}
    | OP_NIL_COALESCE {$$ = OverloadableOperatorType::OpNILCOALESCE;}
    ;

funcOverloadOperatorIncomplete: FUNC overloadableOperators anyRoundBracket funcDeclArgListE ')' funcReturnTypeE '{' lowLevelStmtListE '}' {
    printf("P: func overload Operator Incomplete\n");
    $$ = FuncDeclNode::createRegularOperator($2, $4, $8, $6, false);
}
    | FUNC overloadableOperators anyRoundBracket funcDeclArgListE ')' THROWS funcReturnTypeE '{' lowLevelStmtListE '}' {
        printf("P: func overload Operator Incomplete\n");
        $$ = FuncDeclNode::createRegularOperator($2, $4, $9, $7, true);
        }
    | FUNC overloadableOperators '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' funcReturnTypeE '{' lowLevelStmtListE '}' {
        printf("P: func overload Operator Incomplete\n");
        $$ = FuncDeclNode::createGenericOperator($2, $4, $7, $11, $9, false);
        }
    | FUNC overloadableOperators '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' THROWS funcReturnTypeE '{' lowLevelStmtListE '}' {
        printf("P: func overload Operator Incomplete\n");
        $$ = FuncDeclNode::createGenericOperator($2, $4, $7, $12, $10, true);
        }
	;

    //TODO add modifiers to funcDecl node
    //TODO add operator overloading
funcDeclaration: modifiersWordsList funcDecIncomplete {printf("P: func declaration prefix\n"); $$ = $2; $$ = $$->addModifiers($1);}
    | funcDecIncomplete {printf("P: func declaration default\n"); $$ = $1;}
    
    // operator overloading
    | funcOverloadOperatorIncomplete {printf("P: func overload Operator\n"); $$ = $1;}
    | modifiersWordsList funcOverloadOperatorIncomplete {printf("P: func overload Operator with prefix\n"); $$ = $2; $$ = $$->addModifiers($1);}
    ;

modifiersWords: STATIC { $$ = AccessModifierNode::createModifier(AccessModifierType::Static);}
	| FINAL { $$ = AccessModifierNode::createModifier(AccessModifierType::Final);}
	| OVERRIDE { $$ = AccessModifierNode::createModifier(AccessModifierType::Override);}
	| OPEN { $$ = AccessModifierNode::createModifier(AccessModifierType::Open);}
    | PUBLIC { $$ = AccessModifierNode::createModifier(AccessModifierType::Public);}
    | INTERNAL { $$ = AccessModifierNode::createModifier(AccessModifierType::Internal);}
    | FILEPRIVATE { $$ = AccessModifierNode::createModifier(AccessModifierType::Fileprivate);}
    | PRIVATE { $$ = AccessModifierNode::createModifier(AccessModifierType::Private);}
    | PREFIX { $$ = AccessModifierNode::createModifier(AccessModifierType::Prefix);}
	| POSTFIX { $$ = AccessModifierNode::createModifier(AccessModifierType::Postfix);}
	;

modifiersWordsList: modifiersWords {printf("P: modifiersWordsList\n"); $$ = AccessModifierListNode::createListNode($1);}
	| modifiersWordsList modifiersWords {printf("P: modifiersWordsList\n"); $$ = $1->appendNode($2);}
	;

constructorDeclIncomplete: INIT anyRoundBracket funcDeclArgListE ')' '{' lowLevelStmtListE '}' { printf("P: constructor declaration\n"); $$ = ConstructorDeclNode::createConstructor($3, $6, false);}
    | INIT anyRoundBracket funcDeclArgListE ')' THROWS '{' lowLevelStmtListE '}' {printf("P: constructor declaration\n"); $$ = ConstructorDeclNode::createConstructor($3, $7, true);}

    | INIT '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' '{' lowLevelStmtListE '}' {printf("P: constructor declaration generic\n"); $$ = ConstructorDeclNode::createConstructorGeneric($3, $6, $9, false);}
    | INIT '<' genericIdList '>' anyRoundBracket funcDeclArgListE ')' THROWS '{' lowLevelStmtListE '}' {printf("P: constructor declaration generic\n"); $$ = ConstructorDeclNode::createConstructorGeneric($3, $6, $10, true);}
    ;

constructorDeclaration: modifiersWordsList constructorDeclIncomplete { $$ = $2; $$ = $$->addModifiers($1);}
    | constructorDeclIncomplete { $$ = $1;}
    ;

destructorDeclaration: DEINIT '{' lowLevelStmtListE '}' {printf("P: destructor declaration\n"); $$ = DestructorDeclNode::createDestructor($3);}
    ;

funcCallArg: ID ':' expr {printf("P: funcCallArg\n"); $$ = FuncCallArgNode::createFromExprAndName($3, $1);}
    ;

funcCallArgList: funcCallArg {printf("P: funcCallArgList\n"); $$ = FuncCallArgListNode::createListNode($1);}
    | funcCallArgList ',' funcCallArg {printf("P: funcCallArgList\n"); $$ = $$->appendNode($3);}
    ;

genericIdList: ID {printf("P: genericIdList\n"); $$ = TypeForGenericListNode::createListNode(TypeForGenericNode::createNoBaseClass($1));}
    | ID ':' ID {printf("P: genericIdList\n"); $$ = TypeForGenericListNode::createListNode(TypeForGenericNode::createWithBaseClass($1, $3));}
    | genericIdList ',' ID {printf("P: genericIdList\n"); $$ = $$->appendNode(TypeForGenericNode::createNoBaseClass($3));}
    | genericIdList ',' ID ':' ID  {printf("P: genericIdList\n"); $$ = $$->appendNode(TypeForGenericNode::createWithBaseClass($3, $5));}
    ;

classDeclIncomplete: CLASS ID '{' stmtClassInnerListE '}' {printf("P: classDeclIncomplete\n"); $$ = ClassDeclNode::createClass($2, $4, nullptr);}
    | CLASS ID ':' ID '{' stmtClassInnerListE '}' {printf("P: classDeclIncomplete\n"); $$ = ClassDeclNode::createClassWithBaseClass($2, $6, nullptr, $4);}
    | CLASS ID '<' genericIdList '>' '{' stmtClassInnerListE '}' {printf("P: classDeclIncomplete generic\n"); $$ = ClassDeclNode::createClass($2, $7, $4);}
    | CLASS ID '<' genericIdList '>' ':' ID '{' stmtClassInnerListE '}' {printf("P: classDeclIncomplete generic\n"); $$ = ClassDeclNode::createClassWithBaseClass($2, $9, $4, $7);}
    | CLASS ID '<' genericIdList '>' whereClause '{' stmtClassInnerListE '}' {printf("P: classDeclIncomplete generic\n");}
    ;

classDeclaration: modifiersWordsList classDeclIncomplete {printf("P: class declaration with prefix\n"); $$ = $2; $$ = $$->addModifiers($1);}
    | classDeclIncomplete {printf("P: class declaration default\n"); $$ = $1;}
    ;

structDeclIncomplete: STRUCT ID '{' stmtStructInnerListE '}' {printf("P: structDeclIncomplete\n"); $$ = StructDeclarationNode::createRegular($2, $4);}
	| STRUCT ID ':' ID '{' stmtStructInnerListE '}' {printf("P: structDeclIncomplete\n"); $$ = StructDeclarationNode::createRegular($2, $6);}
    | STRUCT ID '<' genericIdList '>' '{' stmtStructInnerListE '}' {printf("P: structDeclIncomplete generic\n");  $$ = StructDeclarationNode::createGeneric($2, $4, $7);}
    | STRUCT ID '<' genericIdList '>' whereClause '{' stmtStructInnerListE '}' {printf("P: structDeclIncomplete generic\n"); $$ = StructDeclarationNode::createGeneric($2, $4, $8);}
	;

structDeclaration: modifiersWordsList structDeclIncomplete {printf("P: struct declaration with prefix\n"); $$ = $2->addModifiers($1);}
	| structDeclIncomplete {printf("P: struct declaration default\n"); $$ = $1;}
	;

exprList: expr {printf("P: exprList\n"); $$ = ExprListNode::createListNode($1);}
    | exprList ',' expr {printf("P: exprList\n"); $$ = $$->appendNode($3);}
    ;

exprListE: %empty
    | exprList
    ;

genericIdType:  ID '<' typeList '>' {printf("P: genericIdType\n"); switchStateToSubscript();}
	;

funcCall: ID FUNC_CALL_ROUND_BRACKET ')' {printf("P: funcCall exprList\n"); $$ = FuncCallNode::createFuncCallNoArgs($1);}
	| genericIdType FUNC_CALL_ROUND_BRACKET ')' {printf("P: funcCall exprList\n");}
    | ID FUNC_CALL_ROUND_BRACKET exprList ')' {
        printf("P: funcCall exprList\n"); 

        //Convert exprList to argList

        //Convert first and create list
        auto exprList = $3;
        auto argList = FuncCallArgListNode::createListNode(FuncCallArgNode::createFromExpr(exprList->_vec[0]));

        //Convert the rest
        for (int i = 1; i < exprList->_vec.size(); i++)
        {
            argList = argList->appendNode(FuncCallArgNode::createFromExpr(exprList->_vec[i]));
        }

        //Finally create the func call node

        $$ = FuncCallNode::createFuncCall($1, argList);

    }
	| genericIdType FUNC_CALL_ROUND_BRACKET exprList ')' {printf("P: funcCall exprList\n");}
    | ID FUNC_CALL_ROUND_BRACKET funcCallArgList ')' {printf("P: funcCall labelArgs\n"); $$ = FuncCallNode::createFuncCall($1, $3);}
	| genericIdType FUNC_CALL_ROUND_BRACKET funcCallArgList ')' {printf("P: funcCall labelArgs\n");}
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

varDeclaration: modifiersWordsList varDeclIncommplete {printf("P: variable declaration with prefix\n"); $$ = $2; $$ = $$->addModifiers($1);}
    | varDeclIncommplete {printf("P: variable declaration default\n"); $$ = $1;}
    ;

typeList: type {printf("P: typeList \n");}
    | typeList ',' type {printf("P: typeList \n");}
    ;

enumIdList: ID {printf("P: enum: enumIdList \n"); $$ = IdListNode::createListNode(IdNode::create($1));}
    | enumIdList ',' ID {printf("P: enum: enumIdList \n"); $$ = $1->appendNode(IdNode::create($3));}
    ;

enumCase: CASE enumIdList {printf("P: enum: enumCase \n"); $$ = EnumCaseNode::createEnumCaseNode($2);}
    ;

enumDeclarationIncomplete: ENUM ID '{' stmtEnumInnerListE '}'  {printf("P: enumDeclaration\n"); $$ = EnumDeclarationNode::createEnumDeclarationNode($2, $4);}
    ;

enumDeclaration: modifiersWordsList enumDeclarationIncomplete {printf("P: enumDeclaration\n"); $$ = $2->addModifiers($1);}
    | enumDeclarationIncomplete {printf("P: enumDeclaration\n"); $$ = $1;}
	;

whileLoop: WHILE exprList '{' lowLevelStmtList '}' {printf("P: whileLoop\n"); $$ = LoopNode::createWhileLoop($2, $4);}
    | WHILE exprList '{' '}' {printf("P: whileLoop\n"); $$ = LoopNode::createWhileLoopNoBody($2);}
    ;

repeatWhileLoop: REPEAT '{' lowLevelStmtList '}' WHILE exprList {printf("P: repeatWhileLoop\n"); $$ = LoopNode::createRepeatWhileLoop($3, $6);}
    | REPEAT '{' '}' WHILE exprList {printf("P: repeatWhileLoop\n"); $$ = LoopNode::createRepeatWhileLoopNoBody($5);}
    ;

    //TODO: move this to expr
whereIdTypes: ID ':' type {printf("P: whereIdTypes\n");}
	| whereIdTypes ',' ID ':' type {printf("P: whereIdTypes\n");}
	;

whereClause: WHERE exprList {printf("P: whereClause\n");}
    | WHERE whereIdTypes {printf("P: whereClause ID type\n");}
    ;

    //TODO add where clause to loops
forInLoop: FOR ID IN expr whereClause '{' lowLevelStmtList '}' {printf("P: forInLoop\n"); }
    | FOR ID IN expr '{' lowLevelStmtList '}' {printf("P: forInLoop\n"); $$ = LoopNode::createForLoop($2, $4, $6);}

    | FOR ID IN expr whereClause '{' '}' {printf("P: forInLoop\n");}
    | FOR ID IN expr '{' '}' {printf("P: forInLoop\n"); $$ = LoopNode::createForLoopNoBody($2, $4);}
    ;

ifElse: IF exprList '{' lowLevelStmtList '}' {printf("P: ifElse\n"); $$ = IfElseNode::createSimple($2, $4, nullptr);}
    | IF exprList '{' lowLevelStmtList '}' ELSE '{' lowLevelStmtList '}' {printf("P: ifElse with else\n"); $$ = IfElseNode::createSimple($2, $4, $8);}
    | IF exprList '{' lowLevelStmtList '}' ELSE ifElse {printf("P: ifElse else if\n"); $$ = IfElseNode::createComplex($2, $4, $7);}

    | IF exprList '{' '}' {printf("P: ifElse\n"); $$ = IfElseNode::createSimple($2, nullptr, nullptr);}
    | IF exprList '{' '}' ELSE '{' lowLevelStmtList '}' {printf("P: ifElse with else\n"); $$ = IfElseNode::createSimple($2, nullptr, $7);}
    | IF exprList '{' lowLevelStmtList '}' ELSE '{' '}' {printf("P: ifElse with else\n"); $$ = IfElseNode::createSimple($2, $4, nullptr);}
    | IF exprList '{' '}' ELSE '{' '}' {printf("P: ifElse with else\n"); $$ = IfElseNode::createSimple($2, nullptr, nullptr);}
    | IF exprList '{' '}' ELSE ifElse {printf("P: ifElse else if\n"); $$ = IfElseNode::createComplex($2, nullptr, $6);}
    ;

switchCase: SWITCH expr '{'caseList defaultCase '}' {printf("P: switch\n"); $$ = SwitchNode::createSwitchNode($2, $4->appendNode($5));}
    | SWITCH expr '{'defaultCase '}' {printf("P: switch\n"); $$ = SwitchNode::createSwitchNode($2, CaseElementListNode::createListNode($4));}
    | SWITCH expr '{'caseList'}' {printf("P: switch\n"); $$ = SwitchNode::createSwitchNode($2, $4);}
	;

caseElement: CASE exprList ':' lowLevelStmtList {printf("P: case\n"); $$ = CaseElementNode::createCaseExprList($2, $4);}
    | CASE exprList whereClause ':' lowLevelStmtList {printf("P: case\n");}
    | CASE LET ID whereClause ':' lowLevelStmtList {printf("P: case\n");}
    ;

caseList: caseElement {printf("P: caseList\n"); $$ = CaseElementListNode::createListNode($1);}
	| caseList caseElement {printf("P: caseList\n"); $$ = $1->appendNode($2);}
	;

defaultCase: DEFAULT ':' lowLevelStmtList {printf("P: defaultCase\n"); $$ = CaseElementNode::createCaseDefault($3);}
	;

tryStmt: TRY expr {printf("P: try\n"); $$ = TryNode::create($2);}
	;

doCatchStmt: DO '{' lowLevelStmtListE '}' {printf("P: do \n"); $$ = DoCatchNode::createOnlyDoNode($3);}
    | doCatchStmt CATCH expr '.' ID '{' lowLevelStmtListE '}' {
        printf("P: do catch field access\n"); 
        auto fieldAccess = ExprNode::createFieldAccessExpr($3, $5);
        auto catchNode = CatchNode::createCatchFieldAccessNode(fieldAccess, $7); 
        $$ = $1->addCatchNode(catchNode);
    }
    | doCatchStmt CATCH IS ID '{' lowLevelStmtListE '}' {
        printf("P: do catch expr\n");
        auto typeNode = TypeNode::createIdType($4);
        auto catchNode = CatchNode::createCatchTypeNode(typeNode, $6);
        $$ = $1->addCatchNode(catchNode);
    }
    | doCatchStmt CATCH '{' lowLevelStmtListE '}' {printf("P: do catch\n"); auto catchNode = CatchNode::createCatchNode($4); $$ = $1->addCatchNode(catchNode);}
	;

stmtOperators: expr OP_MINUS_ASSIGN expr {
    printf("P: stmtOperator OP_MINUS_ASSIGN\n"); 
    $$ = StmtNode::createStmtAssignment($1, ExprNode::createBinaryOp(ExprType::Sub, $1, $3)); 
    }
    | expr OP_DIV_ASSIGN expr {
    printf("P: stmtOperator OP_DIV_ASSIGN\n");
    $$ = StmtNode::createStmtAssignment($1, ExprNode::createBinaryOp(ExprType::Div, $1, $3)); 
    }
    | expr OP_MUL_ASSIGN expr {
    printf("P: stmtOperator OP_MUL_ASSIGN\n");
    $$ = StmtNode::createStmtAssignment($1, ExprNode::createBinaryOp(ExprType::Mul, $1, $3)); 
    }
    | expr OP_MOD_ASSIGN expr {
    printf("P: stmtOperator OP_MOD_ASSIGN\n");
    $$ = StmtNode::createStmtAssignment($1, ExprNode::createBinaryOp(ExprType::Modulus, $1, $3)); 
    }
    | expr OP_PLUS_ASSIGN expr {
    printf("P: stmtOperator OP_PLUS_ASSIGN\n");
    $$ = StmtNode::createStmtAssignment($1, ExprNode::createBinaryOp(ExprType::Sum, $1, $3)); 
    }
    ;

expr: LITERAL_INT {printf("P: expr int\n"); switchStateToSubscript(); $$ = ExprNode::createInt($1);}
    | LITERAL_FLOAT {printf("P: expr float\n"); switchStateToSubscript(); $$ = ExprNode::createFloat($1);}
    | LITERAL_STRING {printf("P: expr string\n"); switchStateToSubscript(); $$ = ExprNode::createString($1);}
    | ID {printf("P: expr ID\n"); switchStateToSubscript(); $$ = ExprNode::createId($1);}
    | TRUE {printf("P: expr TRUE\n"); switchStateToSubscript(); $$ = ExprNode::createBool(true);}
    | FALSE {printf("P: expr FALSE\n"); switchStateToSubscript(); $$ = ExprNode::createBool(false);}
    | '~' expr {printf("P: expr ~\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::BitNot, $2);}

    | expr BINARY_NOT expr {printf("P: expr BINARY_NOT\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::BinaryNot, $1, $3);}
    | PREFIX_NOT expr {printf("P: expr PREFIX_NOT\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::LogNot, $2);}
    | expr POSTFIX_NOT {printf("P: expr POSTFIX_NOT\n"); switchStateToSubscript();}
    
    | expr BINARY_MINUS expr {printf("P: expr BINARY_MINUS\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Sub, $1, $3);}
    | PREFIX_MINUS expr {printf("P: expr PREFIX_MINUS\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::UnaryMinus, $2);}
    | expr POSTFIX_MINUS {printf("P: expr postfix POSTFIX_MINUS\n"); switchStateToSubscript();}

    | expr BINARY_PLUS expr {printf("P: expr +\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Sum, $1, $3);}
    | PREFIX_PLUS expr {printf("P: expr +\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PrefixPlus, $2);}
    | expr POSTFIX_PLUS {printf("P: expr +\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PostfixPlus, $1);}
    
    | expr BINARY_DIV expr {printf("P: expr BINARY_DIV\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Div, $1, $3);}
    | PREFIX_DIV expr {printf("P: expr PREFIX_DIV\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PrefixDiv, $2);}
    | expr POSTFIX_DIV {printf("P: expr POSTFIX_DIV\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PostfixDiv, $1);}

    | expr BINARY_MUL expr {printf("P: expr BINARY_MUL\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Mul, $1, $3);}
    | PREFIX_MUL expr {printf("P: expr PREFIX_MUL\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PrefixMul, $2);}
    | expr POSTFIX_MUL {printf("P: expr POSTFIX_MUL\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PostfixMul, $1);}

    | expr BINARY_MOD expr {printf("P: expr BINARY_MOD\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Modulus, $1, $3);}
    | PREFIX_MOD expr {printf("P: expr PREFIX_MOD\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PrefixMod, $2);}
    | expr POSTFIX_MOD {printf("P: expr POSTFIX_MOD\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PostfixMod, $1);}

    | expr '<' expr {printf("P: expr <\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::LT, $1, $3);}
    | expr '>' expr {printf("P: expr >\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::GT, $1, $3);}
    | expr OP_GTE expr {printf("P: expr >=\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::GTE, $1, $3);}
    | expr OP_LTE expr {printf("P: expr <=\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::LTE, $1, $3);}
    | expr OP_EQ expr {printf("P: expr ==\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::EQ, $1, $3);}
    | expr OP_NEQ expr {printf("P: expr !=\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::NEQ, $1, $3);}
    | expr '&' expr {printf("P: expr &\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::BitAnd, $1, $3);}
    | expr '|' expr {printf("P: expr |\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::BitOr, $1, $3);}
    | expr '^' expr {printf("P: expr ^\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::BitXor, $1, $3);}

    | expr BINARY_LOG_AND expr {printf("P: expr BINARY_LOG_AND\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::LogAnd, $1, $3);}
    | PREFIX_LOG_AND expr {printf("P: expr PREFIX_LOG_AND\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PrefixLogAnd, $2);}
    | expr POSTFIX_LOG_AND {printf("P: expr POSTFIX_LOG_AND\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PostfixLogAnd, $1);}

    | expr BINARY_LOG_OR expr {printf("P: expr BINARY_LOG_OR\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::LogOr, $1, $3);}
    | PREFIX_LOG_OR expr {printf("P: expr PREFIX_LOG_OR\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PrefixLogOr, $2);}
    | expr POSTFIX_LOG_OR {printf("P: expr POSTFIX_LOG_OR\n"); switchStateToSubscript(); $$ = ExprNode::createUnaryOp(ExprType::PrefixLogOr, $1);}

    | expr OP_LSHIFT expr {printf("P: expr <<\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Lshift, $1, $3);}
    | expr OP_RSHIFT expr {printf("P: expr >>\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Rshift, $1, $3);}
    | expr OP_CLOSED_RANGE expr {printf("P: expr ...\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::ClosedRange, $1, $3);}
    | expr OP_HALF_OPEN_RANGE expr {printf("P: expr ..<\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::HalfOpenRange, $1, $3);}
    | expr OP_NIL_COALESCE expr {printf("P: expr ??\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::NilCoalesce, $1, $3);}
    | expr IS type {printf("P: expr is\n"); switchStateToSubscript(); $$ = ExprNode::createTypeCheck($1, $3);}
    | expr AS type {printf("P: expr as\n"); switchStateToSubscript(); $$ = ExprNode::createTypeCast($1, $3);}
    | expr AS '?' type {printf("P: expr as ?\n"); switchStateToSubscript(); $$ = ExprNode::createTypeCastWithCheck($1, $4);}
    | expr AS '!' type {printf("P: expr as !\n"); switchStateToSubscript(); $$ = ExprNode::createTypeCastWithThrow($1, $4);}
    | expr '?' expr ':' expr {printf("P: expr ternary ? :\n"); switchStateToSubscript(); $$ = ExprNode::createTernary($1, $3, $5);}
    | '(' expr ')' {printf("P: expr brackets\n"); $$ = $2; switchStateToSubscript();}
    | funcCall {printf("P: expr funcCall\n"); switchStateToSubscript(); switchStateToSubscript(); $$ = ExprNode::createFuncCall($1);}
    | SUPER '.' funcCall {printf("P: expr super funcCall\n"); switchStateToSubscript(); $3->_scopeType = superCall; $$ = ExprNode::createFuncCall($3);}
    | expr '.' funcCall {printf("P: expr func access\n"); switchStateToSubscript(); $3->setAsExprAccess($1); $$ = ExprNode::createFuncCall($3);}
    | SELF '.' funcCall {printf("P: expr self func access\n"); switchStateToSubscript(); $3->_scopeType = selfCall; $$ = ExprNode::createFuncCall($3);}
    | expr '.' ID {printf("P: expr field access\n"); switchStateToSubscript(); $$ = ExprNode::createFieldAccessExpr($1, $3);}
    | SELF '.' ID {printf("P: expr self fieldAccess\n"); switchStateToSubscript(); $$ = ExprNode::createFieldAccessSelf($3);}
    | '[' exprList ']' {printf("P: expr array\n"); switchStateToSubscript(); $$ = ExprNode::createArray($2);}
    | expr SUBSCRIPT_SQUARE_BRACKET expr ']' {printf("P: expr array indexing\n"); switchStateToSubscript(); $$ = ExprNode::createBinaryOp(ExprType::Subscript, $1, $3);}
    ;

anyRoundBracket: '('
	| FUNC_CALL_ROUND_BRACKET
	;
        
%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      throw std::runtime_error(errormsg);
}