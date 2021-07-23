#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <memory>
#include <stack>
#include <FlexLexer.h>

#include "Node.hpp"
#include "Scanner.hpp"
#include "compiler.hh"

namespace yy {
    using se::BaseNode;
    using se::ScopeNode;
    using se::NameInfo;
    using se::NameType;

    // Runtime error types (during parsing)
    enum error_type {
        ER_UNDEFINED_NAME, ER_WRONG_ARGS
    };

    class Driver {
        std::unique_ptr<Scanner> plex_;
        std::ifstream input_file_;
        std::vector<std::string> lines_; // all program lines to report errors
        const char* file_name_;
        ScopeNode* cur_scope_; // curent program scope (global scope by default)
        std::stack<ScopeNode*> saved_scopes_; // save last scope in case it was changed to some new unrelated (like function scope) 
    public:
        
        // Handle scope
        ScopeNode* getScope() const {
            return cur_scope_;
        }
        void setScope(ScopeNode* scope) {
            cur_scope_ = scope;
        }
        // Handle saved scope
        void setTempScope(ScopeNode* scope) {
            saved_scopes_.push(cur_scope_);
            cur_scope_ = scope;
        }
        void resetScope() {
            cur_scope_ = saved_scopes_.top();
            saved_scopes_.pop();
        }

        Driver(const char* file_name);

        // Method for getting the next lexem
        parser::token_type yylex(parser::location_type* l, parser::semantic_type* yylval);
        // Parse the input code
        bool parse();
        // Wrappers for scope node methods
        void insert(NameInfo* info, const std::string& name);
        NameInfo* lookup(const std::string& name) const;
        void addInstruction(BaseNode* node) const;
        // Launch the program
        int launch() const;
        // Error handling
        void report_syntax_error(parser::context const& ctx) const;
        void report_error_position(const parser::location_type& loc) const;
        void report_parse_error(const parser::location_type& loc, error_type error) const;
        ~Driver();
    };

    // Colors for the output
    enum color {
        RED, GREEN, YELLOW, BLUE, DEFAULT
    };

    class ASCI_Color {
        static const char* getColor(color type);
    public:
        static const char* red();
        static const char* green();
        static const char* yellow();
        static const char* blue();
        static const char* def();
    };
}