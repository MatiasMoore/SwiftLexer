%option noyywrap
%option never-interactive

%x MULTILINE_COMMENT
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
	/* 
deinit
enum 
	*/

	/* 
extension
fileprivate
import
init
inout
internal
let
open
operator
private
precedencegroup
protocol
public
rethrows
static
struct
subscript
typealias
func
var 
	*/

	/* KEYWORDS FOR LOOPS */

	/* 
break
case
catch
continue
default
defer
do
else
fallthrough
for
guard
if
in
repeat
return
throw
switch
where
while 
	*/

	/* 
Any
as
false
is
nil
self
Self
super
throws
true
try 
	*/

	/* 
associativity
convenience
didSet
dynamic
final
get
indirect
infix
lazy
left
mutating
none
nonmutating
optional
override
postfix
precedence
prefix
Protocol
required
right
set
some
Type
unowned
weak
willSet 
	*/

	/* KEYWORDS WITH HASHTAG */

\#available {cout << "Keyword #available: " << yytext << endl;}
	/* 
\#colorLiteral
\#column
\#dsohandle
\#elseif
\#else
\#endif
\#error
\#fileID
\#fileLiteral
\#filePath
\#file
\#function
\#if
\#imageLiteral
\#keyPath
\#line
\#selector
\#sourceLocation
\#warning 
	*/

	/* OPERATORS */

\+ { cout << "Operator plus: " << yytext << endl; }
	/* 
\-
\/
\*
\% 
	*/

\= { cout << "Operator assignment: " << yytext << endl; }
\+\= { cout << "Operator plus assignment: " << yytext << endl; }
	/* 
\-\=
\/\=
\*=
\%\= 
	*/

	/* 
\<\<
\>\>
\&
\|
\^
\~ 
	*/

	/* 
\<
\>
\=\=
\<\=
\>\=
\!\= 
	*/


	/* 
\&\&
\|\|
\! 
	*/


\?\? { cout << "Operator nil coalescing: " << yytext << endl; }
	/* 
\+\+
\-\- 
	*/


	/* 
\.
, 
	*/
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
	/* 
String
Character
Int8
Int16
Int32
Int64
Int
UInt8
UInt16
UInt32
UInt64
UInt
Float
Float80
Double
	*/

	/* COMMENTS */

\/\/.* { cout << "Found inline comment: " << yytext << endl; }

	/* MULTILINE COMMENTS */
\/\* {str = yytext; multilineNumberBegin = lineNumber; BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>\*\/ { str += yytext; std::cout << "MULTILINE_COMMENT\n" << str << "\n"; BEGIN(INITIAL); }
<MULTILINE_COMMENT>\n { str += yytext; lineNumber++; }
<MULTILINE_COMMENT>. { str += yytext;}
<MULTILINE_COMMENT><<EOF>> {std::cout << "MULTILINE_COMMENT ERROR in line " << multilineNumberBegin << ": comment are not closed"; BEGIN(ERROR); }

	/* LINE COUNTER */
\n {lineNumber++;}

	/* STRINGS */

\"\"\" { stringMultiline.clear(); multilineStringLineNumber = lineNumber; BEGIN(STRING_MULTILINE); }
<STRING_MULTILINE>[^\n\r] { stringMultiline += yytext; }
	/* TODO MAYBE WE SHOULD IGNORE ALL TABS COMPLETELY */
<STRING_MULTILINE>\\\n\t* { stringMultiline += ""; }
<STRING_MULTILINE>\n { stringMultiline += yytext; lineNumber++;}
<STRING_MULTILINE>\\\\ { stringMultiline += "\\"; }
<STRING_MULTILINE>\\n  { stringMultiline += "\n"; }
<STRING_MULTILINE>\\b { stringMultiline += "\b"; }
<STRING_MULTILINE>\\0  { stringMultiline += "\0"; }
<STRING_MULTILINE>\\f { stringMultiline += "\f"; }
<STRING_MULTILINE>\\r  { stringMultiline += "\r"; }
<STRING_MULTILINE>\\t  { stringMultiline += "\t"; }
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
<STRING>\\n  { stringOneLine += "\n"; }
<STRING>\\b { stringOneLine += "\b"; }
<STRING>\\0  { stringOneLine += "\0"; }
<STRING>\\f { stringOneLine += "\f"; }
<STRING>\\r  { stringOneLine += "\r"; }
<STRING>\\t  { stringOneLine += "\t"; }
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

	/* TODO: DELETE THIS WHEN EVERYTHING IS DONE*/
.|[^.] {}
<ERROR>.|[^.] {}
%%