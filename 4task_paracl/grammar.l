%option c++

%{

    #include "grammar.tab.hh"

%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]

%%

{WS}                // skip all blanks and tabs
"+"                 return yy::parser::token_type::PLUS;
"-"                 return yy::parser::token_type::MINUS;
"*"                 return yy::parser::token_type::MUL;
"/"                 return yy::parser::token_type::DIV;
"("                 return yy::parser::token_type::LRBR;
")"                 return yy::parser::token_type::RRBR;
"="                 return yy::parser::token_type::EQUAL;
";"                 return yy::parser::token_type::SCOLON;
{DIGIT1}{DIGIT1}*   return yy::parser::token_type::NUMBER;
.                   return yy::parser::token_type::ERR;

%%