# RE
简单的正则表达式识别器

# 原理
正则表达式->双栈处理得到后缀表达式->逆序构造正则表达式的AST->遍历AST构造等价的NFA->根据NFA->构造等价的DFA->最小化DFA

# 特色
成功识别正则表达式后将会导出`origin_DFA.gv`，`minimized_DFA.gv`和`NFA.gv`三个文件，用graphviz即可获得对应自动机的图例

# TODO
* - [ ] 在NFA转DFA的时候使用了静态的size，要改成增量构造
* - [ ] 可以把原本的`StateList`重构成Intrusive LinkedLists
* - [ ] 好的文件扫描
* + [x] 要提供可以交互构造正则表达式和识别文本的界面
* + [x] `StateList`实现的Set容器效率太低，合并的时候偷懒写了$\Theta(n^2)$的...现在把List合并改掉了
* + [x] 最小化DFA。虽然实现了但好像还有点问题....
* + [x] 实现了stack容器
