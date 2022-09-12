#include "DFA.hpp"
#include "NFA.hpp"
#include "utils.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stack>
#include <vector>

DFA::DFA(std::size_t size) {
  size_ = size;
  state_ = 0;
}

void DFA::add_trans(state_t from, state_t to, char_t ch) {
  if (to > size_) {
    std::cerr << "Out of Size!" << std::endl;
    return;
  }

  trans_[ch][from] = to;
  std::cout << "addDFATrans " << from << " -> " << to << ", ch = " << ch
            << std::endl;
}

void DFA::add_recv(state_t recv) {
  if (recv > size_) {
    std::cerr << "Out of Size!" << std::endl;
    return;
  }
  recv_.insert(recv);
}

std::optional<state_t> DFA::next(state_t state, char_t ch) const {
  if (trans_[ch].find(state) == trans_[ch].end()) {
    return {};
  }
  return {trans_[ch].find(state)->second};
}

std::optional<state_t> DFA::move(char_t ch) {
  if (trans_[ch].find(state_) == trans_[ch].end()) {
    std::cerr << "No such Transfer!" << std::endl;
    return {};
  }
  state_ = trans_[ch][state_];
  return {state_};
}

struct edge {
  std::size_t y, next;
};

std::shared_ptr<DFA> DFA::minimize() const {
  std::size_t res_dfa_size = 2;

  std::vector<std::size_t> bel;
  bel.resize(size_ + 3);

  for (state_t state = 1; state <= size_; ++state) {
    if (recv_.find(state) != recv_.end()) {
      bel[state] = 1;
    } else {
      bel[state] = 2;
    }
  }

  while (true) {
    Utils::Graph graph(size_ + 3);
    for (state_t sx = 1; sx <= size_; ++sx) {
      for (state_t sy = sx + 1; sy <= size_; ++sy) {
        if (bel[sx] != bel[sy]) {
          continue;
        }
        bool flag = false;
        for (char_t ch = 0; ch < 256; ++ch) {
          auto tx = next(sx, ch);
          auto ty = next(sy, ch);

          if (tx.has_value() && ty.has_value()) {
            if (bel[tx.value()] != bel[ty.value()]) {
              flag = true;
              break;
            }
          } else {
            if (tx.has_value() != ty.has_value()) {
              flag = true;
              break;
            }
          }
        }
        if (!flag) {
          graph.add_edge(sx, sy);
        }
      }
    }
    std::size_t col = 0;
    for (state_t state = 1; state <= size_; ++state)
      bel[state] = 0;
    for (state_t state = 1; state <= size_; ++state) {
      if (!bel[state]) {
        graph.color(bel, state, ++col);
      }
    }
    if (col == res_dfa_size)
      break;
    res_dfa_size = col;
  }
  auto res_dfa = std::make_shared<DFA>(res_dfa_size);
  for (state_t sx = 1; sx <= size_; ++sx) {
    if (recv_.find(sx) != recv_.end())
      res_dfa->add_recv(bel[sx]);
    for (char_t ch = 0; ch < 256; ++ch) {
      std::size_t from = bel[sx];
      if (trans_[ch].find(sx) != trans_[ch].end()) {
        std::size_t to = bel[trans_[ch].find(sx)->second];
        res_dfa->add_trans(from, to, ch);
      }
    }
  }
  return res_dfa;
}

void DFA::print_to_file(const std::string &filename) const {
  std::fstream fs{filename + ".gv"};
  fs << "//Number of Nodes: " << size_ << std::endl;

  std::size_t edge_cnt = 0;
  for (std::size_t state = 1; state <= size_; ++state) {
    for (char_t ch = 0; ch < 256; ++ch) {
      edge_cnt += (trans_[ch].find(state) != trans_[ch].end());
    }
  }

  fs << "//Number of Edges: " << edge_cnt << std::endl;
  fs << "digraph G {\n\tnodesep=1.5;\n\tranksep=0.6;\n\trankdir=LR;"
     << std::endl;
  for (std::size_t state = 1; state <= size_; ++state) {
    for (char_t ch = 0; ch < 256; ++ch) {
      auto map_result = trans_[ch].find(state);
      if (map_result != trans_[ch].end()) {
        auto tar_state = map_result->second;
        fs << "\t" << state << " -> " << tar_state << "[style=bold,label=\" "
           << ch << "\"];" << std::endl;
        std::cout << state << " -> " << tar_state << " " << ch << std::endl;
      }
    }
  }
  fs << "\t1:body[shape=doublecircle, width=0.4];" << std::endl;
  for (std::size_t state = 2; state <= size_; ++state) {
    if (recv_.find(state) != recv_.end()) {
      fs << "\t" << state << ":body[shape=doublecircle, width=0.4];"
         << std::endl;
    } else {
      fs << "\t" << state << ":body[shape=circle, width=0.4];" << std::endl;
    }
  }
  fs << "}";
}

void DFA::reset() { state_ = 1; }

bool DFA::is_recv() const { return recv_.find(state_) != recv_.end(); }

bool DFA::match(const std::string &str) {
  for (char ch : str) {
    if (!move(ch).has_value()) {
      return false;
    }
  }
  return is_recv();
}