#pragma once

#include <cstddef>
#include <set>
#include <map>
#include <memory>

using state_t = std::size_t;

using char_t = std::size_t;

class NFA {
	friend class CharExpr;
	friend class ThenExpr;
	friend class OrExpr;
	friend class StarExpr;
	friend class DFA;
public:
	NFA(std::size_t size);
	void add_trans(state_t from, state_t to, char_t ch);
	void add_recv(state_t recv);
	std::set<state_t> get_succ(state_t state, char_t ch) const;
	std::set<state_t> epClosure(const std::set<state_t> &states) const;
	std::shared_ptr<class DFA> toDFA() const;
	void print() const;
private:
	std::map<state_t, std::set<state_t>> trans_[256];
	state_t start_, end_;
	std::size_t size_;
	std::set<state_t> recv_;
} ;
