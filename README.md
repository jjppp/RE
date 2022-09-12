# RE

An simple implementation of Regex Expressions

Supports

* ASCII-text
* epsilon-closure
* or operator
* interactive interface

# Passes

regex -> post-order form -> regex AST -> NFA -> DFA -> minimized_DFA

# Special

On succeed, `origin_DFA.gv` `minimized_DFA.gv` and `NFA.gv` will be generated, which can be visualized using graphviz.

# TODO
* - [ ] Richer syntactic features
* - [ ] File support
* + [x] Interactive interface
* + [x] Minimization algorithm
* + [x] Turn to C++ to avoid making wheels
