%option noyywrap
%option never-interactive
%option yylineno

%x COMMENT_MULTILINE
%x STRING_MULTILINE
%x STRING
%s SUBSCRIPT

FLOAT_HEX [-+]?0x[0-9a-fA-F][_0-9a-fA-F]*\.[0-9a-fA-F][_0-9a-fA-F]*([pP][-+]?[0-9a-fA-F][_0-9]*)|[0-9a-fA-F][_0-9a-fA-F]*p[+-]?[0-9][_0-9]*
FLOAT_DEC [0-9][_0-9]*e[+-]?[0-9][_0-9]*|[-+]?[0-9][_0-9]*\.[0-9][_0-9]*(e[-+]?[0-9][_0-9]*)?
INT_BINARY 0b[01][_01]*
INT_OCTAL 0o[0-7][_0-7]*
INT_DECIMAL [0-9][_0-9]*
INT_HEXADECIMAL 0x[0-9a-fA-F][_0-9a-fA-F]*
ANTI_DELIM [^	() \[\]{};\-=+<>&|~!\^%\*\/\.,:\?\n]
WHITESPACES [ \n	]
BEFORE_BINARY [^;]
NOT_WHITESPACES [^ \n	]
AFTER_NO_WHITESPACE_BINARY [^ \n	;)]
OPERATORS [-+/*%!]|\&\&|\|\|
AFTER_PREFIX [^*/ \n	>=-]
BEFORE_PREFIX [(; \n	]
AFTER_POSTFIX [ \n	;)]

%{
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "swift.tab.h"
using namespace std;

	// FUNCTIONS
void removeChar(std::string& str, char charToRemove);
bool isSubsciptActive = false;
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

void switchStateToSubscript();
yytokentype getPrefixOperatorFromString(string str);
yytokentype getPostfixOperatorFromString(string str);
yytokentype getBinaryOperatorFromString(string str);

vector<yytokentype> tokenQueue;

%}

%%
	/* CODE SECTION */
%{
	// VARIABLES
	std::string str = "";
	int lineNumber = 1;  
	int multilineNumberBegin = 0;

	int multilineStringLineNumber = 1;
	string stringMultiline;
	int multilineStringStartTabCount = 0;
	string stringOneLine;
	string unicodeHexBuffer;

	int intBuf = 0;
	float floatBuf = 0;
	double doubleBuf = 0;

	if (tokenQueue.size() > 0)
	{
		yytokentype token = tokenQueue.back();
		tokenQueue.pop_back();
		cout << "Send element from queue" << endl;
		return token;
	}
%}

