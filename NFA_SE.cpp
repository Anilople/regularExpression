#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <map>
#include "NFA_SE.h"
NFA_SE NFA_SE_init(NFA *start, NFA *end)
{
	NFA_SE temp;
	temp.start = start;
	temp.end = end;
	return temp;
}

NFA_SE NFA_SE_init_edge(char value)
{
	NFA *start = (NFA *)malloc(sizeof(NFA));
	NFA *end = (NFA *)malloc(sizeof(NFA));
	NFA_init(start, end, NULL, value, '\0');
	NFA_init(end, NULL, NULL, '\0', '\0');
	return NFA_SE_init(start, end);
}

int isMatchNFA_SE(const char *str, NFA_SE nfa_se)
{
	return isMatchNFA(str, nfa_se.start);
}

//static void freeNFA_SE_dfs(std::set<NFA*> &mark, NFA *now)
//{
//	if (NULL == now || mark.count(now) > 0) {
//		return;
//	}
//	else {
//
//	}
//	mark.insert(now);
//	freeNFA_SE_dfs(mark, now->next0);
//	freeNFA_SE_dfs(mark, now->next1);
//	free(now);
//}
//
//static void freeNFA_SE_recursive(NFA_SE nfa_se)
//{
//	std::set<NFA*> mark;
//	freeNFA_SE_dfs(mark, nfa_se.start);
//}

/*
	get relationship of old node and new node
*/
static void getOld2New(NFA *oldNow, std::map<NFA*, NFA*> &old2new)
{
	if (NULL == oldNow || old2new.count(oldNow) > 0) {
		return;
	}
	else {

	}

	NFA *newNow = getNewNFA(oldNow);
	old2new[oldNow] = newNow;
	getOld2New(oldNow->next0, old2new);
	getOld2New(oldNow->next1, old2new);
}

static void buildNewAutoma(NFA *oldNow, std::set<NFA*> &mark, const std::map<NFA*, NFA*> &old2new)
{
	if (NULL == oldNow || mark.count(oldNow) > 0) {
		return;
	}
	else {

	}

	mark.insert(oldNow);

	NFA *newNow = old2new.find(oldNow)->second;
	// look what oldNode point to
	NFA *oldNext0 = oldNow->next0;
	if (NULL != oldNext0) {
		NFA *newNext0 = old2new.find(oldNext0)->second;
		newNow->next0 = newNext0;
	}
	else {

	}

	NFA *oldNext1 = oldNow->next1;
	if (NULL != oldNext1) {
		NFA *newNext1 = old2new.find(oldNext1)->second;
		newNow->next1 = newNext1;
	}
	else {

	}

	buildNewAutoma(oldNow->next0, mark, old2new);
	buildNewAutoma(oldNow->next1, mark, old2new);
}

NFA_SE getNewNFA_SE_deepClone(NFA_SE nfa_se)
{
	std::map<NFA*, NFA*> old2new;
	getOld2New(nfa_se.start, old2new);
	std::set<NFA*> mark;
	buildNewAutoma(nfa_se.start, mark, old2new);
	NFA* newStart = old2new.find(nfa_se.start)->second;
	NFA* newEnd = old2new.find(nfa_se.end)->second;
	return NFA_SE_init(newStart, newEnd);
}

NFA_SE getLambda()
{
	return NFA_SE_init_edge('\0');
}

NFA_SE makeOR(NFA_SE a, NFA_SE b)
{
	NFA_SE newA = getNewNFA_SE_deepClone(a);
	NFA_SE newB = getNewNFA_SE_deepClone(b);
	// waste memory here, do not use free
	//freeNFA_SE_recursive(a);
	//freeNFA_SE_recursive(b);
	a = newA;
	b = newB;


	NFA *newStar = (NFA *)malloc(sizeof(NFA));
	// newStar -> a's start (lambda transition)
	// newStar -> b's start (lambda transition)
	NFA_init(newStar, a.start, b.start, '\0', '\0');

	NFA *newEnd = (NFA *)malloc(sizeof(NFA));
	NFA_init(newEnd, NULL, NULL, '\0', '\0');

	// a's end -> newEnd (lambda transition)
	// b's end -> newEnd (lambda transition)
	NFA *aEnd = a.end;
	NFA_init(aEnd, newEnd, NULL, '\0', '\0');
	NFA *bEnd = b.end;
	NFA_init(bEnd, newEnd, NULL, '\0', '\0');

	return NFA_SE_init(newStar, newEnd);
}

