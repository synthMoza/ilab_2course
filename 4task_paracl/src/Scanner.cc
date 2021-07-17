#include "Scanner.hpp"

using namespace yy;

Scanner::Scanner() : cur_line_{1}, cur_loc_{} {}

yy::parser::location_type Scanner::getCurrentLocation() {
    return cur_loc_;
}

std::size_t Scanner::getCurrentLine() {
    return cur_line_;
}

void Scanner::updateLocation() {
    std::size_t line = lineno();
    cur_loc_.lines(line - cur_line_);
    cur_loc_.step();
    if (line - cur_line_ == 0)
        cur_loc_.columns(YYLeng());
    
    cur_line_ = line;
}