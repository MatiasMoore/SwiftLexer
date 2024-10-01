%option noyywrap
%option never-interactive

%x COMMENT_MULTILINE
%x STRING_MULTILINE
%x STRING
%x ERROR

HEX_DIGIT [0-9A-Fa-f]

%{
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
%}

%%
	/* CODE SECTION */
%{
	// FUNCTIONS
	void removeChar(std::string& str, char charToRemove);

	// VARIABLES
	std::string str = "";
	int lineNumber = 1;  
	int multilineNumberBegin = 0;

	int multilineStringLineNumber = 1;
	string stringMultiline;
	string stringOneLine;
	string unicodeHexBuffer;
%}

	/* KEYWORDS */
	
associatedtype {cout << "Keyword associatedtype: " << yytext << endl;} 
class {cout << "Keyword class: " << yytext << endl;}
deinit { cout << "Keyword deinit: " << yytext << endl; }
enum { cout << "Keyword enum: " << yytext << endl; } 
extension {cout << "Keyword extension: " << yytext << endl;}
fileprivate {cout << "Keyword fileprivate: " <<yytext << endl;}
import {cout << "Keyword import: " << yytext << endl;}
init {cout << "Keyword init: " << yytext << endl;}
inout {cout << "Keyword inout: " << yytext << endl;}
internal {cout << "Keyword internal: " << yytext << endl;}
let {cout << "Keyword let: " << yytext << endl;}
open {cout << "Keyword open: " << yytext << endl;}
operator {cout << "Keyword operator: " << yytext << endl;}
private {cout << "Keyword private: " << yytext << endl;}
precedencegroup {cout << "Keyword precedencegroup: " << yytext << endl;}
protocol {cout << "Keyword protocol: " << yytext << endl;}
public {cout << "Keyword public: " << yytext << endl;}
rethrows {cout << "Keyword rethrows: " << yytext << endl;}
static {cout << "Keyword static: " << yytext << endl;}
struct {cout << "Keyword struct: " << yytext << endl;}
subscript {cout << "Keyword subscript: " << yytext << endl;}
typealias {cout << "Keyword typealias: " << yytext << endl;}
func {cout << "Keyword func: " << yytext << endl;}
var {cout << "Keyword var: " << yytext << endl;}
	
	/* KEYWORDS FOR LOOPS */

break {cout << "Keyword break: " << yytext << endl;}
case {cout << "Keyword case: " << yytext << endl;}
catch {cout << "Keyword catch: " << yytext << endl;}
continue {cout << "Keyword continue: " << yytext << endl;}
default {cout << "Keyword default: " << yytext << endl;}
defer {cout << "Keyword defer: " << yytext << endl;}
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
	
\<\< { cout << "Operator bitwise left shift : " << yytext << endl; }
\>\> { cout << "Operator bitwise right shift : " << yytext << endl; }
\& { cout << "Operator bitwise AND : " << yytext << endl; }
\| { cout << "Operator bitwise OR : " << yytext << endl; }
\^ { cout << "Operator bitwise XOR : " << yytext << endl; }
\~ { cout << "Operator bitwise NOT : " << yytext << endl; }
	 
\< { cout << "Operator Less than : " << yytext << endl; }
\> { cout << "Operator greater than : " << yytext << endl; }
\=\= { cout << "Operator equal to : " << yytext << endl; }
\<\= { cout << "Operator less than or equal to : " << yytext << endl; }
\>\= { cout << "Operator greater than or equal to : " << yytext << endl; }
\!\= { cout << "Operator not equal to : " << yytext << endl; }
		 
\&\& { cout << "Operator logical and : " << yytext << endl; }
\|\| { cout << "Operator logical or : " << yytext << endl; }
\! { cout << "Operator logical not : " << yytext << endl; }
	
\?\? { cout << "Operator nil coalescing: " << yytext << endl; }
\+\+ { cout << "Operator increment : " << yytext << endl; }
\-\- { cout << "Operator decerment : " << yytext << endl; }
	
