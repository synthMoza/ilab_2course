#include <iostream>

#include "driver.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./out <input_file>" << std::endl;
        return -1;
    }

    yy::Driver driver(argv[1]);
    try {
        driver.parse();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}