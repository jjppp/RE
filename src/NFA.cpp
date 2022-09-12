#include "NFA.hpp"
#include "DFA.hpp"
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <set>
#include <stack>
#include <vector>

NFA::NFA(std::size_t size):
	size_(size) {
}

void NFA::add_trans(state_t from, state_t to, char_t ch) {
	if (to >= size_) {
		std::cerr << "Out of Size!" << std:: endl;
		return ;
	}

	if (trans_[ch].find(from) == trans_[ch].end()) {
		trans_[ch][from] = std::set<state_t>();
	}
	trans_[ch][from].insert(to);
}

void NFA::add_recv(state_t recv) {
	if (recv>=size_) {
		std::cerr << "Out of Size!" << std:: endl;
		return ;
	}

	recv_.insert(recv);
}

std::set<state_t> NFA::get_succ(state_t state, char_t ch) const {
	if (trans_[ch].find(state) != trans_[ch].end()) {
		return trans_[ch].find(state)->second;
	}
	return {};
}

static void printGraphvizList(std::fstream &fs, state_t from, char_t ch, const std::set<state_t> &states) {
	for (state_t to : states) {
		if (ch) {
			fs << "\t" << from << " -> " << to << " [style=bold,label=\" " << ch << "\"];" << std::endl;
		} else {
			fs << "\t" << from << " -> " << to << " [style=bold,label=\" ε\"];" << std::endl;
		}
	}
}

void NFA::print() const {
	std::fstream fs("NFA.gv", std::ios_base::out);
	fs << "//Number of Nodes: " << size_ << std::endl;
	fs << "digraph G {\n\tnodesep=1;\n\tranksep=0.6;\n\trankdir=LR;\n";
	for (std::size_t state = 0; state < size_; ++ state) {
		for (char_t ch = 0; ch < 256; ++ ch) {
			if (trans_[ch].find(state) != trans_[ch].end()) {
				printGraphvizList(fs, state, ch, trans_[ch].find(state)->second);
			}
		}
	}
	fs << "\t0:body[shape=doublecircle, width=0.4];"<< std::endl;
	for (std::size_t state = 1; state + 1 < size_; ++ state) {
		fs << "\t" << state << ":body[shape=circle, width=0.4];" << std::endl;
	}
	fs << "\t" << size_ - 1 << ":body[shape=doublecircle, width=0.4];" << std::endl;
	fs << "}" << std::endl;
}

std::set<state_t> NFA::epClosure(const std::set<state_t> &states) const {
	std::set<state_t> result = states;
	while (true) {
		auto rec_size = result.size();
		std::set<state_t> tmp_set;

		for (state_t state : result) {
			tmp_set.merge(get_succ(state, 0));
		}

		result.merge(tmp_set);
		if (rec_size==result.size()) {
			return result;
		}
	}
}

std::shared_ptr<DFA> NFA::toDFA() const {
	auto dfa = std::make_shared<DFA>(DFA(size_ * 100));
	std::set<state_t> states[dfa->size_];
	states[1].insert(0);
	states[1] = epClosure(states[1]);
	
	std::size_t total_list = 1;

	std::stack<std::size_t> stack; stack.push(1);
	for (; !stack.empty(); ) {
		std::size_t top = stack.top(); stack.pop();
		auto cur_list = states[top];

		for (char_t ch = 1; ch < 256; ++ ch) {
			std::set<state_t> tar_list;

			for (state_t state : cur_list) {
				tar_list.merge(get_succ(state, ch));
			}

			if (tar_list.empty()) {
				continue;
			}

			tar_list = epClosure(tar_list);
			bool flag = false;
			for (std::size_t i = 1; i <= total_list; ++ i) {
				if (states[i] == tar_list) {
					dfa->add_trans(top, i, ch);
					flag = true;
				}
			}
			
			if (!flag) {
				states[++total_list] = tar_list;
				dfa->add_trans(top, total_list, ch);
				stack.push(total_list);
			}
		}
	}
	for (std::size_t i = 1; i <= total_list; ++ i) {
		if (states[i].find(end_) != states[i].end()) {
			dfa->add_recv(i);
		}
	}
	dfa->size_ = total_list;
	return dfa;
}
