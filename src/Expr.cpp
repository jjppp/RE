#include "Expr.hpp"
#include "NFA.hpp"
#include "utils.hpp"

#include <cstddef>
#include <iostream>
#include <memory>

ThenExpr::ThenExpr(std::shared_ptr<Expr> fst, std::shared_ptr<Expr> snd)
    : fst_{fst}, snd_{snd} {}

OrExpr::OrExpr(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
    : lhs_{lhs}, rhs_{rhs} {}

StarExpr::StarExpr(std::shared_ptr<Expr> sub) : sub_{sub} {}

CharExpr::CharExpr(const char_t ch) : ch_{ch} {}

ThenExpr::~ThenExpr() {}

OrExpr::~OrExpr() {}

StarExpr::~StarExpr() {}

CharExpr::~CharExpr() {}

std::size_t CharExpr::calc_nfa_size() {
  size_ = 2;
  return 2;
}

std::size_t OrExpr::calc_nfa_size() {
  std::size_t lhs_size = lhs_->calc_nfa_size();
  std::size_t rhs_size = rhs_->calc_nfa_size();
  size_ = lhs_size + rhs_size + 2;
  return size_;
}

std::size_t ThenExpr::calc_nfa_size() {
  std::size_t fst_size = fst_->calc_nfa_size();
  std::size_t snd_size = snd_->calc_nfa_size();
  size_ = fst_size + snd_size;
  return size_;
}

std::size_t StarExpr::calc_nfa_size() {
  size_ = sub_->calc_nfa_size() + 2;
  return size_;
}

void CharExpr::print(std::size_t dep) {
  Utils::print_n_chars(' ', dep);
  std::cout << "CharExpr" << std::endl;
}

void OrExpr::print(std::size_t dep) {
  Utils::print_n_chars(' ', dep);
  std::cout << "OrExpr" << std::endl;
  lhs_->print(dep + 1);
  rhs_->print(dep + 1);
}

void ThenExpr::print(std::size_t dep) {
  Utils::print_n_chars(' ', dep);
  std::cout << "ThenExpr" << std::endl;
  fst_->print(dep + 1);
  snd_->print(dep + 1);
}

void StarExpr::print(std::size_t dep) {
  Utils::print_n_chars(' ', dep);
  std::cout << "StarExpr" << std::endl;
  sub_->print(dep + 1);
}

void CharExpr::build(std::shared_ptr<NFA> nfa, std::size_t offset) {
  nfa->add_trans(offset, offset + 1, ch_);
  nfa->start_ = offset;
  nfa->end_ = offset + 1;
}

void ThenExpr::build(std::shared_ptr<NFA> nfa, std::size_t offset) {
  fst_->build(nfa, offset);
  state_t s1 = nfa->start_;
  state_t e1 = nfa->end_;

  nfa->start_ = nfa->end_ = 0;
  snd_->build(nfa, offset + fst_->size_);
  nfa->add_trans(e1, nfa->start_, 0);
  nfa->start_ = s1;
}

void OrExpr::build(std::shared_ptr<NFA> nfa, std::size_t offset) {
  lhs_->build(nfa, offset + 2);
  state_t s1 = nfa->start_;
  state_t e1 = nfa->end_;

  nfa->start_ = nfa->end_ = 0;
  rhs_->build(nfa, offset + 2 + lhs_->size_);

  state_t s2 = nfa->start_;
  state_t e2 = nfa->end_;
  nfa->start_ = offset;
  nfa->end_ = offset + 1;

  nfa->add_trans(offset, s1, 0);
  nfa->add_trans(offset, s2, 0);
  nfa->add_trans(e1, offset + 1, 0);
  nfa->add_trans(e2, offset + 1, 0);
}

void StarExpr::build(std::shared_ptr<NFA> nfa, std::size_t offset) {
  sub_->build(nfa, offset + 2);
  state_t s1 = nfa->start_;
  state_t e1 = nfa->end_;

  nfa->start_ = offset;
  nfa->end_ = offset + 1;

  nfa->add_trans(offset, offset + 1, 0);
  nfa->add_trans(offset, s1, 0);
  nfa->add_trans(e1, offset + 1, 0);
  nfa->add_trans(e1, s1, 0);
}
