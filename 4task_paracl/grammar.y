%language "c++"
%skeleton "lalr1.cc"

%defines
%define api.value.type variant
%param {yy::Driver* driver}

%locations

%code requires
{
    #include "Node.hpp"
    #include "Symtab.hpp"

    namespace se {
        // Nodes
        class BaseNode;
        class DeclNode;
        class NumNode;
        class BinOpNode;
        class UnOpNode;
        class ScopeNode;

        // Symbol table
        struct NameInfo;
        struct VarInfo;
    }

    // Forward declaration of argument for the parser
    namespace yy { class Driver; }
}

%code
{
    #include "driver.hpp"

    namespace se {
        // Nodes
        class BaseNode;
        class DeclNode;
        class NumNode;
        class BinOpNode;
        class UnOpNode;
        class ScopeNode;
        class WhileNode;

        // Symbol table
        struct NameInfo;
        struct VarInfo;
    }

    namespace yy {

        parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* l, Driver* driver);
    }
}

%token
    ASSIGN      "="
    
    MINUS       "-"
    PLUS        "+"
    MUL         "*"
    DIV         "/"
    MOD         "%"

    EQUAL       "=="
    NEQUAL      "!="
    GR          ">"
    GREQ        ">="
    LS          "<"
    LSEQ        "<="
    NOT         "!"

    LAND        "&&"    // logical and
    LOR         "||"    // logical or
    
    LRBR        "("     // left round bracket
    RRBR        ")"     // right round bracket
    LCBR        "{"     // left curly bracket
    RCBR        "}"     // right curly bracket

    QMARK       "?"     // question mark
    KW_PRINT    "print" // key-word
    KW_WHILE    "while"
    KW_IF       "if"

    SCOLON      ";"
    ERR
;

%token <int>            NUMBER
%token <std::string>    NAME

%nterm <se::BaseNode*>  expr3
%nterm <se::BaseNode*>  expr2
%nterm <se::BaseNode*>  expr1
%nterm <se::BaseNode*>  expr0
%nterm <se::BaseNode*>  lgc_relate;     // logical related
%nterm <se::BaseNode*>  lgc_equal;      // logical equal
%nterm <se::BaseNode*>  lgc_and;        // logical and
%nterm <se::BaseNode*>  lgc_or;         // logical or
%nterm <se::BaseNode*>  pref;           // prefix
%nterm <se::BaseNode*>  lvalue;
%nterm <se::BaseNode*>  line
%nterm <se::BaseNode*>  scope;
%nterm <se::BaseNode*>  op_scope;
%nterm <se::BaseNode*>  cl_scope;
%nterm <se::BaseNode*>  instr;          // instruction

%left '+' '-'
%left '*' '/'

%start program

%%

program: instr                          {
                                            driver->launch();
                                        }
;

instr: line                             {
                                            driver->addInstruction($1);
                                        }
| instr line                            {   
                                            driver->addInstruction($2);
                                        }
| instr scope                           {
                                            driver->addInstruction($2);
                                        }
;

scope: op_scope instr cl_scope          {

                                            $$ = $3;
                                        }
;

op_scope: LCBR                          {
                                            driver->cur_scope_ = new ScopeNode(driver->cur_scope_);
                                        }
;

cl_scope: RCBR                          {
                                            $$ = driver->cur_scope_;
                                            driver->cur_scope_ = driver->cur_scope_->getPrevScope();
                                        }
;

lvalue: NAME                            {
                                            se::NameInfo* name_info = driver->lookup($1);
                                            se::VarInfo* var_info = nullptr;
                                            if (name_info == nullptr) {
                                                // Declare this variable
                                                var_info = new se::VarInfo;
                                                driver->insert(var_info, $1);
                                            } else
                                                var_info = static_cast<se::VarInfo*>(name_info);
                                            
                                            $$ = new se::VarNode($1, var_info);
                                        }
;

line: lgc_or SCOLON                     {
                                            $$ = $1;
                                        }
| lvalue ASSIGN lgc_or SCOLON           {
                                            // Assign operator
                                            $$ = new se::BinOpNode(se::ASSIGN, $1, $3);
                                        }