<SUBSCRIPT>\[ { cout << "Subscript left square bracket: " << yytext << endl; isSubsciptActive = false; return SUBSCRIPT_SQUARE_BRACKET; BEGIN(INITIAL);}
<SUBSCRIPT>\( { cout << "Subscript left round bracket: " << yytext << endl; isSubsciptActive = false; return FUNC_CALL_ROUND_BRACKET; BEGIN(INITIAL);}
<SUBSCRIPT>[^	 ] { cout << "Subscript end" << endl;  yyless(0); isSubsciptActive = false; BEGIN(INITIAL);}

	/* KEYWORDS */
	
associatedtype {cout << "Keyword associatedtype: " << yytext << endl; return ASSOCIATED_TYPE;} 
class {cout << "Keyword class: " << yytext << endl; return CLASS;}
deinit { cout << "Keyword deinit: " << yytext << endl; return DEINIT;}
enum { cout << "Keyword enum: " << yytext << endl; return ENUM;} 
extension {cout << "Keyword extension: " << yytext << endl; return EXTENSION;}
fileprivate {cout << "Keyword fileprivate: " <<yytext << endl; return FILEPRIVATE;}
import {cout << "Keyword import: " << yytext << endl; return IMPORT;}
init {cout << "Keyword init: " << yytext << endl; return INIT;}
inout {cout << "Keyword inout: " << yytext << endl; return INOUT;}
internal {cout << "Keyword internal: " << yytext << endl; return INTERNAL;}
let {cout << "Keyword let: " << yytext << endl; return LET;}
open {cout << "Keyword open: " << yytext << endl; return OPEN;}
operator {cout << "Keyword operator: " << yytext << endl; return OPERATOR;}
private {cout << "Keyword private: " << yytext << endl; return PRIVATE;}
precedencegroup {cout << "Keyword precedencegroup: " << yytext << endl; return PRECEDENCEGROUP;}
protocol {cout << "Keyword protocol: " << yytext << endl; return PROTOCOL;}
public {cout << "Keyword public: " << yytext << endl; return PUBLIC;}
rethrows {cout << "Keyword rethrows: " << yytext << endl; return RETHROWS;}
static {cout << "Keyword static: " << yytext << endl; return STATIC;}
struct {cout << "Keyword struct: " << yytext << endl; return STRUCT;}
subscript {cout << "Keyword subscript: " << yytext << endl; return SUBSCRIPT;}
typealias {cout << "Keyword typealias: " << yytext << endl; return TYPEALIAS;}
func {cout << "Keyword func: " << yytext << endl; return FUNC;}
var {cout << "Keyword var: " << yytext << endl; return VAR;}
	
	/* KEYWORDS FOR LOOPS */

break {cout << "Keyword break: " << yytext << endl; return BREAK;}
case {cout << "Keyword case: " << yytext << endl; return CASE;}
catch {cout << "Keyword catch: " << yytext << endl; return CATCH;}
continue {cout << "Keyword continue: " << yytext << endl; return CONTINUE;}
default {cout << "Keyword default: " << yytext << endl; return DEFAULT;}
defer {cout << "Keyword defer: " << yytext << endl; return DEFER;}
do {cout << "Keyword do: " << yytext << endl; return DO;}
else {cout << "Keyword else: " << yytext << endl; return ELSE;}
fallthrough {cout << "Keyword fallthrough: " << yytext << endl; return FALLTHROUGH;}
for {cout << "Keyword for: " << yytext << endl; return FOR;}
guard {cout << "Keyword guard: " << yytext << endl; return GUARD;}
if {cout << "Keyword if: " << yytext << endl; return IF;}
in {cout << "Keyword in: " << yytext << endl; return IN;}
repeat {cout << "Keyword repeat: " << yytext << endl; return REPEAT;}
return {cout << "Keyword return: " << yytext << endl; return RETURN;}
throw {cout << "Keyword throw: " << yytext << endl; return THROW;}
switch {cout << "Keyword switch: " << yytext << endl; return SWITCH;}
where {cout << "Keyword where: " << yytext << endl; return WHERE;}
while {cout << "Keyword while: " << yytext << endl; return WHILE;}

Any {cout << "Keyword any: " << yytext << endl; return ANY;}
as {cout << "Keyword as: " << yytext << endl; return AS;}
false {cout << "Keyword false: " << yytext << endl; return FALSE;}
is {cout << "Keyword is: " << yytext << endl; return IS;}
nil {cout << "Keyword nil: " << yytext << endl; return NIL;}
self {cout << "Keyword self: " << yytext << endl; return SELF;}
Self {cout << "Keyword Self: " << yytext << endl; return SELF_TYPE;}
super {cout << "Keyword super: " << yytext << endl; return SUPER;}
throws {cout << "Keyword throws: " << yytext << endl; return THROWS;}
true {cout << "Keyword true: " << yytext << endl; return TRUE;}
try {cout << "Keyword try: " << yytext << endl; return TRY;}
	
associativity {cout << "Keyword associativity: " << yytext << endl; return ASSOCIATIVITY;}
convenience {cout << "Keyword convenience: " << yytext << endl; return CONVENIENCE;}
didSet {cout << "Keyword didSet: " << yytext << endl; return DIDSET;}
dynamic {cout << "Keyword dynamic: " << yytext << endl; return DYNAMIC;}
final {cout << "Keyword final: " << yytext << endl; return FINAL;}
get {cout << "Keyword get: " << yytext << endl; return GET;}
indirect {cout << "Keyword indirect: " << yytext << endl; return INDIRECT;}
infix {cout << "Keyword infix: " << yytext << endl; return INFIX;}
lazy {cout << "Keyword lazy: " << yytext << endl; return LAZY;}
left {cout << "Keyword left: " << yytext << endl; return LEFT;}
mutating {cout << "Keyword mutating: " << yytext << endl; return MUTATING;}
none {cout << "Keyword none: " << yytext << endl; return NONE;}
nonmutating {cout << "Keyword nonmutating: " << yytext << endl; return NONMUTATING;}
optional {cout << "Keyword optional: " << yytext << endl; return OPTIONAL;}
override {cout << "Keyword override: " << yytext << endl; return OVERRIDE;}
postfix {cout << "Keyword postfix: " << yytext << endl; return POSTFIX;}
precedence {cout << "Keyword precedence: " << yytext << endl; return PRECEDENCE;}
prefix {cout << "Keyword prefix: " << yytext << endl; return PREFIX;}
Protocol {cout << "Keyword protocol: " << yytext << endl; return PROTOCOL_UPPERCASE;}
required {cout << "Keyword required: " << yytext << endl; return REQUIRED;}
right {cout << "Keyword right: " << yytext << endl; return RIGHT;}
set {cout << "Keyword set: " << yytext << endl; return SET;}
some {cout << "Keyword some: " << yytext << endl; return SOME;}
Type {cout << "Keyword Type: " << yytext << endl; return TYPE;}
unowned {cout << "Keyword unowned: " << yytext << endl; return UNOWNED;}
weak {cout << "Keyword weak: " << yytext << endl; return WEAK;}
willSet {cout << "Keyword willSet: " << yytext << endl; return WILLSET;}
	
\#available {cout << "Keyword #available: " << yytext << endl; return HASH_AVAILABLE;}
\#colorLiteral {cout << "Keyword #colorLiteral: " << yytext << endl; return HASH_COLORLITERAL;}
\#column {cout << "Keyword #column: " << yytext << endl; return HASH_COLUMN;}
\#dsohandle {cout << "Keyword #dsohandle: " << yytext << endl; return HASH_DSOHANDLE;}
\#elseif {cout << "Keyword #elseif: " << yytext << endl; return HASH_ELSEIF;}
\#else {cout << "Keyword #else: " << yytext << endl; return HASH_ELSE;}
\#endif {cout << "Keyword #endif: " << yytext << endl; return HASH_ENDIF;}
\#error {cout << "Keyword #error: " << yytext << endl; return HASH_ERROR;}
\#fileID {cout << "Keyword #fileID: " << yytext << endl; return HASH_FILEID;}
\#fileLiteral {cout << "Keyword #fileLiteral: " << yytext << endl; return HASH_FILELITERAL;}
\#filePath {cout << "Keyword #filePath: " << yytext << endl; return HASH_FILEPATH;}
\#file {cout << "Keyword #file: " << yytext << endl; return HASH_FILE;}
\#function {cout << "Keyword #function: " << yytext << endl; return HASH_FUNCTION;}
\#if {cout << "Keyword #if: " << yytext << endl; return HASH_IF;}
\#imageLiteral {cout << "Keyword #imageLiteral: " << yytext << endl; return HASH_IMAGELITERAL;}
\#keyPath {cout << "Keyword #keyPath: " << yytext << endl; return HASH_KEYPATH;}
\#line {cout << "Keyword #line: " << yytext << endl; return HASH_LINE;}
\#selector {cout << "Keyword #selector: " << yytext << endl; return HASH_SELECTOR;}
\#sourceLocation {cout << "Keyword #sourceLocation: " << yytext << endl; return HASH_SOURCELOCATION;}
\#warning {cout << "Keyword #warning: " << yytext << endl; return HASH_WARNING;}

\= {cout << "Operator assignment: " << yytext << endl; return '=';}
\+\= {cout << "Operator addition assignment: " << yytext << endl; return OP_PLUS_ASSIGN;}
\-\= {cout << "Operator subtraction assignment: " << yytext << endl; return OP_MINUS_ASSIGN;}
\/\= {cout << "Operator division assignment: " << yytext << endl; return OP_DIV_ASSIGN;}
\*\= {cout << "Operator multiplication assignment: " << yytext << endl; return OP_MUL_ASSIGN;}
\%\= {cout << "Operator modulus assignment: " << yytext << endl; return OP_MOD_ASSIGN;}
	
\<\< {cout << "Operator bitwise left shift: " << yytext << endl; return OP_LSHIFT;}
\>\> {cout << "Operator bitwise right shift: " << yytext << endl; return OP_RSHIFT;}
\& {cout << "Operator bitwise AND: " << yytext << endl; return '&';}
\| {cout << "Operator bitwise OR: " << yytext << endl; return '|';}
\^ {cout << "Operator bitwise XOR: " << yytext << endl; return '^';}
\~ {cout << "Operator bitwise NOT: " << yytext << endl; return '~';}
	 
\< {cout << "Operator less than: " << yytext << endl; return '<';}
\> {cout << "Operator greater than: " << yytext << endl; return '>';}
\=\= {cout << "Operator equal to: " << yytext << endl; return OP_EQ;}
\<\= {cout << "Operator less than or equal to: " << yytext << endl; return OP_LTE;}
\>\= {cout << "Operator greater than or equal to: " << yytext << endl; return OP_GTE;}
\!\= {cout << "Operator not equal to: " << yytext << endl; return OP_NEQ;}
	
\?\? {cout << "Operator nil coalescing: " << yytext << endl; return OP_NIL_COALESCE;}
\+\+ {cout << "Operator increment: " << yytext << endl; return OP_INC;}
\-\- {cout << "Operator decrement: " << yytext << endl; return OP_DEC;}
	
\. {cout << "Operator dot (member access): " << yytext << endl; return '.';}
, {cout << "Operator comma: " << yytext << endl; return ',';} 
	
\.\.\. {cout << "Operator closed range: " << yytext << endl; return OP_CLOSED_RANGE;}
\.\.\< {cout << "Operator half open range: " << yytext << endl; return OP_HALF_OPEN_RANGE;} 

\? {cout << "Operator ternary: " << yytext << endl; return '?';}
\: {cout << "Operator colon: " << yytext << endl; return ':';}
\-\> {cout << "Operator function return type: " << yytext << endl; return OP_FUNC_RETURN;}



	/* BRACKETS */

\( {cout << "Left round bracket: " << yytext << endl; return '(';}
\) {cout << "Right round bracket: " << yytext << endl; return ')';}
\[ {cout << "Left square bracket: " << yytext << endl; return '[';}
\] {cout << "Right square bracket: " << yytext << endl; return ']';}
\{ {cout << "Left curly bracket: " << yytext << endl; return '{';}
\} {cout << "Right curly bracket: " << yytext << endl; return '}';}
	
	/* TYPES */
	
Bool {cout << "Type bool: " << yytext << endl; return TYPE_BOOL;}
String {cout << "Type String: " << yytext << endl; return TYPE_STRING;}
Character {cout << "Type Character: " << yytext << endl; return TYPE_CHARACTER;}
Int8 {cout << "Type Int8: " << yytext << endl; return TYPE_INT8;}
Int16 {cout << "Type Int16: " << yytext << endl; return TYPE_INT16;}
Int32 {cout << "Type Int32: " << yytext << endl; return TYPE_INT32;}
Int64 {cout << "Type Int64: " << yytext << endl; return TYPE_INT64;}
Int {cout << "Type Int: " << yytext << endl; return TYPE_INT;}
UInt8 {cout << "Type UInt8: " << yytext << endl; return TYPE_UINT8;}
UInt16 {cout << "Type UInt16: " << yytext << endl; return TYPE_UINT16;}
UInt32 {cout << "Type UInt32: " << yytext << endl; return TYPE_UINT32;}
UInt64 {cout << "Type UInt64: " << yytext << endl; return TYPE_UINT64;}
UInt {cout << "Type UInt: " << yytext << endl; return TYPE_UINT;}
Float {cout << "Type Float: " << yytext << endl; return TYPE_FLOAT;}
Float80 {cout << "Type Float80: " << yytext << endl; return TYPE_FLOAT80;}
Double {cout << "Type Double: " << yytext << endl; return TYPE_DOUBLE;}

	/* COMMENTS */

<INITIAL,SUBSCRIPT>\/\/.* { cout << "Inline comment: " << yytext << endl; }

	/* COMMENTS MULTILINE */
\/\* {str = yytext; multilineNumberBegin = lineNumber; BEGIN(COMMENT_MULTILINE); }
<COMMENT_MULTILINE>\*\/ { str += yytext; cout << "Multiline comment: \n" << str << "\n"; isSubsciptActive ? BEGIN(SUBSCRIPT) : BEGIN(INITIAL); }
<COMMENT_MULTILINE>\n { str += yytext; lineNumber++;  isSubsciptActive = false;}
<COMMENT_MULTILINE>. { str += yytext;}
<COMMENT_MULTILINE><<EOF>> {cout << "ERROR: Multiline comment at line " << multilineNumberBegin << ": is not closed" << endl; isSubsciptActive = false; isSubsciptActive ? BEGIN(SUBSCRIPT) : BEGIN(INITIAL); }

	/* LINE COUNTER */
\n {lineNumber++;}

	/* STATEMENT_DELIMITER */
\; {cout << "Statement delimiter: " << yytext << endl; return ';';}

	/* STRINGS */

[\t]*\"\"\" { 
	stringMultiline.clear();  
	string str = string(yytext); 
	multilineStringStartTabCount = count(str.begin(), str.end(), '\t'); 
	multilineStringLineNumber = lineNumber; 
	BEGIN(STRING_MULTILINE); 
}
<STRING_MULTILINE>[^\n\r] { stringMultiline += yytext;}
	/* TODO MAYBE WE SHOULD IGNORE ALL TABS COMPLETELY */
<STRING_MULTILINE>\\\n\t* { stringMultiline += ""; }
<STRING_MULTILINE>\n { stringMultiline += yytext; lineNumber++; }
<STRING_MULTILINE>\\\\ { stringMultiline += "\\"; }
<STRING_MULTILINE>\\n { stringMultiline += "\n"; }
<STRING_MULTILINE>\\b { stringMultiline += "\b"; }
<STRING_MULTILINE>\\0 { stringMultiline += "\0"; }
<STRING_MULTILINE>\\f { stringMultiline += "\f"; }
<STRING_MULTILINE>\\r { stringMultiline += "\r"; }
<STRING_MULTILINE>\\t { stringMultiline += "\t"; }
<STRING_MULTILINE>\\v { stringMultiline += "\v"; }
<STRING_MULTILINE>\\\' { stringMultiline += "\'"; }
<STRING_MULTILINE>\\\" { stringMultiline += "\""; }
<STRING_MULTILINE>\\u\{0*([0-9A-Fa-f]){1,8}\} {
	//Remove \u{ from yytext
	unicodeHexBuffer = (yytext + 3);
	//Remove } from yytext
	unicodeHexBuffer.pop_back();
	stringMultiline += char(strtoll(unicodeHexBuffer.c_str(), NULL, 16));
 }
