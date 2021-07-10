#pragma once

#include <iostream>
#include <vector>
#include <FlexLexer.h>

#include "INode.hpp"
#include "grammar.tab.hh"

namespace yy {
    using se::INode;
    
    class Driver {
        std::vector<INode*> program_;
        FlexLexer* plex_;
    public:
        Driver(FlexLexer* plex) : plex_ (plex) {}

        parser::token_type yylex(parser::semantic_type* yylval) {
            parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());

            if (tt == yy::parser::token_type::NUMBER)
                yylval->as<int>() = std::stoi(plex_->YYText());
            
            return tt;
        }

        bool parse() {
            parser parser(this);
            bool res = parser.parse();
            return !res;
        }

        void addInstr(INode* instr) {
            program_.push_back(instr);
        }

        void dumpProgram() {
            INode* cur = nullptr;
            int counter  = 0;
            
            std::cout << "\nPROGRAM DUMP:\n" << std::endl;
            for (auto it = program_.rbegin(), it_end = program_.rend(); it != it_end; ++it) {
                cur = *(it);
                std::cout << "[" << counter++ << "]" << "Instruction: ";
                cur->node_->dumpNode();
                std::cout << std::endl;
            }
        }

        void launch() {
            INode* cur = nullptr;

            for (auto it = program_.rbegin(), it_end = program_.rend(); it != it_end; ++it) {
                cur = *(it);
                cur->commit();
            }
        }
    };
}