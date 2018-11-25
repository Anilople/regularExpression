# 正则表达式匹配

这是一个解析正则表达式的小程序, 使用C/C++编写.

## NFA.h 和 NFA.cpp

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

## NFA_SE.h 和 NFA_SE.cpp

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