<STRING_MULTILINE>\"{1,2} { stringMultiline += yytext; }
	/* TODO THIS MAY NOT WORK ALL THE TIME */
<STRING_MULTILINE>\\\n\s*\"\"\" { cout << "ERROR: Escaped newline at the last line of multiline string is not allowed at line " << multilineStringLineNumber << endl; isSubsciptActive = false; BEGIN(INITIAL); }
<STRING_MULTILINE>[\t]*\"\"\" {
	string str = string(yytext); 
	int tabCount = count(str.begin(), str.end(), '\t');

	if (tabCount != multilineStringStartTabCount)
	{
		cout << "ERROR: Multiline string tabulation is not the same at line " << multilineStringLineNumber << endl; 
	}

	//Remove extra tabs
	string temp = stringMultiline;
	stringMultiline.clear();
	int tabsToRemove = multilineStringStartTabCount;
	for (auto& letter : temp)
	{
		if (letter == '\t' && tabsToRemove > 0)
		{
			tabsToRemove -= 1;
			continue;
		}
		else if (letter == '\n')
		{
			tabsToRemove = multilineStringStartTabCount;
		}
		else
		{
			tabsToRemove = 0;
		}
		stringMultiline += letter;
	}

	cout << "Multiline string: " << stringMultiline << endl; 
	isSubsciptActive ? BEGIN(SUBSCRIPT) : BEGIN(INITIAL); 
	//This is a memory leak
	yylval.strval = _strdup(stringMultiline.c_str());
	return LITERAL_STRING;
}
<STRING_MULTILINE><<EOF>> { cout << "ERROR: Multiline string is not closed at line " << multilineStringLineNumber << endl; isSubsciptActive ? BEGIN(SUBSCRIPT) : BEGIN(INITIAL); }

