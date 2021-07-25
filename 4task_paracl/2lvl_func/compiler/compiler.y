%language "c++"
%skeleton "lalr1.cc"

%defines
%define api.value.type variant
%define parse.error custom
%param {yy::Driver* driver}

%locations

%code requires
{
    #include <vector>
    #include <string>
    #include <memory>

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

    LAND        "&&"        // logical and
    LOR         "||"        // logical or
    
    LRBR        "("         // left round bracket
    RRBR        ")"         // right round bracket
    LCBR        "{"         // left curly bracket
    RCBR        "}"         // right curly bracket

    QMARK       "?"         // question mark
    KW_PRINT    "print"     // key-word
    KW_WHILE    "while"
    KW_IF       "if"
    KW_FUNC     "func"      // function
    KW_RET      "return"    // return from function

    SCOLON      ";"
    COLON       ":"
    COMMA       ","         // function arguments
    ERR
;

%token <int>                        NUMBER
%token <std::string>                NAME

%nterm <se::BaseNode*>              expr3
%nterm <se::BaseNode*>              expr2
%nterm <se::BaseNode*>              expr1
%nterm <se::BaseNode*>              expr0
%nterm <se::BaseNode*>              lgc_relate;     // logical related
%nterm <se::BaseNode*>              lgc_equal;      // logical equal
%nterm <se::BaseNode*>              lgc_and;        // logical and
%nterm <se::BaseNode*>              lgc_or;         // logical or
%nterm <se::BaseNode*>              pref;           // prefix
%nterm <se::BaseNode*>              block;          // to be used in construction like "if", "for"
%nterm <se::BaseNode*>              line
%nterm <se::BaseNode*>              scope;
%nterm <se::BaseNode*>              op_scope;
%nterm <se::BaseNode*>              cl_scope;
%nterm <se::BaseNode*>              instr;          // instruction

%nterm <std::vector<se::BaseNode*>> args_val;       // arguments by value
%nterm <std::vector<std::string>>   args_br;
%nterm <std::vector<std::string>>   args;           // collect args for the function
%nterm <std::shared_ptr<se::FuncInfo>>              func_decl;      // function declaration
%nterm <std::shared_ptr<se::FuncInfo>>              func_decl_name;
%nterm <std::shared_ptr<se::FuncInfo>>              named_func;

%nterm <se::BaseNode*>              assign;         // assignment that can be interpreted as expression
%nterm <se::BaseNode*>              assign_1;       // divide for assigning inside expressions (without scolon)
%nterm <se::BaseNode*>              assign_2;

%nterm <se::BaseNode*>              body;           // function body (scope)
%nterm <se::BaseNode*>              op_body;
%nterm <se::BaseNode*>              cl_body;


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
                                            driver->setScope(new ScopeNode(driver->getScope()));
                                        }
;

cl_scope: RCBR                          {
                                            $$ = driver->getScope();
                                            driver->setScope(driver->getScope()->getPrevScope());
                                        }
;

assign_1: NAME ASSIGN lgc_or            {
                                            // Assign operator
                                            auto name_info = driver->lookup($1);
                                            std::shared_ptr<VarInfo> var_info = nullptr;

                                            if (name_info == nullptr) {
                                                // Declare this variable
                                                var_info = std::shared_ptr<VarInfo>(new VarInfo);
                                                driver->insert(var_info, $1);
                                            } else if (name_info->type_ != NameType::VAR) {
                                                // Redeclare as a variable
                                                driver->erase_name($1);
                                                var_info = std::shared_ptr<VarInfo>(new VarInfo);
                                                driver->insert(var_info, $1);
                                            } else
                                                var_info = std::static_pointer_cast<VarInfo>(driver->lookup($1));

                                            VarNode* var_node = new VarNode($1, var_info);
                                            $$ = new BinOpNode(bin_op_type::ASSIGN, var_node, $3);
                                        }
| NAME ASSIGN QMARK                   {
                                            // Assign operator with input value
                                            auto name_info = driver->lookup($1);

                                            std::shared_ptr<VarInfo> var_info = nullptr;
                                            if (name_info == nullptr) {
                                                // Declare this variable
                                                var_info = std::shared_ptr<VarInfo>(new VarInfo);
                                                driver->insert(var_info, $1);
                                                name_info = driver->lookup($1);
                                            } else if (name_info->type_ != NameType::VAR) {
                                                // Redeclare as a variable
                                                driver->erase_name($1);
                                                var_info = std::shared_ptr<VarInfo>(new VarInfo);
                                                driver->insert(var_info, $1);
                                                name_info = driver->lookup($1);
                                            } else
                                                var_info = std::static_pointer_cast<VarInfo>(driver->lookup($1));
                                            
                                            VarNode* var_node = new VarNode($1, var_info);
                                            UnOpNode* input = new UnOpNode(un_op_type::INPUT, nullptr);
                                            $$ = new BinOpNode(bin_op_type::ASSIGN, var_node, input);
                                        }
;

assign_2: NAME ASSIGN scope             {
                                            // Assign to scope
                                            auto name_info = driver->lookup($1);

                                            std::shared_ptr<VarInfo> var_info = nullptr;
                                            if (name_info == nullptr) {
                                                // Declare this variable
                                                var_info = std::shared_ptr<VarInfo>(new VarInfo);
                                                driver->insert(var_info, $1);
                                                name_info = driver->lookup($1);
                                            } else if (name_info->type_ != NameType::VAR) {
                                                // Redeclare as a variable
                                                driver->erase_name($1);
                                                var_info = std::shared_ptr<VarInfo>(new VarInfo);
                                                driver->insert(var_info, $1);
                                                name_info = driver->lookup($1);
                                            } else
                                                var_info = std::static_pointer_cast<VarInfo>(driver->lookup($1));
                                            
                                            VarNode* var_node = new VarNode($1, var_info);
                                            $$ = new BinOpNode(bin_op_type::ASSIGN, var_node, $3);
                                        }