NFA_SE makeAND(NFA_SE a, NFA_SE b)
{
	NFA_SE newA = getNewNFA_SE_deepClone(a);
	NFA_SE newB = getNewNFA_SE_deepClone(b);
	// waste memory here, do not use free
	//freeNFA_SE_recursive(a);
	//freeNFA_SE_recursive(b);
	a = newA;
	b = newB;

	NFA *newStar = a.start;

	NFA *aEnd = a.end;
	NFA *bStar = b.start;
	// a's end -> b's start (lambda transition)
	NFA_init(aEnd, bStar, NULL, '\0', '\0');
	NFA *newEnd = b.end;

	return NFA_SE_init(newStar, newEnd);
}

NFA_SE makeStar(NFA_SE a)
{
	a = getNewNFA_SE_deepClone(a);

	NFA *newStar = (NFA *)malloc(sizeof(NFA));
	NFA_init(newStar, NULL, NULL, '\0', '\0');
	NFA *newEnd = (NFA *)malloc(sizeof(NFA));
	NFA_init(newEnd, NULL, NULL, '\0', '\0');

	// newStar -> a's start (lambda transition)
	// newStar -> newEnd    (lambda transition)
	NFA_init(newStar, a.start, newEnd, '\0', '\0');

	// a's end -> a's start (lambda transition)
	// a's end -> newEnd    (lambda transition)
	NFA_init(a.end, a.start, newEnd, '\0', '\0');

	return NFA_SE_init(newStar, newEnd);
}


NFA_SE makeOR_Array(NFA_SE *arr, int len)
{
	NFA_SE temp = makeOR(arr[0], arr[1]);
	for (int i = 2; i < len; i++) {
		temp = makeOR(arr[2], temp);
	}
	return temp;
}

NFA_SE makeAND_Array(NFA_SE *arr, int len)
{
	NFA_SE temp = makeAND(arr[0], arr[1]);
	for (int i = 2; i < len; i++) {
		temp = makeAND(arr[2], temp);
	}
	return temp;
}

NFA_SE makeAND_repeat(NFA_SE one, int n)
{
	if (n < 0) {
		printf("repeat < 0, warning!.\n");
	}
	else {

	}

	if (0 == n) {
		return getLambda();
	}
	else {
		NFA_SE concat = one;
		for (int i = 1; i < n; i++) {
			concat = makeAND(one, concat);
		}
		return concat;
	}

}

NFA_SE makeRangeToHigh(NFA_SE one, int high)
{
	if (high < 0) {
		printf("high value is %d, it < 0, warning!.\n", high);
		return NFA_SE_init(NULL, NULL);
	}
	else {

	}

	NFA_SE lambda = getLambda();
	if (0 == high) {
		return lambda;
	}
	else {

		return makeAND(makeOR(one, lambda), makeRangeToHigh(one, high - 1));
	}
}

NFA_SE makeLowToInfinity(NFA_SE one, int low)
{
	if (low < 0) {
		printf("warning! low < 0\n");
	}
	else {

	}

	NFA_SE lowTimes = makeAND_repeat(one, low);
	NFA_SE star = makeStar(one);
	return makeAND(lowTimes, star);
}

NFA_SE makeRange(NFA_SE one, int low, int high)
{
	if (low < 0) {
		printf("warning! low < 0\n");
	}
	else {

	}

	if (low > high) {
		printf("low > high\n");
	}
	else {

	}

	NFA_SE lowTimes = makeAND_repeat(one, low);
	NFA_SE zeroToHigh = makeRangeToHigh(one, high - low);
	return makeAND(lowTimes, zeroToHigh);
}