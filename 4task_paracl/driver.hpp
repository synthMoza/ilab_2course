#pragma once

#include <iostream>
#include <vector>
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
        Driver(FlexLexer* plex) : global_symtab_ (new Symtab), cur_symtab_ {global_symtab_}, plex_ (plex) {}

        parser::token_type yylex(parser::semantic_type* yylval) {
            parser::token_type tt = static_cast<parser::token_type>(plex_->yylex());

            if (tt == yy::parser::token_type::NUMBER)
                yylval->as<int>() = std::stoi(plex_->YYText());
            if (tt == yy::parser::token_type::NAME) {
                std::string name(plex_->YYText());
                parser::semantic_type tmp;
                tmp.as<std::string>() = name;
                yylval->swap<std::string>(tmp);
            }

            std::cout.flush();
            return tt;
        }

        // Parse the input code
        bool parse() {
            parser parser(this);
            bool res = parser.parse();
            return !res;
        }

        // Add instruction to the program
        void addInstr(INode* instr) {
            program_.push_back(instr);
        }

        // Dump all program instructions
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

        // Launch the parsed program
        void launch() {
            INode* cur = nullptr;
            for (auto it = program_.rbegin(), it_end = program_.rend(); it != it_end; ++it) {
                cur = *(it);
                cur->commit();
            }
        }

        void insert(NameType type, const std::string& name) {
            if (cur_symtab_ == nullptr)
                throw std::runtime_error("Empty current scope symbol table!");
            
            cur_symtab_->insert(type, name);
        }

        NameInfo* lookup(const std::string& name) {
            if (cur_symtab_ == nullptr)
                throw std::runtime_error("Empty current scope symbol table!");
            
            return cur_symtab_->lookup(name);
        }

        void openScope() {
            Symtab* symtab = new Symtab(cur_symtab_);
            cur_symtab_ = symtab;
        }

        void closeScope() {
            Symtab* symtab = cur_symtab_->getParent();
            delete cur_symtab_;
            cur_symtab_ = symtab;
        }

        Symtab* getSymtab() {
            return cur_symtab_;
        }
    };
}