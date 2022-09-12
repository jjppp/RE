#include <cstddef>
#include <iostream>

#include "utils.hpp"

void Utils::print_n_chars(const char ch, std::size_t n) {
    for (std::size_t i = 0; i < n; ++ i) {
        std::cout << ch;
    }
}