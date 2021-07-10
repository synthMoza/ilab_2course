%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires
{
    #include <algorithm>
    #include <string>
    #include <vector>

    #include "Node.hpp"
    #include "INode.hpp"

    namespace se {
        struct INode;
        struct BaseNode;
        struct OpNode;
        struct NumNode;
    }

    // Forward declaration of argument for the parser
    namespace yy { class Driver; }
}

%code
{
    #include "driver.hpp"

    namespace se {
        struct INode;
        struct BaseNode;
        struct OpNode;
        struct NumNode;
    }

    namespace yy {

        parser::token_type yylex(parser::semantic_type* yylval, Driver* driver);
    }
}

%token
    EQUAL   "="
    MINUS   "-"
    PLUS    "+"
    MUL     "*"
    DIV     "/"
    SCOLON  ";"
    LRBR    "("     // left round bracket
    RRBR    ")"     // right round bracket
    ERR
;

%token <int> NUMBER
%nterm <se::BaseNode*> expr3
%nterm <se::BaseNode*> expr2
%nterm <se::BaseNode*> expr1
%nterm <se::BaseNode*> expr0
%nterm <se::INode*> instr; // instruction

%left '+' '-'
%left '*' '/'

%start program

%%

program: instr                  {}
;

instr: expr0 SCOLON instr       { 
                                    $$ = new se::INode{$1};
                                    driver->addInstr($$);
                                }
| expr0 SCOLON                  { 
                                    $$ = new se::INode{$1};
                                    driver->addInstr($$);
                                }
;

expr0: expr0 PLUS expr1         {
                                    $$ = new se::OpNode(se::OpNode::PLUS);
                                    $$->addChild($1); $$->addChild($3);
                                }
| expr0 MINUS expr1             {
                                    $$ = new se::OpNode(se::OpNode::MINUS);
                                    $$->addChild($1); $$->addChild($3);
                                }
| expr1                         {
                                    $$ = $1;
                                }
;

expr1: expr1 MUL expr2          {
                                    $$ = new se::OpNode(se::OpNode::MUL);
                                    $$->addChild($1); $$->addChild($3);
                                }
| expr1 DIV expr2               {
                                    $$ = new se::OpNode(se::OpNode::DIV);
                                    $$->addChild($1); $$->addChild($3);
                                }
| expr2                         {
                                    $$ = $1;
                                }
;

expr2: PLUS expr3               {
                                    $$ = new se::OpNode(se::OpNode::U_PLUS);
                                    $$->addChild($2);
                                }
| MINUS expr3                   {
                                    $$ = new se::OpNode(se::OpNode::U_MINUS);
                                    $$->addChild($2);
                                }
| expr3                         {
                                    $$ = $1;
                                }
;

expr3: NUMBER                   { 
                                    $$ = new se::NumNode($1);
                                }
| LRBR expr0 RRBR               {
                                    $$ = $2;
                                }
;
%%

namespace yy {

    parser::token_type yylex(parser::semantic_type* yylval, Driver* driver) {
        return driver->yylex(yylval);
    }

    void parser::error(const std::string&) {}
}