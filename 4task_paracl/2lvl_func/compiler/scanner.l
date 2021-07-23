%option c++
%option yylineno
%option noyywrap nounput noinput batch debug

%option yyclass="yy::Scanner"

%{
    #include "compiler.hh"
    #include "Scanner.hpp"

    #define YY_USER_ACTION updateLocation();
%}

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
SYMB    [a-zA-Z_]
SYMBD   [a-zA-Z0-9_]

%%

{WS}                
"-"                 return yy::parser::token_type::MINUS;
"+"                 return yy::parser::token_type::PLUS;
"*"                 return yy::parser::token_type::MUL;
"/"                 return yy::parser::token_type::DIV;
"%"                 return yy::parser::token_type::MOD;

"=="                return yy::parser::token_type::EQUAL;
"!="                return yy::parser::token_type::NEQUAL;
">"                 return yy::parser::token_type::GR;
">="                return yy::parser::token_type::GREQ;
"<"                 return yy::parser::token_type::LS;
"<="                return yy::parser::token_type::LSEQ;
"!"                 return yy::parser::token_type::NOT;

"&&"                return yy::parser::token_type::LAND;
"||"                return yy::parser::token_type::LOR;

"("                 return yy::parser::token_type::LRBR;
")"                 return yy::parser::token_type::RRBR;
"{"                 return yy::parser::token_type::LCBR;
"}"                 return yy::parser::token_type::RCBR;

"?"                 return yy::parser::token_type::QMARK;
"print"             return yy::parser::token_type::KW_PRINT;
"while"             return yy::parser::token_type::KW_WHILE;
"if"                return yy::parser::token_type::KW_IF;

"func"              return yy::parser::token_type::KW_FUNC;
"return"            return yy::parser::token_type::KW_RET;

"="                 return yy::parser::token_type::ASSIGN;
";"                 return yy::parser::token_type::SCOLON;
","                 return yy::parser::token_type::COMMA;

{SYMB}{SYMBD}*      return yy::parser::token_type::NAME;
{DIGIT1}{DIGIT}*    return yy::parser::token_type::NUMBER;
{DIGIT}             return yy::parser::token_type::NUMBER;

.                   return yy::parser::token_type::ERR;

%%