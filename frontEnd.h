#pragma once
#include "NFA_SE.h"
#include <stack>
#include <vector>

/*
	keyword:
		& | *
		use "|" to denote OR operation, (a|b) mean match (1's a or 1's b)
		use "&" to denote AND operation, (a&a) mean must match 2's a
		use "*" to denote STAR closure
		not use ( ) [ ] { } as token in this level
	priority:
		"*" has highest priority
		"&" and "|" have same priority, and operate from left to right
	example:
			"a&b|c" = "(a&b)|c"
			"a&b|c&d" = "(a&b)|c&d" = "((a&b)|c)&d"
*/
NFA_SE parseRegularExpressionOrigin(const char str[]);

/*
	keyword:
		( ) & | *
		use "|" to denote OR operation
		use "&" to denote AND operation
		use "*" to denote STAR closure
		not use [ ] { } in this level
	priority:
		content in "()" has highest priority
		content before "*" has second priority
		"&" and "|" have lowest but same priority

	info:
		function parseRegularExpressionOrigin do not suppose "(" and ")"
		in this function, add "(" and ") to control the priority of expression
*/
NFA_SE parseRegularExpressionWithParentheses(const char str[]);