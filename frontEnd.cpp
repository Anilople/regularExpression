#include "NFA_SE.h"
#include <stdio.h>
#include <stack>
#include <vector>

static NFA_SE makeAND_vector(std::vector<char> topBuffer)
{
	if (0 == topBuffer.size()) {
		return getLambda();
	}
	else {
		NFA_SE concat = NFA_SE_init_edge(*topBuffer.begin());
		for (int i = 1; i < topBuffer.size(); i++) {
			NFA_SE now = NFA_SE_init_edge(topBuffer[i]);
			concat = makeAND(now, concat);
		}
		return concat;
	}
}

/*
	only "&" and "|" in symbols, no "*"

	backward mean:
		expressions: a b c d
		symbols: & | &
		(right part is stack top, left part is bottom)
		you will get (((d&c)|c)&a)
*/
static NFA_SE makeOperationsBackward(std::stack<NFA_SE> expressions, std::stack<char> symbols)
{
	// make "&" "|" operation, "*" operation have done before.
	while (!symbols.empty()) {
		char symbol = symbols.top();
		symbols.pop();
		switch (symbol)
		{
		case '&': { // get top 2 NFA_SE to make a "and" operation
			if (expressions.size() < 2) {
				printf("number of automa is not enough to make &.\n");
			}
			else {
				NFA_SE top1 = expressions.top();
				expressions.pop();
				NFA_SE top2 = expressions.top();
				expressions.pop();
				NFA_SE newTop = makeAND(top1, top2); // and operation
				expressions.push(newTop);
			}
		}
				  break;
		case '|': { // get top 2 NFA_SE to make a "or" operation
			if (expressions.size() < 2) {
				printf("number of automa is not enough to make |.\n");
			}
			else {
				NFA_SE top1 = expressions.top();
				expressions.pop();
				NFA_SE top2 = expressions.top();
				expressions.pop();
				NFA_SE newTop = makeOR(top1, top2); // or operation
				expressions.push(newTop);
			}
		}
				  break;
		default:
			printf("%c is not the keyword symbol.\n", symbol);
			break;
		}
	}

	// expressions size must be 1 now
	if (1 != expressions.size()) {
		printf("number of automa is not right after & and | operation.\n");
		if (expressions.size() < 1) {
			printf("size < 1.\n");
		}
		else {
			printf("size > 1.\n");
		}
		return NFA_SE_init(NULL, NULL);
	}
	else {
		return expressions.top();
	}
}

/*
	only "&" and "|" in symbols, no "*"

	forward mean:
		expressions: a b c d
		symbols: & | &
		(right part is stack top, left part is bottom)
		you will get (((a&b)|c)&d)
*/
static NFA_SE makeOperationsForward(std::stack<NFA_SE> expressions, std::stack<char> symbols)
{
	// reverse the stack
	std::stack<NFA_SE> expressionsReverse;
	while (!expressions.empty()) {
		expressionsReverse.push(expressions.top());
		expressions.pop();
	}
	expressions = expressionsReverse;

	std::stack<char> symbolsReverse;
	while (!symbols.empty()) {
		symbolsReverse.push(symbols.top());
		symbols.pop();
	}
	symbols = symbolsReverse;
	return makeOperationsBackward(expressions, symbols);
}
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
static NFA_SE parseRegularExpressionOrigin(const char str[])
{
	std::stack<NFA_SE> expressions;
	std::stack<char> symbols;
	for (int i = 0; '\0' != str[i]; i++) {
		char charNow = str[i];
		switch (charNow)
		{
		case '&':
			symbols.push('&');
			break;
		case '|':
			symbols.push('|');
			break;
		case '*': { // when meet "*", make star closure immidiately
			NFA_SE top = expressions.top();
			expressions.pop();
			NFA_SE newTop = makeStar(top);
			expressions.push(newTop);
		}
				  break;
		default: // let normal char to NFA
			expressions.push(NFA_SE_init_edge(charNow));
			break;
		}
	}

	return makeOperationsForward(expressions, symbols);
}

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
NFA_SE parseRegularExpressionWithParentheses(const char str[])
{
	/*
		use multi stack to emulate recursion in function
	*/

	std::stack<std::stack<NFA_SE> > expressionsBuffers;
	std::stack<NFA_SE> emptyExpressions;
	expressionsBuffers.push(emptyExpressions); // must initialize a empty to be the top
	// when meet a '(', then push a new buffer to top
	// only '&' and '|' in symbols
	std::stack<std::stack<char> > symbolsBuffers;
	std::stack<char> emptySymbols;
	symbolsBuffers.push(emptySymbols); // must initialize a empty to be the top

	for (int i = 0; '\0' != str[i]; i++) {
		char charNow = str[i];
		switch (charNow)
		{
		case '(':
			// make new buffer to save expressions and symbols
			expressionsBuffers.push(std::stack<NFA_SE>());
			symbolsBuffers.push(std::stack<char>());
			break;
		case ')': {
			// use the top of expressions and symbols to make new NFA_SE
			// this NFA_SE will push in lower level stack
			std::stack<NFA_SE> topExpressions = expressionsBuffers.top();
			expressionsBuffers.pop();
			std::stack<char> topSymbols = symbolsBuffers.top();
			symbolsBuffers.pop();
			NFA_SE newTopExpression = makeOperationsForward(topExpressions, topSymbols);
			expressionsBuffers.top().push(newTopExpression);
		}
				  break;
		case '&':
			symbolsBuffers.top().push('&');
			break;
		case '|':
			symbolsBuffers.top().push('|');
			break;
		case '*': { // when meet "*", make star closure immidiately
			NFA_SE topTop = expressionsBuffers.top().top();
			expressionsBuffers.top().pop();
			NFA_SE newTopTop = makeStar(topTop);
			expressionsBuffers.top().push(newTopTop);
		}
				  break;
		default: // normal char
			// make it to NFA_SE, 
			// then save this NFA_SE to the top of expressionsBuffers
			expressionsBuffers.top().push(NFA_SE_init_edge(charNow));
			break;
		}
	}

	// now, all expression in () have been parsed.
	std::stack<NFA_SE> expressions = expressionsBuffers.top();
	expressionsBuffers.pop();
	std::stack<char> symbols = symbolsBuffers.top();
	symbolsBuffers.pop();
	if (!expressionsBuffers.empty() || !symbolsBuffers.empty()) {
		printf("there is still something in expressionsBuffers or symbolsBuffers.\n");
		printf("so parse failed.\n");
		return NFA_SE_init(NULL, NULL);
	}
	else {
		return makeOperationsForward(expressions, symbols);
	}
}