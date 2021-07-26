#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <FlexLexer.h>

#include "matrix.h"
#include "weight.h"
#include "compiler.hpp"


namespace yy {
    using se::Weight;
    using se::Matrix;
    using se::solve_linear;

    using edge_t = std::pair<std::pair<std::size_t, std::size_t>, se::Weight>;
    using msize_type = typename Matrix<double>::size_type;

    class Driver {
        std::unique_ptr<FlexLexer> plex_; // scan the text for lexical tokens
        // Vector that will contain pairs of:
        // - pair of adjacent nodes
        // - weight of the their edge represented by its resistance and voltage 
        std::vector<edge_t> data_;
        std::size_t vertices_; // total amount of vetrices in the graph
        std::size_t edges_; // total amount of edges in the graph
    public:
        Driver();
        // Temp function for printing the vector
        void print() const;
        // Add this pair to the vector
        void add_pair(edge_t edge);
        // Method for getting the next lexem
        parser::token_type yylex(parser::semantic_type* yylval);
        // Parse the input code
        bool parse();
        // Get the input data through the vector
        std::vector<edge_t> getVector() {
            std::vector<edge_t> tmp{std::move(data_)};
            return tmp;
        }
        ~Driver();
    };
}