\" { stringOneLine.clear(); BEGIN(STRING); }
<STRING>[^\\\"\n]+ { stringOneLine += yytext; }
<STRING>\n { cout << "ERROR: unterminated string at line " << lineNumber << endl; isSubsciptActive ? BEGIN(SUBSCRIPT) : BEGIN(INITIAL);}
<STRING>\\\\ { stringOneLine += "\\"; }
<STRING>\\n { stringOneLine += "\n"; }
<STRING>\\b { stringOneLine += "\b"; }
<STRING>\\0 { stringOneLine += "\0"; }
<STRING>\\f { stringOneLine += "\f"; }
<STRING>\\r { stringOneLine += "\r"; }
<STRING>\\t { stringOneLine += "\t"; }
<STRING>\\v { stringOneLine += "\v"; }
<STRING>\\\' { stringOneLine += "\'"; }
<STRING>\\\" { stringOneLine += "\""; }
<STRING>\\u\{0*([0-9A-Fa-f]){1,8}\} {
	//Remove \u{ from yytext
	unicodeHexBuffer = (yytext + 3);
	//Remove } from yytext
	unicodeHexBuffer.pop_back();
	stringOneLine += char(strtoll(unicodeHexBuffer.c_str(), NULL, 16));
 }
<STRING>\\u { cout << "ERROR: incorrect unicode char format at line " << lineNumber << endl; isSubsciptActive ? BEGIN(SUBSCRIPT) : BEGIN(INITIAL);}
<STRING>\" { 
	cout << "Oneline string: " << stringOneLine << endl; 
	isSubsciptActive ? BEGIN(SUBSCRIPT) : BEGIN(INITIAL);
	//This is a memory leak
	yylval.strval = _strdup(stringOneLine.c_str());
	return LITERAL_STRING;
}
<STRING><<EOF>> { cout << "ERROR: string in not closed at the end of file at line " << lineNumber << endl; isSubsciptActive ? BEGIN(SUBSCRIPT) : BEGIN(INITIAL); }

	/* NUMBERS BINARY */

