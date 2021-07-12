#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <FlexLexer.h>

#include "Node.hpp"

#include "grammar.tab.hh"

namespace yy {
    using se::BaseNode;
    using se::ScopeNode;
    using se::NameInfo;
    using se::NameType;
    
    class Driver {
        FlexLexer* plex_;
    public:
        ScopeNode* cur_scope_;

        Driver(FlexLexer* plex);

        // Method for getting the next lexem
        parser::token_type yylex(parser::location_type* l, parser::semantic_type* yylval);
        // Parse the input code
        bool parse();
        // Wrappers for scope node methods
        void insert(NameInfo* info, const std::string& name);
        NameInfo* lookup(const std::string& name);
        void addInstruction(BaseNode* node);
        // Launch the program
        int launch();

        ~Driver();
    };
}