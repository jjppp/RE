#include <cstddef>
#include <iostream>
#include <vector>

#include "NFA.hpp"
#include "utils.hpp"

void Utils::print_n_chars(const char ch, std::size_t n) {
  for (std::size_t i = 0; i < n; ++i) {
    std::cout << ch;
  }
}

Utils::Graph::Graph(std::size_t nnode) {
  nnode_ = nnode;
  nedge_ = 0;
  conn_.resize(nnode_);
}

void Utils::Graph::add_edge(std::size_t from, std::size_t to) {
  nnode_ = std::max(nnode_, from);
  nnode_ = std::max(nnode_, to);
  nedge_ += 1;
  conn_.resize(nnode_ + 1);
  conn_[from].push_back(to);
}

std::vector<std::size_t> Utils::Graph::get_succ(std::size_t from) const {
  return conn_.at(from);
}

void Utils::Graph::color(std::vector<std::size_t> &color, state_t x,
                         std::size_t c) const {
  if (color[x]) {
    return;
  }
  color[x] = c;
  for (auto y : get_succ(x)) {
    this->color(color, y, c);
  }
}
