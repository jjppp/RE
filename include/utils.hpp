#pragma once

#include "NFA.hpp"
#include <cstddef>
#include <functional>
#include <iterator>
#include <vector>

class Utils {
public:
  static void print_n_chars(const char ch, std::size_t n);

  class Graph {
  public:
    Graph(std::size_t nnode);
    void add_edge(std::size_t from, std::size_t to);
    std::vector<std::size_t> get_succ(std::size_t from) const;
    void color(std::vector<std::size_t> &color, state_t x, std::size_t c) const;

  private:
    std::size_t nnode_;
    std::size_t nedge_;
    std::vector<std::vector<std::size_t>> conn_;
  };
};