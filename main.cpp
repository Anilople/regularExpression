#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include "frontEnd.h"

void outOneMatchSituaction(const char *str, const char *pattern)
{
	NFA_SE automa = parseRegularExpressionWithParentheses(pattern); // parse pattern to automa
	if(isMatchNFA_SE(str, automa)) {
		printf("-----------------------\n");
		printf("pattern:%s\n", pattern);
		printf("string: %s\n", str);
		printf("match!\n");
		printf("-----------------------\n");
	} else {
		printf("-----------------------\n");
		printf("pattern:%s\n", pattern);
		printf("string: %s\n", str);
		printf("not match!\n");
		printf("-----------------------\n");
	}
	// print automa to dot language
	// you can use this function to print the structure of automa
	// then copy them to dot language
	// printNFADotRecursive(automa.start);
}

int main()
{
	static char str[10000];
	static char pattern[1000];
	// one line contain a string and a pattern, 
	// they split by some empty chars
	// waring!! do not input space, because cin 
	while(std::cin >> str >> pattern) {
		outOneMatchSituaction(str, pattern);
	}
	return 0;
}