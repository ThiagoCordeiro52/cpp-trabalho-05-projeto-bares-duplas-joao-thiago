#include "../include/bares_manager.h"

int main( void ) {
    BaresManager bm; // an instance of class BaresManager

    std::string expr;
    while (std::getline(std::cin, expr))
    {
        bm.read_and_compute(expr);
    }

    return EXIT_SUCCESS;
}