| lvalue ASSIGN QMARK SCOLON            {
                                            // Assign operator with input value
                                            se::UnOpNode* input = new se::UnOpNode(se::INPUT, nullptr);
                                            $$ = new se::BinOpNode(se::ASSIGN, $1, input);
                                        }
| KW_PRINT lgc_or SCOLON                {
                                            // Output operator
                                            $$ = new se::UnOpNode(se::OUTPUT, $2);
                                        }
| KW_IF LRBR lgc_or RRBR scope          {
                                            // Conditional operator
                                            $$ = new se::IfNode($3, $5);
                                        }
| KW_WHILE LRBR lgc_or RRBR scope       {
                                            // Loop operator
                                            $$ = new se::WhileNode($3, $5);
                                        }
;

lgc_or: lgc_or LOR lgc_and              {
                                            $$ = new se::BinOpNode(se::L_OR, $1, $3);
                                        }
| lgc_and                               {
                                            $$ = $1;
                                        }
;

lgc_and: lgc_and LAND pref              {
                                            $$ = new se::BinOpNode(se::L_AND, $1, $3);
                                        }
| pref                                  {
                                            $$ = $1;
                                        }
;

pref: NOT lgc_equal                     {
                                            $$ = new se::UnOpNode(se::NOT, $2);
                                        }
| lgc_equal                             {
                                            $$ = $1;
                                        }
;

lgc_equal: lgc_equal EQUAL lgc_relate   {
                                            $$ = new se::BinOpNode(se::EQUAL, $1, $3);
                                        }
| lgc_equal NEQUAL lgc_relate           {
                                            $$ = new se::BinOpNode(se::NEQUAL, $1, $3);  
                                        }
| lgc_relate                            {
                                            $$ = $1;
                                        }
;

lgc_relate: lgc_relate GR expr0         {
                                            $$ = new se::BinOpNode(se::GR, $1, $3);
                                        }
| lgc_relate GREQ expr0                 {
                                            $$ = new se::BinOpNode(se::GREQ, $1, $3);
                                        }
| lgc_relate LS expr0                   {
                                            $$ = new se::BinOpNode(se::LS, $1, $3);
                                        }
| lgc_relate LSEQ expr0                 {
                                            $$ = new se::BinOpNode(se::LSEQ, $1, $3);
                                        }
| expr0                                 {
                                            $$ = $1;
                                        }
;

expr0: expr0 PLUS expr1                 {
                                            $$ = new se::BinOpNode(se::PLUS, $1, $3);
                                        }
| expr0 MINUS expr1                     {
                                            $$ = new se::BinOpNode(se::MINUS, $1, $3);
                                        }
| expr1                                 {
                                            $$ = $1;
                                        }
;

expr1: expr1 MUL expr2                  {
                                            $$ = new se::BinOpNode(se::MUL, $1, $3);
                                        }
| expr1 DIV expr2                       {
                                            $$ = new se::BinOpNode(se::DIV, $1, $3);
                                        }
| expr1 MOD expr2                       {
                                            $$ = new se::BinOpNode(se::MOD, $1, $3);
                                        }
| expr2                                 {
                                            $$ = $1;
                                        }
;

expr2: PLUS expr3                       {
                                            $$ = new se::UnOpNode(se::U_PLUS, $2);
                                        }
| MINUS expr3                           {
                                            $$ = new se::UnOpNode(se::U_MINUS, $2);
                                        }
| expr3                                 {
                                            $$ = $1;
                                        }
;

expr3: NUMBER                           {
                                            $$ = new se::NumNode($1);
                                        }
| NAME                                  {
                                            se::VarInfo* var_info = static_cast<se::VarInfo*>(driver->lookup($1));
                                            if (var_info == nullptr) {
                                                // Run-time error
                                                throw std::runtime_error("Variable " + $1 + " wasn't declared in this scope!");
                                            }

                                            $$ = new se::VarNode($1, var_info);
                                        }
| LRBR lgc_or RRBR                      {
                                            $$ = $2;
                                        }
;
%%

namespace yy {

    parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* l, Driver* driver) {
        return driver->yylex(l, yylval);
    }

    void parser::error(const parser::location_type& l, const std::string& message) {
        std::cout << message << ", line: " << l.begin.line << std::endl;
    }
}