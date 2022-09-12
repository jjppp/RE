#include "DFA.hpp"
#include "NFA.hpp"

#include <cstdlib>
#include <iostream>

static void prompt() { std::cout << "RE:>"; }

int repl(std::shared_ptr<DFA> dfa) {
  std::string input;
  prompt();
  while (std::cin >> input) {
    dfa->reset();
    if (dfa->match(input)) {
      std::cout << "matched!" << std::endl;
    }
    prompt();
  }
  return EXIT_SUCCESS;
}