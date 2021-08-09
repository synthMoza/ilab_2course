#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <FlexLexer.h>

#include "weight.h"
#include "compiler.hpp"

namespace yy {
    using se::Weight;
    using edge_t = std::pair<std::pair<std::size_t, std::size_t>, se::Weight>;

    class Driver {
        std::unique_ptr<FlexLexer> plex_; // scan the text for lexical tokens
        // Vector that will contain pairs of:
        // - pair of adjacent nodes
        // - weight of the their edge represented by its resistance and voltage 
        std::vector<edge_t> data_;
    public:
        Driver() : plex_ (new yyFlexLexer) {}
        // Add this pair to the vector
        void add_pair(edge_t edge) {
            data_.push_back(edge);
        }
        // Method for getting the next lexem
        parser::token_type yylex(parser::semantic_type* yylval);
        // Parse the input code
        bool parse();
        // Get the input data through the vector
        std::vector<edge_t> getVector() {
            std::vector<edge_t> tmp{std::move(data_)};
            return tmp;
        }

        ~Driver() {}
    };
}