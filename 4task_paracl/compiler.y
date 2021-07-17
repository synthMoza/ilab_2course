%language "c++"
%skeleton "lalr1.cc"

%defines
%define api.value.type variant
%define parse.error custom
%param {yy::Driver* driver}

%locations

%code requires
{
    #include "Node.hpp"
    #include "Symtab.hpp"

    // Forward declaration of argument for the parser
    namespace yy { class Driver; }
}

%code
{
    #include "driver.hpp"

    using namespace se;

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
%nterm <se::BaseNode*>  block;          // to be used in construction like "if", "for"
%nterm <se::BaseNode*>  assign;         // assignment that can be interpreted as expression
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
                                            NameInfo* name_info = driver->lookup($1);
                                            VarInfo* var_info = nullptr;
                                            if (name_info == nullptr) {
                                                // Declare this variable
                                                var_info = new VarInfo;
                                                driver->insert(var_info, $1);
                                            } else
                                                var_info = static_cast<VarInfo*>(name_info);
                                            
                                            $$ = new VarNode($1, var_info);
                                        }
;

assign: lvalue ASSIGN lgc_or            {
                                            // Assign operator
                                            $$ = new BinOpNode(bin_op_type::ASSIGN, $1, $3);
                                        }
| lvalue ASSIGN QMARK                   {
                                            // Assign operator with input value
                                            UnOpNode* input = new UnOpNode(un_op_type::INPUT, nullptr);
                                            $$ = new BinOpNode(bin_op_type::ASSIGN, $1, input);
                                        }
;

line: SCOLON                            {
                                            // Create empty instruction
                                            $$ = nullptr; 
                                        }
| lgc_or SCOLON                         {
                                            $$ = $1;
                                        }
| assign SCOLON                         {
                                            $$ = $1;
                                        }
| KW_PRINT lgc_or SCOLON                {
                                            // Output operator
                                            $$ = new UnOpNode(un_op_type::OUTPUT, $2);
                                        }
| KW_IF LRBR lgc_or RRBR block          {
                                            // Conditional operator
                                            $$ = new IfNode($3, $5);
                                        }
| KW_WHILE LRBR lgc_or RRBR block       {
                                            // Loop operator
                                            $$ = new WhileNode($3, $5);
                                        }
;

block: scope                            {
                                            // Transfer control
                                            $$ = $1;
                                        }
| line                                  {
                                            $$ = $1;
                                        }
;

lgc_or: lgc_or LOR lgc_and              {
                                            $$ = new BinOpNode(bin_op_type::L_OR, $1, $3);
                                        }
| lgc_and                               {
                                            $$ = $1;
                                        }
;

lgc_and: lgc_and LAND pref              {
                                            $$ = new BinOpNode(bin_op_type::L_AND, $1, $3);
                                        }
| pref                                  {
                                            $$ = $1;
                                        }
;

pref: NOT lgc_equal                     {
                                            $$ = new UnOpNode(un_op_type::NOT, $2);
                                        }
| lgc_equal                             {
                                            $$ = $1;
                                        }
;

lgc_equal: lgc_equal EQUAL lgc_relate   {
                                            $$ = new BinOpNode(bin_op_type::EQUAL, $1, $3);
                                        }
| lgc_equal NEQUAL lgc_relate           {
                                            $$ = new BinOpNode(bin_op_type::NEQUAL, $1, $3);  
                                        }
| lgc_relate                            {
                                            $$ = $1;
                                        }
;

lgc_relate: lgc_relate GR expr0         {
                                            $$ = new BinOpNode(bin_op_type::GR, $1, $3);
                                        }
| lgc_relate GREQ expr0                 {
                                            $$ = new BinOpNode(bin_op_type::GREQ, $1, $3);
                                        }
| lgc_relate LS expr0                   {
                                            $$ = new BinOpNode(bin_op_type::LS, $1, $3);
                                        }
| lgc_relate LSEQ expr0                 {
                                            $$ = new BinOpNode(bin_op_type::LSEQ, $1, $3);
                                        }
| expr0                                 {
                                            $$ = $1;
                                        }
;

expr0: expr0 PLUS expr1                 {
                                            $$ = new BinOpNode(bin_op_type::PLUS, $1, $3);
                                        }
| expr0 MINUS expr1                     {
                                            $$ = new BinOpNode(bin_op_type::MINUS, $1, $3);
                                        }
| expr1                                 {
                                            $$ = $1;
                                        }
;

expr1: expr1 MUL expr2                  {
                                            $$ = new BinOpNode(bin_op_type::MUL, $1, $3);
                                        }
| expr1 DIV expr2                       {
                                            $$ = new BinOpNode(bin_op_type::DIV, $1, $3);
                                        }
| expr1 MOD expr2                       {
                                            $$ = new BinOpNode(bin_op_type::MOD, $1, $3);
                                        }
| expr2                                 {
                                            $$ = $1;
                                        }
;

expr2: PLUS expr3                       {
                                            $$ = new UnOpNode(un_op_type::U_PLUS, $2);
                                        }
| MINUS expr3                           {
                                            $$ = new UnOpNode(un_op_type::U_MINUS, $2);
                                        }
| expr3                                 {
                                            $$ = $1;
                                        }
;

expr3: NUMBER                           {
                                            $$ = new NumNode($1);
                                        }
| LRBR assign RRBR                      {
                                            // Declare variable inside expression
                                            $$ = $2;
                                        }
| NAME                                  {
                                            VarInfo* var_info = static_cast<VarInfo*>(driver->lookup($1));
                                            if (var_info == nullptr) {
                                                // Run-time error
                                                driver->report_parse_error(@1, ER_UNDEFINED_NAME);
                                                throw std::runtime_error("Compilation failed!");
                                            }

                                            $$ = new VarNode($1, var_info);
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

    void parser::error(const parser::location_type& loc, const std::string& message) {
        std::cerr << message << " in line " << loc.begin.line << std::endl;
    }

    void yy::parser::report_syntax_error(yy::parser::context const& ctx) const {
        driver->report_syntax_error(ctx);
    }
}