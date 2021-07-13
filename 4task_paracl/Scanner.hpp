#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "compiler.tab.hh"

namespace yy {
    // Own lexer class with methods for determing and updating location
    class Scanner : public yyFlexLexer {
        yy::parser::location_type cur_loc_;
        std::size_t cur_line_;
    public:
        Scanner();

        yy::parser::location_type getCurrentLocation();
        std::size_t getCurrentLine();
        void updateLocation();
        int yylex() override;
        virtual ~Scanner() = default;
    };
}