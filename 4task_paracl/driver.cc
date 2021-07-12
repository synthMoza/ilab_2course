#include "driver.hpp"

using namespace yy;

Driver::Driver(FlexLexer* plex) : cur_scope_ (new ScopeNode(nullptr)), plex_ (plex) {}

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

void Driver::insert(NameInfo* info, const std::string& name) {
    cur_scope_->insert(info, name);
}

NameInfo* Driver::lookup(const std::string& name) {
    return cur_scope_->lookup(name);
}

void Driver::addInstruction(BaseNode* node) {
    cur_scope_->addInstruction(node);
}

int Driver::launch() {
    return cur_scope_->processNode();
}

Driver::~Driver() {
    delete cur_scope_;
}