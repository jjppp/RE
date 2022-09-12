#pragma once
#include "NFA.hpp"
#include <cstddef>
#include <memory>

class Expr {
public:
	virtual ~Expr() = default;
	virtual std::size_t calc_nfa_size() = 0;
	virtual void print(std::size_t dep) = 0;
	virtual void build(std::shared_ptr<NFA> nfa, std::size_t offset) = 0;
	std::size_t size_;
} ;

class OrExpr : public Expr {
public:
	~OrExpr();
	OrExpr(std::shared_ptr<Expr> fst, std::shared_ptr<Expr> snd);
	std::size_t calc_nfa_size();
	void print(std::size_t dep);
	void build(std::shared_ptr<NFA> nfa, std::size_t offset);
private:
	std::shared_ptr<Expr> lhs_, rhs_;
} ;

class ThenExpr : public Expr {
public:
	~ThenExpr();
	ThenExpr(std::shared_ptr<Expr> fst, std::shared_ptr<Expr> snd);
	std::size_t calc_nfa_size();
	void print(std::size_t dep);
	void build(std::shared_ptr<NFA> nfa, std::size_t offset);
private:
	std::shared_ptr<Expr> fst_, snd_;
} ;

class StarExpr : public Expr {
public:
	~StarExpr();
	StarExpr(std::shared_ptr<Expr> sub);
	std::size_t calc_nfa_size();
	void print(std::size_t dep);
	void build(std::shared_ptr<NFA> nfa, std::size_t offset);
private:
	std::shared_ptr<Expr> sub_;
} ;

class CharExpr : public Expr {
public:
	~CharExpr();
	CharExpr(const char_t ch);
	std::size_t calc_nfa_size();
	void print(std::size_t dep);
	void build(std::shared_ptr<NFA> nfa, std::size_t offset);
private:
	char_t ch_;
} ;