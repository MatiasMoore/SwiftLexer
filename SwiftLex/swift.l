%option noyywrap
%option never-interactive

%x COMMENT_MULTILINE
%x STRING_MULTILINE

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

	string stringMultiline;
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

	/* COMMENTS MULTILINE */
\/\* {str = yytext; multilineNumberBegin = lineNumber; BEGIN(COMMENT_MULTILINE); }
<COMMENT_MULTILINE>\*\/ { str += yytext; cout << "COMMENT_MULTILINE\n" << str << "\n"; BEGIN(INITIAL); }
<COMMENT_MULTILINE>\n { str += yytext; lineNumber++; }
<COMMENT_MULTILINE>. { str += yytext;}
<COMMENT_MULTILINE><<EOF>> {cout << "ERROR: COMMENT_MULTILINE in line " << multilineNumberBegin << ": comment are not closed"; BEGIN(INITIAL); }

	/* LINE COUNTER */
\n {lineNumber++;}

	/* STRINGS */

\"\"\" { stringMultiline.clear(); BEGIN(STRING_MULTILINE); }
<STRING_MULTILINE>[^\n\r] { stringMultiline += yytext; }
	/* TODO MAYBE WE SHOULD IGNORE ALL TABS COMPLETELY */
<STRING_MULTILINE>\\\n\t* { stringMultiline += ""; }
<STRING_MULTILINE>\n { stringMultiline += yytext; }
<STRING_MULTILINE>\\\\ { stringMultiline += "\\"; }
<STRING_MULTILINE>\\\" { stringMultiline += "\""; }
<STRING_MULTILINE>\\\' { stringMultiline += "\'"; }
<STRING_MULTILINE>\"{1,2} { stringMultiline += yytext; }
	/* TODO THIS MAY NOT WORK ALL THE TIME */
<STRING_MULTILINE>\\\n\s*\"\"\" { cout << "ERROR: Escaped newline at the last line is not allowed" << endl; BEGIN(INITIAL); }
<STRING_MULTILINE>\s*\"\"\" { cout << "Found multiline string: " << stringMultiline << endl; BEGIN(INITIAL); }
<STRING_MULTILINE><<EOF>> { cout << "ERROR: Multiline string is not closed" << endl; BEGIN(INITIAL); }

	/* NUMBERS BINARY */

0b[01]+[_01]* {str = yytext; removeChar(str, '_'); cout << "NUMBER_INTEGER_BINARY: " << stoi(str.substr(2), nullptr, 2) << endl;}

	/* NUMBERS OCTAL */

0o[0-7]+[_0-7]* {str = yytext; removeChar(str, '_'); cout << "NUMBER_INTEGER_OCTAL: " << stoi(str.substr(2), nullptr, 8) << endl;}

	/* NUMBERS DECIMAL */

[0-9][_0-9]* {str = yytext; removeChar(str, '_'); cout << "NUMBER_INTEGER_DECIMAL: " << stoi(str) << endl;}

	/* NUMBERS HEX */

0x[0-9a-fA-F]+[_0-9a-fA-F]* {str = yytext; removeChar(str, '_'); cout << "NUMBER_INTEGER_HEX: " << stoi(str.substr(2), nullptr, 16) << endl;}
	
	/* TODO: DELETE THIS WHEN EVERYTHING IS DONE*/
.|[^.] {}
%%

	/* FUNCTIONS */
void removeChar(string& str, char charToRemove) 
{
    str.erase(remove(str.begin(), str.end(), charToRemove), str.end());
}