\. { cout << "Operator dot(member access) : " << yytext << endl; }
, { cout << "Operator comma : " << yytext << endl; } 
	
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
<COMMENT_MULTILINE>\*\/ { str += yytext; cout << "COMMENT_MULTILINE\n" << str << "\n"; BEGIN(INITIAL); }
<COMMENT_MULTILINE>\n { str += yytext; lineNumber++; }
<COMMENT_MULTILINE>. { str += yytext;}
<COMMENT_MULTILINE><<EOF>> {cout << "ERROR: COMMENT_MULTILINE in line " << multilineNumberBegin << ": comment are not closed"; BEGIN(ERROR); }

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
<STRING_MULTILINE>\\u\{0*({HEX_DIGIT}){1,8}\} {
	//Remove \u{ from yytext
	unicodeHexBuffer = (yytext + 3);
	//Remove } from yytext
	unicodeHexBuffer.pop_back();
	stringMultiline += char(strtoll(unicodeHexBuffer.c_str(), NULL, 16));
 }
<STRING_MULTILINE>\"{1,2} { stringMultiline += yytext; }
	/* TODO THIS MAY NOT WORK ALL THE TIME */
<STRING_MULTILINE>\\\n\s*\"\"\" { cout << "ERROR: Escaped newline at the last line of multiline string is not allowed at line " << multilineStringLineNumber << endl; BEGIN(ERROR); }
<STRING_MULTILINE>\s*\"\"\" { cout << "Multiline string: " << stringMultiline << endl; BEGIN(INITIAL); }
<STRING_MULTILINE><<EOF>> { cout << "ERROR: Multiline string is not closed at line " << multilineStringLineNumber << endl; BEGIN(ERROR); }

\" { stringOneLine.clear(); BEGIN(STRING); }
<STRING>[^\\\"\n]+ { stringOneLine += yytext; }
<STRING>\n { cout << "ERROR: unterminated string in line " << lineNumber; BEGIN(ERROR);}
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
<STRING>\\u\{0*({HEX_DIGIT}){1,8}\} {
	//Remove \u{ from yytext
	unicodeHexBuffer = (yytext + 3);
	//Remove } from yytext
	unicodeHexBuffer.pop_back();
	stringOneLine += char(strtoll(unicodeHexBuffer.c_str(), NULL, 16));
 }
<STRING>\\u { cout << "ERROR: incorrect unicode char format at line " << lineNumber << endl; BEGIN(ERROR);}
<STRING>\" { std::cout << "Oneline string: " << stringOneLine << std::endl; BEGIN(INITIAL); }
<STRING><<EOF>> { cout << "ERROR: string in not closed at the end of file at line" << lineNumber << endl; BEGIN(ERROR); }

	/* NUMBERS BINARY */

0b[01][_01]* {str = yytext; removeChar(str, '_'); cout << "NUMBER_INTEGER_BINARY: " << stoi(str.substr(2), nullptr, 2) << endl;}

	/* NUMBERS OCTAL */

0o[0-7][_0-7]* {str = yytext; removeChar(str, '_'); cout << "NUMBER_INTEGER_OCTAL: " << stoi(str.substr(2), nullptr, 8) << endl;}

	/* NUMBERS DECIMAL */

[0-9][_0-9]* {str = yytext; removeChar(str, '_'); cout << "NUMBER_INTEGER_DECIMAL: " << stoi(str) << endl;}

[-+]?[0-9][_0-9]*\.[0-9][_0-9]*([eE][-+]?[0-9][_0-9]*)?|[0-9][_0-9]*[eE][+-]?[0-9][_0-9]* { 
	str = yytext;
	removeChar(str, '_');
    cout << "DECIMAL_FLOATINGPOINT_NUMBER: " << stod(str) << endl;
}

	/* NUMBERS HEX */

0x[0-9a-fA-F][_0-9a-fA-F]* {str = yytext; removeChar(str, '_'); cout << "NUMBER_INTEGER_HEX: " << stoi(str.substr(2), nullptr, 16) << endl;}

[-+]?0x[0-9a-fA-F][_0-9a-fA-F]*\.[0-9a-fA-F][_0-9a-fA-F]*([pP][-+]?[0-9a-fA-F][_0-9a-fA-F]*)?|[0-9a-fA-F][_0-9a-fA-F]*[pP][+-]?[0-9][_0-9]* {
	str = yytext;
	removeChar(str, '_');
	cout << "HEX_FLOATINGPOINT_NUMBER: " << stod(str) << endl;
}
	
[_a-zA-Z][_a-zA-Z0-9]* { printf("Found IDENTIFIER: %s\n", yytext); }

	/* TODO: DELETE THIS WHEN EVERYTHING IS DONE*/
.|[^.] {}
<ERROR>.|[^.] {}
%%

	/* FUNCTIONS */
void removeChar(string& str, char charToRemove) 
{
    str.erase(remove(str.begin(), str.end(), charToRemove), str.end());
}