{INT_BINARY} {
	str = yytext; removeChar(str, '_'); 
	intBuf = stoi(str.substr(2), nullptr, 2);
	cout << "Binary integer: " << intBuf << endl;
	yylval.ival = intBuf;
	return LITERAL_INT;
}

	/* TODO EXPERIMENTAL */

{INT_BINARY}{ANTI_DELIM} {cout << "ERROR: Invalid character \"" <<  (yytext + strlen(yytext) - 1) << "\" in binary integer at line " << lineNumber << endl;}
0b[^01] {cout << "ERROR: Invalid binary integer at line " << lineNumber << endl;}

	/* NUMBERS OCTAL */

{INT_OCTAL} {
	str = yytext; removeChar(str, '_'); 
	intBuf = stoi(str.substr(2), nullptr, 8);
	cout << "Octal integer: " << intBuf << endl;
	yylval.ival = intBuf;
	return LITERAL_INT;
}

	/* TODO EXPERIMENTAL */

{INT_OCTAL}{ANTI_DELIM} {cout << "ERROR: Invalid character \"" <<  (yytext + strlen(yytext) - 1) << "\" in octal integer at line " << lineNumber << endl;}
0o[^0-7] {cout << "ERROR: Invalid octal integer at line " << lineNumber << endl;}

	/* NUMBERS DECIMAL */

