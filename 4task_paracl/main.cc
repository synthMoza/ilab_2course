#include <iostream>
#include <fstream>

#include "driver.hpp"

int yyFlexLexer::yywrap() {
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./out <input_file>" << std::endl;
        return -1;
    }

    // Open the input file
    std::ifstream input_file;
    input_file.open((argv[1]));

    FlexLexer* lexer = new yyFlexLexer;
    // Set new input stream
    lexer->switch_streams(input_file, std::cout);
    yy::Driver driver(lexer);
    driver.parse();
    driver.launch();

    input_file.close();
    return 0;
}