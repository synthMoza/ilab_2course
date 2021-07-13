#include "driver.hpp"

#include <algorithm>

using namespace yy;

// Driver methods

Driver::Driver(const char* file_name) : cur_scope_ (new ScopeNode(nullptr)), plex_ (new Scanner), file_name_ (file_name) {
    // Open the input file
    input_file_.open(file_name);

    // Set new input stream
    plex_->switch_streams(input_file_, std::cout);

    // Push all program lines to the vector
    std::string str;
    while (std::getline(input_file_, str)) {
        str.erase(remove(str.begin(), str.end(), '\t'), str.end());
        lines_.push_back(str);
    }

    input_file_.clear();
    input_file_.seekg(0);
}

parser::token_type Driver::yylex(parser::location_type* yyloc, parser::semantic_type* yylval) {
    parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());

    if (tt == yy::parser::token_type::NUMBER)
        yylval->as<int>() = std::stoi(plex_->YYText());
    if (tt == yy::parser::token_type::NAME) {
        std::string name(plex_->YYText());
        parser::semantic_type tmp;
        tmp.as<std::string>() = name;
        yylval->swap<std::string>(tmp);
    }

    // Update current location in the program
    *yyloc = plex_->getCurrentLocation();

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

NameInfo* Driver::lookup(const std::string& name) const {
    return cur_scope_->lookup(name);
}

void Driver::addInstruction(BaseNode* node) const {
    cur_scope_->addInstruction(node);
}

int Driver::launch() const {
    return cur_scope_->processNode();
}

void Driver::report_syntax_error(yy::parser::context const& ctx) const {
    std::size_t lineno = plex_->getCurrentLine();
    std::cerr << file_name_ << ": ";
    std::cerr << ASCI_Color::red() << "syntax error" << ASCI_Color::def() << " in line " << lineno << ", ";

    enum { TOKENMAX = 20 }; // max tokens amount to be printed
    yy::parser::symbol_kind_type expected_tokens[TOKENMAX];
    int num = ctx.expected_tokens(expected_tokens, TOKENMAX);

    std::cerr << "expected \"" << yy::parser::symbol_name(expected_tokens[0]) << "\"";
    for (int i = 1; i < num; ++i) {
        std::cerr << " or \"" << yy::parser::symbol_name(expected_tokens[i]) << "\"";
    }
    
    std::cerr << ":" << std::endl;
    yy::parser::location_type location = ctx.location();
    report_error_position(location);
    throw std::runtime_error("Compilation failed!");
}

void Driver::report_error_position(const parser::location_type& loc) const {
    int lineno = plex_->getCurrentLine();
    std::cerr  << lineno << "\t| " << lines_.at(lineno - 1) << std::endl;
    std::cerr << "\t| ";
    
    int begin_column = loc.begin.column;
    int end_column = loc.end.column;

    for (int i = 0; i < begin_column - 1; ++i) {
        std::cerr << "~";
    }

    int length = end_column - begin_column;
    for (int i = 0; i < length; ++i) {
        std::cerr << "^";
    }

    std::cerr << std::endl;
}

void Driver::report_parse_error(const parser::location_type& loc, error_type error) const {
    int lineno = plex_->lineno();
    std::cerr << file_name_ << ": ";
    std::cerr << ASCI_Color::red() << "parse error" << ASCI_Color::def() << " in line " << lineno << ", ";

    switch (error) {
        case ER_UNDEFINED_NAME:
            std::cerr << "undefined name: " << std::endl;
            break;
        default:
            throw std::logic_error("Unknown error type!");
    }

    report_error_position(loc);
}

Driver::~Driver() {
    input_file_.close();

    delete cur_scope_;
    delete plex_;
}

// ASCI_Color methods

const char* ASCI_Color::getColor(color type) {
    switch (type) {
        case RED:
            return "\033[31m";
        case GREEN:
            return "\033[32m";
        case YELLOW:
            return "\033[33m";
        case BLUE:
            return "\033[34m";
        case DEFAULT:
            return "\033[39m";
        default:
            return nullptr;
    }
}

const char* ASCI_Color::red() {
    return getColor(RED);
}

const char* ASCI_Color::green() {
    return getColor(GREEN);
}

const char* ASCI_Color::yellow() {
    return getColor(YELLOW);
}

const char* ASCI_Color::blue() {
    return getColor(BLUE);
}

const char* ASCI_Color::def() {
    return getColor(DEFAULT);
}