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

