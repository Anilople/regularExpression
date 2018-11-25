#include "NFA.h"
#include <stdio.h>
#include <stdlib.h>

void printNFADot(NFA *nfa)
{
	if (NULL == nfa) {
		return;
	}
	else {

	}

	if (nfa->next0) {
		printf("\"%x\" -> \"%x\" [ label = \"%c\"]\n", nfa, nfa->next0, nfa->edgeValue0);
	}
	else {

	}

	if (nfa->next1) {
		printf("\"%x\" -> \"%x\" [ label = \"%c\"]\n", nfa, nfa->next1, nfa->edgeValue1);
	}
	else {

	}
}

void printNFADotRecursive(NFA *nfa)
{
	if (NULL == nfa) {
		return;
	}
	else {

	}

	printNFADot(nfa);
	NFA *next0 = nfa->next0;
	NFA *next1 = nfa->next1;
	// delete edge to forbid cycle
	nfa->next0 = NULL;
	nfa->next1 = NULL;
	printNFADotRecursive(next0);
	printNFADotRecursive(next1);
}

void NFA_init(NFA *nfa, NFA *next0, NFA *next1, char edgeValue0, char edgeValue1)
{
	nfa->next0 = next0;
	nfa->next1 = next1;
	nfa->edgeValue0 = edgeValue0;
	nfa->edgeValue1 = edgeValue1;
}

NFA* getNewNFA(NFA* nfa)
{
	NFA* newNfa = (NFA*)malloc(sizeof(NFA));
	NFA_init(newNfa, nfa->next0, nfa->next1, nfa->edgeValue0, nfa->edgeValue1);
	return newNfa;
}

int hasNext(NFA *next, char edgeValue)
{
	return NULL != next;
}

int isLambdaEdge(NFA *next, char edgeValue)
{
	return '\0' == edgeValue;
}

char getEdgeValue(NFA *next, char edgeValue)
{
	return edgeValue;
}

int isMatchNFA(const char *str, const NFA *nfa)
{
	if (NULL == nfa) {
		// even the end node is not NULL
		// so if node is NULL, it's not match
		return false;
	}
	else {

	}

	char nowValue = *str;
	const char *nextStrAddr = str + 1;


	if (NULL == nfa->next0 && NULL == nfa->next1) {
		return '\0' == nowValue;
	}
	else {

	}

	int match = 0;

	// next0 branch
	if (hasNext(nfa->next0, nfa->edgeValue0)) {
		if (isLambdaEdge(nfa->next0, nfa->edgeValue0)) {
			// lambda transition
			match |= isMatchNFA(str, nfa->next0);
		}
		else {
			// is not lambda transition
			// must match edge now
			if (nowValue == getEdgeValue(nfa->next0, nfa->edgeValue0)) {
				// match edge not
				match |= isMatchNFA(nextStrAddr, nfa->next0);
			}
			else {
				// not match edge not
				match |= false;
			}
		}
	}
	else {

	}

	// next1 branch
	if (hasNext(nfa->next1, nfa->edgeValue1)) {
		if (isLambdaEdge(nfa->next1, nfa->edgeValue1)) {
			// lambda transition
			match |= isMatchNFA(str, nfa->next1);
		}
		else {
			// is not lambda transition
			// must match edge now
			if (nowValue == getEdgeValue(nfa->next1, nfa->edgeValue1)) {
				// match edge not
				match |= isMatchNFA(nextStrAddr, nfa->next1);
			}
			else {
				// not match edge not
				match |= false;
			}
		}
	}
	else {

	}

	if (NULL == nfa->next0 && NULL == nfa->next1) {
		return '\0' == nowValue;
	}
	else {
		return match;
	}
}