| NAME ASSIGN func_decl               {
                                            // Check if this name already exists
                                            auto name_info = driver->lookup($1);
                                            if (name_info != nullptr) {
                                                // Redeclare this object as this function
                                                driver->erase_name($1);
                                            }

                                            // Insert function declaration into the driver
                                            driver->insert($3, $1);
                                        }
| NAME ASSIGN func_decl_name            {
                                            // Check if local name already exists
                                            auto name_info = driver->lookup($1);
                                            if (name_info != nullptr) {
                                                // Redeclare this object as this function
                                                driver->erase_name($1);
                                            }

                                            // Insert local name
                                            driver->insert($3, $1);
                                        }
;

assign: assign_1 SCOLON                 {
                                            $$ = $1;
                                        }
| assign_2                              {
                                            $$ = $1;
                                        }
;

body: LCBR instr RCBR                   {}
;

op_body:                                {
                                            driver->setTempScope(new ScopeNode(nullptr));
                                        }
;

cl_body:                                {
                                            $$ = driver->getScope();
                                            driver->resetScope();
                                        }
;

func_decl: KW_FUNC op_body args_br body cl_body                     {
                                                                        // Create function info structure
                                                                        $$ = std::shared_ptr<FuncInfo>(new FuncInfo($5, std::move($3)));
                                                                    }
;

func_decl_name: KW_FUNC named_func body cl_body                     {   
                                                                        // Create function info structure
                                                                        $2->setBody($4);
                                                                        $$ = $2;                                        
                                                                    }
;

named_func: op_body args_br COLON NAME  {
                                            // Check global name
                                            auto name_info = driver->lookup($4);
                                            if (name_info != nullptr) {
                                                // Run-time error, function with this name already exists
                                                driver->report_parse_error(@4, ER_TAKEN_NAME);
                                                throw std::runtime_error("Compilation failed!");
                                            }

                                            // Declare this name in the global scope
                                            auto func_info = std::shared_ptr<FuncInfo>(new FuncInfo);
                                            func_info->setArgs(std::move($2));

                                            driver->insertGlobal(func_info, $4);
                                            $$ = func_info;
                                        }
;

args_br: LRBR args RRBR                 {   
                                            std::shared_ptr<VarInfo> var_info = nullptr;
                                            for (auto&& arg : $2) {
                                                // Declare variables in function scope
                                                var_info = std::shared_ptr<VarInfo>(new VarInfo);
                                                driver->insert(var_info, arg);
                                            }

                                            $$ = $2;                                     
                                        }
;

args_val: lgc_or                        {
                                            $$.push_back($1);
                                        }
| lgc_or COMMA args_val                 {
                                            $$.push_back($1);
                                            $$.insert($$.end(), $3.begin(), $3.end());
                                        }
;

args: NAME                              {
                                            $$.push_back($1);
                                        }
| NAME COMMA args                       {
                                            $$.push_back($1);
                                            $$.insert($$.end(), $3.begin(), $3.end());
                                        }
;

line: SCOLON                            {
                                            // Create empty instruction
                                            $$ = nullptr; 
                                        }
| assign                                {
                                            $$ = $1;
                                        }
| lgc_or SCOLON                         {
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
| KW_RET lgc_or SCOLON                  {
                                            if (!driver->mayReturn()) {
                                                // Run-time error, return can be used only in functions
                                                driver->report_parse_error(@1, ER_WRONG_RET);
                                                throw std::runtime_error("Compilation failed!");
                                            }

                                            $$ = new RetNode($2);
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
| LRBR assign_1 RRBR                    {
                                            // Declare variable inside expression
                                            $$ = $2;
                                        }
| LRBR assign_2 RRBR                    {
                                            // Declare variable inside expression
                                            $$ = $2;
                                        }
| NAME                                  {
                                            auto var_info = std::static_pointer_cast<VarInfo>(driver->lookup($1));
                                            if (var_info == nullptr) {
                                                // Run-time error
                                                driver->report_parse_error(@1, ER_UNDEFINED_NAME);
                                                throw std::runtime_error("Compilation failed!");
                                            }
                                            
                                            $$ = new VarNode($1, var_info);
                                        }
| NAME LRBR args_val RRBR               {
                                            auto func_info = std::static_pointer_cast<FuncInfo>(driver->lookup($1));
                                            if (func_info == nullptr) {
                                                // Look for this function in global namespace
                                                func_info = std::static_pointer_cast<FuncInfo>(driver->lookupGlobal($1));
                                                
                                                if (func_info == nullptr) {
                                                    // Run-time error, can't resolve this name
                                                    driver->report_parse_error(@1, ER_UNDEFINED_NAME);
                                                    throw std::runtime_error("Compilation failed!");
                                                }
                                            }

        
                                            if (func_info->args_.size() != $3.size()) {
                                                // Run-time error, wrong argument number
                                                driver->report_parse_error(@3, ER_WRONG_ARGS);
                                                throw std::runtime_error("Compilation failed!");
                                            }
                                            
                                            // Create function node
                                            $$ = new FuncNode($1, func_info, $3);
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