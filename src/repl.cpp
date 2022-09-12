#include "DFA.hpp"
#include "NFA.hpp"

#include <cstdlib>
#include <iostream>

static void prompt() {
    std:: cout << "RE:>";
}

int repl(std::shared_ptr<DFA> dfa) {
    std::cout << "RE:>";
    std::string input;
    prompt();
    while (std::cin >> input) {
        dfa->reset();
        for (char ch : input) {
            dfa->move(static_cast<char_t>(ch));
        }
        if (dfa->is_recv()) {
            std::cout << "matched!" << std::endl;
        }
        prompt();
    }
    return EXIT_SUCCESS;
}