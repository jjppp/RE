#pragma once

#include "NFA.hpp"

#include <cstddef>
#include <optional>

class DFA {
	friend class NFA;
public:
	DFA(std::size_t size);
	void add_trans(state_t from, state_t to, char_t ch);
	void add_recv(state_t recv);
	bool is_recv() const;
	void print_to_file(const std::string &filename) const;
	std::optional<state_t> move(char_t ch);
	std::optional<state_t> next(state_t state, char_t ch) const;
	std::shared_ptr<DFA> minimize() const;
	void reset();
private:
	std::map<state_t, state_t> trans_[256];
	unsigned int size_;
	state_t state_;
	std::set<state_t> recv_;
} ;
