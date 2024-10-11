%option noyywrap
%option never-interactive

%x COMMENT_MULTILINE
%x STRING_MULTILINE
%x STRING

FLOAT_HEX [-+]?0x[0-9a-fA-F][_0-9a-fA-F]*\.[0-9a-fA-F][_0-9a-fA-F]*([pP][-+]?[0-9a-fA-F][_0-9]*)|[0-9a-fA-F][_0-9a-fA-F]*p[+-]?[0-9][_0-9]*
FLOAT_DEC [0-9][_0-9]*e[+-]?[0-9][_0-9]*|[-+]?[0-9][_0-9]*\.[0-9][_0-9]*(e[-+]?[0-9][_0-9]*)?
INT_BINARY 0b[01][_01]*
INT_OCTAL 0o[0-7][_0-7]*
INT_DECIMAL [0-9][_0-9]*
INT_HEXADECIMAL 0x[0-9a-fA-F][_0-9a-fA-F]*
ANTI_DELIM [^	() \[\]{};\-=+<>&|~!\^%\*\/\.,:\?\n]

%{
#include <stdio.h>
#include <iostream>
#include <string>
#include "swift.tab.h"
using namespace std;

	// FUNCTIONS
void removeChar(std::string& str, char charToRemove);
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
	string stringOneLine;
	string unicodeHexBuffer;

	int intBuf = 0;
	float floatBuf = 0;
	double doubleBuf = 0;
%}

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
subscript {cout << "Keyword subscript: " << yytext << endl; return SUBCRIPT;}
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
do {cout << "Keyword do: " << yytext << endl;}
else {cout << "Keyword else: " << yytext << endl;}
fallthrough {cout << "Keyword fallthrough: " << yytext << endl;}
for {cout << "Keyword for: " << yytext << endl;}
guard {cout << "Keyword guard: " << yytext << endl;}
if {cout << "Keyword if: " << yytext << endl;}
in {cout << "Keyword in: " << yytext << endl;}
repeat {cout << "Keyword repeat: " << yytext << endl;}
return {cout << "Keyword return: " << yytext << endl;}
throw {cout << "Keyword throw: " << yytext << endl;}
switch {cout << "Keyword switch: " << yytext << endl;}
where {cout << "Keyword where: " << yytext << endl;}
while {cout << "Keyword while: " << yytext << endl;}

Any {cout << "Keyword any: " << yytext << endl;}
as {cout << "Keyword as: " << yytext << endl;}
false {cout << "Keyword false: " << yytext << endl;}
is {cout << "Keyword is: " << yytext << endl;}
nil {cout << "Keyword nil: " << yytext << endl;}
self {cout << "Keyword self: " << yytext << endl;}
Self {cout << "Keyword Self: " << yytext << endl;}
super {cout << "Keyword super: " << yytext << endl;}
throws {cout << "Keyword throws: " << yytext << endl;}
true {cout << "Keyword true: " << yytext << endl;}
try {cout << "Keyword try: " << yytext << endl;}
	
associativity {cout << "Keyword associativity: " << yytext << endl;}
convenience {cout << "Keyword convenience: " << yytext << endl;}
didSet {cout << "Keyword didSet: " << yytext << endl;}
dynamic {cout << "Keyword dynamic: " << yytext << endl;}
final {cout << "Keyword final: " << yytext << endl;}
get {cout << "Keyword get: " << yytext << endl;}
indirect {cout << "Keyword indirect: " << yytext << endl;}
infix {cout << "Keyword infix: " << yytext << endl;}
lazy {cout << "Keyword lazy: " << yytext << endl;}
left {cout << "Keyword left: " << yytext << endl;}
mutating {cout << "Keyword mutating: " << yytext << endl;}
none {cout << "Keyword none: " << yytext << endl;}
nonmutating {cout << "Keyword nonmutating: " << yytext << endl;}
optional {cout << "Keyword optional: " << yytext << endl;}
override {cout << "Keyword override: " << yytext << endl;}
postfix {cout << "Keyword postfix: " << yytext << endl;}
precedence {cout << "Keyword precedence: " << yytext << endl;}
prefix {cout << "Keyword prefix: " << yytext << endl;}
Protocol {cout << "Keyword protocol: " << yytext << endl;}
required {cout << "Keyword requierd: " << yytext << endl;}
right {cout << "Keyword right: " << yytext << endl;}
set {cout << "Keyword set: " << yytext << endl;}
some {cout << "Keyword some: " << yytext << endl;}
Type {cout << "Keyword Type: " << yytext << endl;}
unowned {cout << "Keyword unowned: " << yytext << endl;}
weak {cout << "Keyword weak: " << yytext << endl;}
willSet {cout << "Keyword willSet: " << yytext << endl;}
	