{INT_DECIMAL} {
	str = yytext; removeChar(str, '_'); 
	intBuf = stoi(str);
	cout << "Decimal integer: " << intBuf << endl;
	yylval.ival = intBuf;
	return LITERAL_INT;
}

	/* TODO EXPERIMENTAL */

{INT_DECIMAL}{ANTI_DELIM} {cout << "ERROR: Invalid character \"" <<  (yytext + strlen(yytext) - 1) << "\" in decimal integer at line " << lineNumber << endl;}

{FLOAT_DEC} { 
	str = yytext;
	removeChar(str, '_');
	doubleBuf = stod(str);
    cout << "Decimal floatingpoint number: " << doubleBuf << endl;
	yylval.dval = doubleBuf;
	return LITERAL_FLOAT;
}

	/* TODO EXPERIMENTAL */

{FLOAT_DEC}{ANTI_DELIM} {cout << "ERROR: Invalid character \"" <<  (yytext + strlen(yytext) - 1) << "\" in decimal floatingpoint at line " << lineNumber << endl;}

	/* NUMBERS HEX */

{INT_HEXADECIMAL} {
	str = yytext; removeChar(str, '_'); 
	intBuf = stoi(str.substr(2), nullptr, 16);
	cout << "Hexadecimal integer: " << intBuf << endl;
	yylval.ival = intBuf;
	return LITERAL_INT;
}

	/* TODO EXPERIMENTAL */

