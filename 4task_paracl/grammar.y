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
        struct VarNode;
        struct InputNode;
        struct OutputNode;
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
        struct VarNode;
        struct InputNode;
        struct OutputNode;
    }

    namespace yy {

        parser::token_type yylex(parser::semantic_type* yylval, Driver* driver);
    }
}

%token
    EQUAL       "="
    MINUS       "-"
    PLUS        "+"
    MUL         "*"
    DIV         "/"
    SCOLON      ";"
    LRBR        "("     // left round bracket
    RRBR        ")"     // right round bracket
    LCBR        "{"     // left curly bracket
    RCBR        "}"     // right curly bracket
    QMARK       "?"     // question mark
    KW_PRINT    "print" // key-word
    ERR
;

%token <int>            NUMBER
%token <std::string>    NAME

%nterm <se::BaseNode*>  line
%nterm <se::BaseNode*>  expr3
%nterm <se::BaseNode*>  expr2
%nterm <se::BaseNode*>  expr1
%nterm <se::BaseNode*>  expr0
%nterm <se::INode*>     instr; // instruction

%left '+' '-'
%left '*' '/'

%start program

%%

program: instr                  {}
;

instr: line SCOLON instr        { 
                                    $$ = new se::INode($1, driver->getSymtab());
                                    driver->addInstr($$);
                                }
| line SCOLON                   { 
                                    $$ = new se::INode($1, driver->getSymtab());
                                    driver->addInstr($$);
                                }
| LCBR instr RCBR instr         {
                                    // Open new scope
                                    driver->openScope();

                                    $$ = $2;

                                    // Close this scope
                                    driver->closeScope();
                                }
| LCBR instr RCBR               {
                                    // Open new scope
                                    driver->openScope();

                                    $$ = $2;

                                    // Close this scope
                                    driver->closeScope();
                                }
;

line: expr0                     {
                                    $$ = $1;
                                }
| NAME EQUAL expr0              {
                                    // Assign operation
                                    $$ = new se::OpNode(se::OpNode::ASSIGN);
                                    $$->addChild(new se::VarNode($1));
                                    $$->addChild($3);
                                }
| NAME EQUAL QMARK              {
                                    // Input operator
                                    $$ = new se::OpNode(se::OpNode::ASSIGN);
                                    $$->addChild(new se::VarNode($1));
                                    $$->addChild(new se::InputNode{});
                                }
| KW_PRINT expr0                {
                                    $$ = new se::OutputNode{};
                                    $$->addChild($2);
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
| NAME                          {
                                    $$ = new se::VarNode($1);
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

    void parser::error(const std::string&) {
        std::cout << "ERROR!" << std::endl;
    }
}