\#available {cout << "Keyword #available: " << yytext << endl;}
\#colorLiteral {cout << "Keyword #colorLiteral: " << yytext << endl;}
\#column {cout << "Keyword #column: " << yytext << endl;}
\#dsohandle {cout << "Keyword #dsohandle: " << yytext << endl;}
\#elseif {cout << "Keyword #elsif: " << yytext << endl;}
\#else {cout << "Keyword #import: " << yytext << endl;}
\#endif {cout << "Keyword #endif: " << yytext << endl;}
\#error {cout << "Keyword #error: " << yytext << endl;}
\#fileID {cout << "Keyword #fileID: " << yytext << endl;}
\#fileLiteral {cout << "Keyword #fileLiteral: " << yytext << endl;}
\#filePath {cout << "Keyword #filepath: " << yytext << endl;}
\#file {cout << "Keyword #file: " << yytext << endl;}
\#function {cout << "Keyword #function: " << yytext << endl;}
\#if {cout << "Keyword #if: " << yytext << endl;}
\#imageLiteral {cout << "Keyword #imageLiteral: " << yytext << endl;}
\#keyPath {cout << "Keyword #keypath: " << yytext << endl;}
\#line {cout << "Keyword #line: " << yytext << endl;}
\#selector {cout << "Keyword #selector: " << yytext << endl;}
\#sourceLocation {cout << "Keyword #sourceLocation: " << yytext << endl;}
\#warning {cout << "Keyword #warning: " << yytext << endl;}

\+ { cout << "Operator plus: " << yytext << endl; }
\- {cout << "Operator subtraction: " << yytext << endl;}
\/ {cout << "Operator division: " << yytext << endl;}
\* {cout << "Operator multiplication: " << yytext << endl;}
\% {cout << "Operator modulus: " << yytext << endl;}

\= { cout << "Operator assignment: " << yytext << endl; }
\+\= { cout << "Operator adition assignment: " << yytext << endl; }
\-\= { cout << "Operator subtraction assignment: " << yytext << endl; }
\/\= { cout << "Operator division assignment: " << yytext << endl; }
\*= { cout << "Operator multiplication assignment: " << yytext << endl; }
\%\= { cout << "Operator modulus assignment: " << yytext << endl; }
	
\<\< { cout << "Operator bitwise left shift: " << yytext << endl; }
\>\> { cout << "Operator bitwise right shift: " << yytext << endl; }
\& { cout << "Operator bitwise AND: " << yytext << endl; }
\| { cout << "Operator bitwise OR: " << yytext << endl; }
\^ { cout << "Operator bitwise XOR: " << yytext << endl; }
\~ { cout << "Operator bitwise NOT: " << yytext << endl; }
	 
\< { cout << "Operator Less than: " << yytext << endl; }
\> { cout << "Operator greater than: " << yytext << endl; }
\=\= { cout << "Operator equal to: " << yytext << endl; }
\<\= { cout << "Operator less than or equal to: " << yytext << endl; }
\>\= { cout << "Operator greater than or equal to: " << yytext << endl; }
\!\= { cout << "Operator not equal to: " << yytext << endl; }
		 
\&\& { cout << "Operator logical and: " << yytext << endl; }
\|\| { cout << "Operator logical or: " << yytext << endl; }
\! { cout << "Operator logical not: " << yytext << endl; }
	
\?\? { cout << "Operator nil coalescing: " << yytext << endl; }
\+\+ { cout << "Operator increment: " << yytext << endl; }
\-\- { cout << "Operator decerment: " << yytext << endl; }
	
\. { cout << "Operator dot(member access): " << yytext << endl; }
, { cout << "Operator comma: " << yytext << endl; } 
	
\.\.\. { cout << "Operator closed range: " << yytext << endl; }
\.\.\< { cout << "Operator half open range: " << yytext << endl; } 

\? { cout << "Operator ternary: " << yytext << endl;}
\: { cout << "Operator colon: " << yytext << endl; }
\-\> { cout << "Operator function return type: " << yytext << endl; }

	/* BRACKETS */

