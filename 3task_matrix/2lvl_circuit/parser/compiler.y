%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::Driver* driver}

%code requires 
{
    #include <utility>
    #include "weight.h"

    // Forward declaration of argument for the parser
    namespace yy { class Driver; }
}

%code
{
    #include "driver.h"

    using namespace se;

    namespace yy {
        parser::token_type yylex(parser::semantic_type* yylval, Driver* driver);
    }
}

%token
    DDASH       "--"           // double dash
    SCOLON      ";"     
    VOLTAGE     "V"     
    PLUS        "+"     
    MINUS       "-"     
    COMMA       "," 
    ERR    
;

%token <int>    INT
%token <double> DOUBLE

%nterm <std::pair<std::pair<std::size_t, std::size_t>, se::Weight>>     line
%nterm <std::pair<std::size_t, std::size_t>>                            nodes

%start program

%%

program: instr                              {}
;

instr: line                                 {
                                                driver->add_pair($1);
                                            }
| instr line                                {
                                                driver->add_pair($2);
                                            }
;

line: nodes COMMA DOUBLE SCOLON             {
                                                $$ = std::pair<std::pair<std::size_t, std::size_t>, Weight>($1, Weight($3));
                                            }
| nodes COMMA DOUBLE SCOLON DOUBLE VOLTAGE  {
                                                $$ = std::pair<std::pair<std::size_t, std::size_t>, Weight>($1, Weight($3, $5));
                                            }
;

nodes: INT DDASH INT                        {
                                                $$ = std::pair<std::size_t, std::size_t>($1, $3);
                                            }
;

%%

namespace yy {

    parser::token_type yylex(parser::semantic_type* yylval, Driver* driver) {
        return driver->yylex(yylval);
    }

    void parser::error(const std::string& message) {
        std::cerr << "Failure: " << message << std::endl;
    }
}