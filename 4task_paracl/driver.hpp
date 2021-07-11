#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <FlexLexer.h>

#include "Symtab.hpp"
#include "INode.hpp"

#include "grammar.tab.hh"

namespace yy {
    using se::INode;
    using se::Symtab;
    using se::NameInfo;
    using se::NameType;
    
    class Driver {
        Symtab* global_symtab_;
        Symtab* cur_symtab_;
        std::vector<INode*> program_;
        FlexLexer* plex_;
    public:
        Driver(FlexLexer* plex);

        // Method for getting the next lexem
        parser::token_type yylex(parser::location_type* l, parser::semantic_type* yylval);
        // Parse the input code
        bool parse();
        // Add instruction to the program
        void addInstr(INode* instr);
        // Launch the parsed program
        void launch();
        // Insert name in the symbol table of the current scope
        void insert(NameType type, const std::string& name);
        // Look up the name in the symbol table of the current scope
        NameInfo* lookup(const std::string& name);
        // Insert name in the symbol table of the global scope
        void insertGlobal(NameType type, const std::string& name);
        // Look up the name in the symbol table of the global scope
        NameInfo* lookupGlobal(const std::string& name);
        // Open a new scope
        void openScope();
        // Close the current scope
        void closeScope();
        // Get the symbol table of the current scope
        Symtab* getSymtab();

        ~Driver();
    };
}