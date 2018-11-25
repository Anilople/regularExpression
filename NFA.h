#pragma once

/*
	nondeterministic finite automa
	use edge value '\0' denote lambda transition
	when nexti is NULL, edgeValuei is meanless
*/
typedef struct NFA
{
	NFA *next0;
	NFA *next1;
	char edgeValue0;
	char edgeValue1;
}NFA;

void printNFADot(NFA *nfa);
/*
	to forbid recursive forever,
	this function will break data structure
*/
void printNFADotRecursive(NFA *nfa);
void NFA_init(NFA *nfa, NFA *next0, NFA *next1, char edgeValue0, char edgeValue1);

/*
	copy a new NFA and return it's pointer
*/
NFA* getNewNFA(NFA *nfa);

int hasNext(NFA *next, char edgeValue);
int isLambdaEdge(NFA *next, char edgeValue);
char getEdgeValue(NFA *next, char edgeValue);

/*
	use depth first search
*/
int isMatchNFA(const char *str, const NFA *nfa);