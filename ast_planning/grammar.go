// Code snips are placed as example for a production

Program := Statement_List $
Statement_List := Statement Statement_List
Statement_List := epsilon
Statement := Term
Statement := Declaration

Declaration := Variable_Declaration ?
Declaration := Function_Declaration
Declaration := Type_Declaration ?

auto func(int a, string &d, vector<string> a, string g = "hello") -> int { int a; return a; }
Function_Declaration := Type Identifier OpenParen Parameter_Head CloseParen Trailing_Return Body
Function_Declaration := Type Identifier OpenParen Parameter_Head CloseParen Trailing_Return SemiColon

int a, string &d, vector<string> a, string g = "hello"
Parameter_Head := epsilon
Parameter_Head := Parameter Parameter_List
Parameter_List := epsilon
Parameter_List := Comma Parameter Parameter_Tail
Parameter := Type Identifier
Parameter := Type Identifier Equal Identifier

-> int
Trailing_Return := epsilon
Trailing_Return := Arrow Type

{ int a; return a; }
Body := epsilon
Body := OpenBrace Statement_List CloseBrace

functionName("thing", 1234)
Function_Call := Identifier OpenParen Argument_Head CloseParen

"thing", 1234
Argument_Head := epsilon
Argument_Head := Argument Argument_List
Argument_List := epsilon
Argument_List := Comma Argument Argument_Tail
Argument := Term
Argument := Array

("hello") + (thing) - 23 * *yes
Term := OpenParen Term CloseParen
Term := Term Op Term
Term := Identifier
Term := Function_Call
Term := UnaryOp Term

Identifier := identifier
Identifier := number
Identifier := stringLiteral
Identifier := characterLiteral

["thing", [1234, 3, []]]
Array := OpenBracket Argument_Head CloseBracket

"####### Symbols ########"
"////////////////////////"
{
  OpenParen := '('
  OpenBrace := '{'
  OpenBracket := '['
  CloseParen := ')'
  CloseBrace := ']'
  CloseBracket := '}'

  UnaryOp :=
    Asterisk := *
    Ampersand := &
    Plus := +
    Minus := -

  Op :=
    Asterisk := *
    ForwardSlash := /
    Percent := %
    Plus := +
    Minus := -
    LessThan := <
    DoubleLessThan := <<
    GreaterThan := >
    DoubleGreaterThan := >>
    LessThanOrEqual := <=
    GreaterThanOrEqual := >=
    DoubleEqual := ==
    NotEqual := !=
    Ampersand := &
    Upwards := ^
    Pipe := |
    DoubleAmpersand := &&
    DoublePipe := ||
    Arrow := ->
}
