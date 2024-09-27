%option noyywrap
%option never-interactive

%{
#include <stdio.h>
#include <iostream>
using namespace std;
%}

%%

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

	/* TODO: DELETE THIS WHEN EVERYTHING IS DONE*/
.|[^.] {}
%%