{INT_HEXADECIMAL}{ANTI_DELIM} {cout << "ERROR: Invalid character \"" <<  (yytext + strlen(yytext) - 1) << "\" in hexadecimal integer at line " << lineNumber << endl;}
0x[^0-9a-fA-F] {cout << "ERROR: Invalid hexadecimal integer at line " << lineNumber << endl;}

{FLOAT_HEX} {
	str = yytext;
	removeChar(str, '_');
	doubleBuf = stod(str);
	cout << "Hexadecimal floatingpoint number: " << doubleBuf << endl;
	yylval.dval = doubleBuf;
	return LITERAL_FLOAT;
}

	/* TODO EXPERIMENTAL */

[-+]?0x[0-9a-fA-F][_0-9a-fA-F]*\.[0-9a-fA-F][_0-9a-fA-F]* {cout << "ERROR: Hexadecimal floatingpoint number at line " << lineNumber << " must end with an exponent: " << yytext << endl;}

[-+]?0x[0-9a-fA-F][_0-9a-fA-F]*\.[0-9a-fA-F][_0-9a-fA-F]*p {cout << "ERROR: Hexadecimal floatingpoint number at line" << lineNumber << " exponent must end with a number: " << yytext << endl;}

{FLOAT_HEX}{ANTI_DELIM} {
	cout << "ERROR: Invalid character \"" <<  (yytext + strlen(yytext) - 1) << "\" in hexadecimal floatingpoint at line " << lineNumber << endl;
}
	
_ { cout << "Underscore" << endl; return '_';}

[_a-zA-Z][_a-zA-Z0-9]* { 
	cout << "Identifier: " << yytext << endl; 
	yylval.strval = _strdup(yytext);
	switchStateToSubscript();
	return ID;
}

	/* BINARY */
{WHITESPACES}{OPERATORS}/{WHITESPACES} {
	cout << "Binary whitespaces Operator: " << yytext << endl;
	str = yytext;
	return getBinaryOperatorFromString(str);

}

	/* PREFIX UNNARY */
{BEFORE_PREFIX}{OPERATORS}/{AFTER_PREFIX} {
	cout << "Prefix operator: " << yytext << endl;
	str = yytext;
	if (str.find('(') != string::npos) {
		cout << "Added operator to queue, ";
		tokenQueue.push_back(getPrefixOperatorFromString(str));
		if (isSubsciptActive) {
			BEGIN(INITIAL);
			cout << "send function call round bracket" << endl;
			return FUNC_CALL_ROUND_BRACKET;
		} else {
			cout << "send (" << endl;
			return '(';
		}
	} else if (str.find(';') != string::npos) {
		cout << "Added operator to queue, ";
		tokenQueue.push_back(getPrefixOperatorFromString(str));
		cout << "send ;" << endl;
		return ';';
	};

	return getPrefixOperatorFromString(str);
}

	/* PREFIX UNNARY */
