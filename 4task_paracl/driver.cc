#include "driver.hpp"

using namespace yy;

Driver::Driver(FlexLexer* plex) : global_symtab_ (new Symtab), cur_symtab_ {global_symtab_}, plex_ (plex) {}

parser::token_type Driver::yylex(parser::location_type* l, parser::semantic_type* yylval) {
    parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());

    if (tt == yy::parser::token_type::NUMBER)
        yylval->as<int>() = std::stoi(plex_->YYText());
    if (tt == yy::parser::token_type::NAME) {
        std::string name(plex_->YYText());
        parser::semantic_type tmp;
        tmp.as<std::string>() = name;
        yylval->swap<std::string>(tmp);
    }

    return tt;
}


bool Driver::parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
}

void Driver::addInstr(INode* instr) {
    program_.push_back(instr);
}

void Driver::launch() {
    INode* cur = nullptr;
    for (auto it = program_.begin(), it_end = program_.end(); it != it_end; ++it) {
        cur = *(it);
        cur->commit();
    }
}

void Driver::insert(NameType type, const std::string& name) {
    if (cur_symtab_ == nullptr)
        throw std::runtime_error("Empty current scope symbol table!");
    
    cur_symtab_->insert(type, name);
}

NameInfo* Driver::lookup(const std::string& name) {
    if (cur_symtab_ == nullptr)
        throw std::runtime_error("Empty current scope symbol table!");
    
    return cur_symtab_->lookup(name);
}

void Driver::insertGlobal(NameType type, const std::string& name) {
    if (cur_symtab_ == nullptr)
        throw std::runtime_error("Empty global symbol table!");
    
    global_symtab_->insert(type, name);
}

NameInfo* Driver::lookupGlobal(const std::string& name) {
    if (global_symtab_ == nullptr)
        throw std::runtime_error("Empty global symbol table!");
    
    return global_symtab_->lookup(name);
}

void Driver::openScope() {
    Symtab* symtab = new Symtab(cur_symtab_);
    cur_symtab_ = symtab;
}

void Driver::closeScope() {
    Symtab* symtab = cur_symtab_->getParent();
    delete cur_symtab_;
    cur_symtab_ = symtab;
}

Symtab* Driver::getSymtab() {
    return cur_symtab_;
}

Driver::~Driver() {
    // Remove global scope
    delete global_symtab_;
    // Remove all instruction nodes
    for (auto inode : program_) {
        delete inode;
    }
    // Delete lexer
    delete plex_;
}