\( { cout << "Left round bracket: " << yytext << endl; }
\) { cout << "Right round bracket: " << yytext << endl; }
\[ { cout << "Left square bracket: " << yytext << endl; }
\] { cout << "Right square bracket: " << yytext << endl; }
\{ { cout << "Left curly bracket: " << yytext << endl; }
\} { cout << "Left right bracket: " << yytext << endl; }
	
	/* TYPES */
	
Bool { cout << "Type bool: " << yytext << endl; }
String { cout << "Type String: " << yytext << endl; }
Character { cout << "Type Character: " << yytext << endl; }
Int8 { cout << "Type Int8: " << yytext << endl; }
Int16 { cout << "Type Int16: " << yytext << endl; }
Int32 { cout << "Type Int32: " << yytext << endl; }
Int64 { cout << "Type Int64: " << yytext << endl; }
Int { cout << "Type Int: " << yytext << endl; }
UInt8 { cout << "Type UInt8: " << yytext << endl; }
UInt16 { cout << "Type UInt16: " << yytext << endl; }
UInt32 { cout << "Type UInt32: " << yytext << endl; }
UInt64 { cout << "Type UInt64: " << yytext << endl; }
UInt { cout << "Type UInt: " << yytext << endl; }
Float { cout << "Type Float: " << yytext << endl; }
Float80 { cout << "Type Float80: " << yytext << endl; }
Double { cout << "Type Double: " << yytext << endl; }

	/* COMMENTS */

\/\/.* { cout << "Inline comment: " << yytext << endl; }

	/* COMMENTS MULTILINE */
\/\* {str = yytext; multilineNumberBegin = lineNumber; BEGIN(COMMENT_MULTILINE); }
<COMMENT_MULTILINE>\*\/ { str += yytext; cout << "Multiline comment: \n" << str << "\n"; BEGIN(INITIAL); }
<COMMENT_MULTILINE>\n { str += yytext; lineNumber++; }
<COMMENT_MULTILINE>. { str += yytext;}
<COMMENT_MULTILINE><<EOF>> {cout << "ERROR: Multiline comment at line " << multilineNumberBegin << ": is not closed" << endl; BEGIN(INITIAL); }

	/* LINE COUNTER */
\n {lineNumber++;}

	/* STRINGS */

\"\"\" { stringMultiline.clear(); multilineStringLineNumber = lineNumber; BEGIN(STRING_MULTILINE); }
<STRING_MULTILINE>[^\n\r] { stringMultiline += yytext; }
	/* TODO MAYBE WE SHOULD IGNORE ALL TABS COMPLETELY */
<STRING_MULTILINE>\\\n\t* { stringMultiline += ""; }
<STRING_MULTILINE>\n { stringMultiline += yytext; lineNumber++;}
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
<STRING_MULTILINE>\\\n\s*\"\"\" { cout << "ERROR: Escaped newline at the last line of multiline string is not allowed at line " << multilineStringLineNumber << endl; BEGIN(INITIAL); }
<STRING_MULTILINE>\s*\"\"\" { 
	cout << "Multiline string: " << stringMultiline << endl; 
	BEGIN(INITIAL); 
	//This is a memory leak
	yylval.strval = _strdup(stringMultiline.c_str());
	return LITERAL_STRING;
}
<STRING_MULTILINE><<EOF>> { cout << "ERROR: Multiline string is not closed at line " << multilineStringLineNumber << endl; BEGIN(INITIAL); }

\" { stringOneLine.clear(); BEGIN(STRING); }
<STRING>[^\\\"\n]+ { stringOneLine += yytext; }
<STRING>\n { cout << "ERROR: unterminated string at line " << lineNumber << endl; BEGIN(INITIAL);}
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
<STRING>\\u { cout << "ERROR: incorrect unicode char format at line " << lineNumber << endl; BEGIN(INITIAL);}
<STRING>\" { 
	cout << "Oneline string: " << stringOneLine << endl; 
	BEGIN(INITIAL); 
	//This is a memory leak
	yylval.strval = _strdup(stringOneLine.c_str());
	return LITERAL_STRING;
}
<STRING><<EOF>> { cout << "ERROR: string in not closed at the end of file at line " << lineNumber << endl; BEGIN(INITIAL); }

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
	
[_a-zA-Z][_a-zA-Z0-9]* { cout << "Identifier: " << yytext << endl; }

	/* TODO: DELETE THIS WHEN EVERYTHING IS DONE*/
.|[^.] {}
%%


	/* FUNCTIONS */
void removeChar(string& str, char charToRemove) 
{
    str.erase(remove(str.begin(), str.end(), charToRemove), str.end());
}