^{OPERATORS}/{AFTER_PREFIX} {
	cout << "Prefix newline operator: " << yytext << endl; 
	str = yytext;
	return getPrefixOperatorFromString(str);
}	


	/* POSTFIX UNNARY */
{OPERATORS}/{AFTER_POSTFIX} {
	cout << "Postfix operator: " << yytext << endl;
	str = yytext;
	if (str.find(')') != string::npos) {
		cout << "Added operator to queue, ";
		tokenQueue.push_back(getPrefixOperatorFromString(str));
		return ')';
	} else if (str.find(';') != string::npos) {
		cout << "Added operator to queue, ";
		tokenQueue.push_back(getPrefixOperatorFromString(str));
		cout << "send ;" << endl;
		return ';';
	};
	return getPostfixOperatorFromString(str);
}

	/* BINARY */
{OPERATORS}/{AFTER_NO_WHITESPACE_BINARY} {
	cout << "Binary no whitespaces Operator: " << yytext << endl; 
	str = yytext;
	return getBinaryOperatorFromString(str);
}

	/* TODO: DELETE THIS WHEN EVERYTHING IS DONE*/
.|[^.] {}
%%


	/* FUNCTIONS */
void removeChar(string& str, char charToRemove) 
{
    str.erase(remove(str.begin(), str.end(), charToRemove), str.end());
}

void switchStateToSubscript(){
	cout << "Switched to subscript state" << endl;
	isSubsciptActive = true;
	BEGIN(SUBSCRIPT);
}

yytokentype getPrefixOperatorFromString(string str)
{	// +-/*%&&||!

	if (str.find('-') != string::npos) {
		return PREFIX_MINUS;
	} else if (str.find('+') != string::npos) {
		return PREFIX_PLUS;
	} else if (str.find('/') != string::npos) {
		return PREFIX_DIV;
	} else if (str.find('*') != string::npos) {
		return PREFIX_MUL;
	} else if (str.find('%') != string::npos) {
		return PREFIX_MOD;
	} else if (str.find("&&") != string::npos) {
		return PREFIX_LOG_AND;
	} else if (str.find("||") != string::npos) {
		return PREFIX_LOG_OR;
	} else if (str.find('!') != string::npos) {
		return PREFIX_NOT;
	}
}

yytokentype getPostfixOperatorFromString(string str)
{
	if (str.find('-') != string::npos) {
		return POSTFIX_MINUS;
	} else if (str.find('+') != string::npos) {
		return POSTFIX_PLUS;
	} else if (str.find('/') != string::npos) {
		return POSTFIX_DIV;
	} else if (str.find('*') != string::npos) {
		return POSTFIX_MUL;
	} else if (str.find('%') != string::npos) {
		return POSTFIX_MOD;
	} else if (str.find("&&") != string::npos) {
		return POSTFIX_LOG_AND;
	} else if (str.find("||") != string::npos) {
		return POSTFIX_LOG_OR;
	} else if (str.find('!') != string::npos) {
		return POSTFIX_NOT;
	}
}

yytokentype getBinaryOperatorFromString(string str)
{
	if (str.find('-') != string::npos) {
		return BINARY_MINUS;
	}  else if (str.find('+') != string::npos) {
		return BINARY_PLUS;
	} else if (str.find('/') != string::npos) {
		return BINARY_DIV;
	} else if (str.find('*') != string::npos) {
		return BINARY_MUL;
	} else if (str.find('%') != string::npos) {
		return BINARY_MOD;
	} else if (str.find("&&") != string::npos) {
		return BINARY_LOG_AND;
	} else if (str.find("||") != string::npos) {
		return BINARY_LOG_OR;
	} else if (str.find('!') != string::npos) {
		return BINARY_NOT;
	}
}