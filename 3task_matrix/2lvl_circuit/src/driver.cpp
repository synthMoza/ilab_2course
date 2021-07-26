#include "driver.h"

#include <algorithm>

using namespace yy;

// Driver methods

Driver::Driver() : plex_ (new yyFlexLexer), vertices_ (0), edges_ (0) {}

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

void Driver::print() const {
    std::cout << "Vetrices: " << vertices_ << std::endl;
    std::cout << "Edges: " << edges_ << std::endl << std::endl;

    for (auto&& pair : data_) {
        std::cout << "Nodes: " << pair.first.first << " -- " << pair.first.second << ", ";
        std::cout << "weight: R=" << pair.second.resistance_ << ", V=" << pair.second.voltage_ << std::endl;
    }
}

void Driver::add_pair(edge_t pair) {
    data_.push_back(pair);
}

bool Driver::parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
}

Driver::~Driver() {}