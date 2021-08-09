#include "driver.h"

#include <algorithm>

using namespace yy;

// Driver methods

parser::token_type Driver::yylex(parser::semantic_type* yylval) {
    parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());

    switch (tt) {
        case yy::parser::token_type::INT:
            yylval->as<int>() = std::stoi(plex_->YYText());
            break;
        case yy::parser::token_type::DOUBLE:
            yylval->as<double>() = std::stod(plex_->YYText());
            break;
    }

    return tt;
}

bool Driver::parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
}