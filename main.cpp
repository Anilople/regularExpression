#include<stdio.h>
#include<stdlib.h>
#include "frontEnd.h"

int main()
{
	//NFA_SE a = NFA_SE_init_edge('a');
	//NFA_SE b = NFA_SE_init_edge('b');
	//NFA_SE c = NFA_SE_init_edge('c');
	// NFA_SE aClone = getNewNFA_SE_deepClone(a);
	//NFA_SE all = makeRange(makeOR(a, c), 3, 6);
	 //printNFADotRecursive(all.start);
	const char *str = "abacabacacac";
	const char *pattern = "(a&(b|c))*";
	//NFA_SE all = parseRegularExpressionOrigin(pattern);
	NFA_SE all = parseRegularExpressionWithParentheses(pattern);
	// printNFADotRecursive(all.start);
	printf("%s is %d", str, isMatchNFA_SE(str, all));
	return 0;
}