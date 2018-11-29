# 正则表达式匹配

这是一个解析正则表达式的小程序, 使用C/C++编写.

## 使用

```shell
git clone https://github.com/Anilople/regularExpression.git
make run
cat result.txt
```

提供了一些样例在`example.txt`中, 当执行`make run`后, `example.txt`中的数据会被`main.cpp`中的代码读入, 然后调用背后的api, 来进行正则匹配, 运行的结果会保存在文件'result.txt'中

`example.txt`中的格式为, 每个一个待匹配的字符串(左边), 以及模式字符串(右边), 他们中间间隔了一个空格, 这是为了方便程序的输入, `main.cpp`中使用`cin`来进行输入, `cin`遇到空格会中断, 所以如果要匹配空格, 得需要改变程序的输入方式.

## 后端部分

### NFA.h 和 NFA.cpp

最底层的部分, `NFA`代表`Nondeterministic Finite Accepters`, 主要是非确定性自动机单个节点的数据结构, 使用字符`'\0'`来代表$\lambda$转移, 有些资料也叫做$\epsilon$转移.

```c
typedef struct NFA
{
	NFA *next0;
	NFA *next1;
	char edgeValue0;
	char edgeValue1;
}NFA;
```

还有判断某个字符串是否符合这个自动机的函数

```c
/*
	这个函数使用深度优先搜索, 边作为值, 
	lambda边的值为'\0', 代表不用消耗任何字符就可以跨过这条边
*/
int isMatchNFA(const char *str, const NFA *nfa);
```

### NFA_SE.h 和 NFA_SE.cpp

`NFA_SE`代表``Nondeterministic Finite Accepters start and end`,  用来保存自动机的开始节点和结束节点, 起到封装自动机的作用.

由于构造方法的独特性, 开始节点唯一, 结束节点也唯一, 并且开始节点没有入度, 结束节点没有出度.

数据结构如下

```c
typedef struct NFA_SE
{
	NFA *start;
	NFA *end;
}NFA_SE;
NFA_SE NFA_SE_init_edge(char value);
```

注意其中的构造函数, 传入一个字符后, 能构造出带有2个节点的自动机, 边的值为字符的值.

主要的函数有3个, 分别对应自动机的与, 或, 星闭环

```c
NFA_SE makeOR(NFA_SE a, NFA_SE b);
NFA_SE makeAND(NFA_SE a, NFA_SE b);
NFA_SE makeStar(NFA_SE a);
```

## 前端部分

### frontEnd.h 和 frontEnd.cpp

负责将某个字符串转为非确定自动机, 未完成

主要函数有

```c
NFA_SE parseRegularExpressionWithParentheses(const char str[]); // 支持括号
```

这个函数可以parse一个字符串, 并返回一个非确定自动机, 不支持现代正则的[]和{}

关键字符如下

| 字符 | 作用                                                         |
| ---- | ------------------------------------------------------------ |
| ( )  | 运算的优先级, 在括号内的会优先运算                           |
| &    | 与运算, 串联自动机, "(a&b)"代表必须匹配"ab"                  |
| \|   | 或运算, 并联自动机, "(a\|b)"代表匹配可以匹配的字符串有"a", "b" |
| *    | 星闭环(star closure), 用来修饰字符或者表达式, "a*"可以匹配任意数量的'a'构成的字符串 |