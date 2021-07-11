%option c++

%{

    #include "grammar.tab.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
SYMB    [a-zA-Z_]
SYMBD   [a-zA-Z0-9_]

%%

{WS}                // skip all blanks and tabs
"+"                 return yy::parser::token_type::PLUS;
"-"                 return yy::parser::token_type::MINUS;
"*"                 return yy::parser::token_type::MUL;
"/"                 return yy::parser::token_type::DIV;

"("                 return yy::parser::token_type::LRBR;
")"                 return yy::parser::token_type::RRBR;
"{"                 return yy::parser::token_type::LCBR;
"}"                 return yy::parser::token_type::RCBR;

"?"                 return yy::parser::token_type::QMARK;
"print"             return yy::parser::token_type::KW_PRINT;

"="                 return yy::parser::token_type::EQUAL;
";"                 return yy::parser::token_type::SCOLON;

{SYMB}{SYMBD}*      return yy::parser::token_type::NAME;
{DIGIT1}{DIGIT1}*   return yy::parser::token_type::NUMBER;

.                   return yy::parser::token_type::ERR;

%%