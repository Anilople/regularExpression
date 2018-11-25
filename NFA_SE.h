#pragma once
#include "NFA.h"
/*
	nfa start and end
*/
typedef struct NFA_SE
{
	NFA *start;
	NFA *end;
}NFA_SE;

NFA_SE NFA_SE_init(NFA *start, NFA *end);
NFA_SE NFA_SE_init_edge(char value);

/*
	use depth first search
*/
int isMatchNFA_SE(const char *str, NFA_SE nfa_se);

/*
	build a new automa and delete old.
*/
NFA_SE getNewNFA_SE_deepClone(NFA_SE nfa_se);

/*
	return lambda transition nfa.
	i.e now -> next (lambda transition)
*/
NFA_SE getLambda();

/*
	a and b will be cloned
*/
NFA_SE makeOR(NFA_SE a, NFA_SE b);

/*
	a and b will be cloned
*/
NFA_SE makeAND(NFA_SE a, NFA_SE b);
/*
	a will be cloned
*/
NFA_SE makeStar(NFA_SE a);

/* function follow base on uppon*/

/*
	assume : arr != NULL, len >= 2
*/
NFA_SE makeOR_Array(NFA_SE *arr, int len);
/*
	assume : arr != NULL, len >= 2
*/
NFA_SE makeAND_Array(NFA_SE *arr, int len);

/*
	return a dfa satisfy match "one" n times.

	assume : n >= 0
		if n = 0 return lambda nfa
*/
NFA_SE makeAND_repeat(NFA_SE one, int n);

/*
	high >= 0

	(pattern){0, 0} = lambda transition edge.
	(pattern){0, n} = (pattern){n, n} | (pattern){0, n - 1}

	there is a way which use less memory.
	(pattern){0, 0} = lambda transition edge.
	(pattern){0, n} =  ( (pattern) | (lambda transition) ) & (pattern){0, n - 1}
*/
NFA_SE makeRangeToHigh(NFA_SE one, int high);

/*
	low >= 0

	(pattern){low, infinity}
	= (pattern){low, low} & (pattern){0, infinity}
	= (pattern){low, low} & (pattern)*
*/
NFA_SE makeLowToInfinity(NFA_SE one, int low);

/*
	assume: 0 <= low <= high
	match "one" low~high times.
	like : (pattern){low, high}

	principle:
		(pattern){low, high} = (pattern){low, low} & (pattern){0, high - low}
*/
NFA_SE makeRange(NFA_SE one, int low, int high);

