%option c++
%option noyywrap nounput noinput batch debug

%{
    #include "compiler.hpp"
%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
SIGN    [+-]?
UINT    {DIGIT1}{DIGIT}*|{DIGIT}
INT     {SIGN}{UINT}
DOUBLE  {SIGN}{DIGIT}+.{DIGIT}+

%%

{WS}

"V"                 return yy::parser::token_type::VOLTAGE;
","                 return yy::parser::token_type::COMMA;
";"                 return yy::parser::token_type::SCOLON;
"--"                return yy::parser::token_type::DDASH;

{INT}               return yy::parser::token_type::INT;
{DOUBLE}            return yy::parser::token_type::DOUBLE;

.                   return yy::parser::token_type::ERR;

%%