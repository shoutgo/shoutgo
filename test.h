#ifndef _TEST
#define _TEST

#include "infoboard.h"
#include "brain.h"	
#include "nn.h"
#include "learn.h"
#include "tsumego.h"
#include "fir.h"
#include "inout.h"

/* class TREESEARCH */

class TREESEARCH : public SEARCH<int, NODE, NODE*, bool> {
	vector<NODE*>	findpath(NODE * p, bool) {
		return p->sons; 
	}
	bool	isleaf(NODE * p, bool) {
		return p->isleaf; 
	}
	int		leafvalue(NODE * p, bool){
		return p->value; 
	}
	NODE*	findson(NODE * fatherp, NODE * sonp){
		return sonp; 
	}
	int		upbound() {
		return INT_MAX; 
	}
	int		downbound() {
		return INT_MIN; 
	}
	bool	ismax(NODE * p, bool){
		return p->ismax; 
	}
	bool    control(NODE * p, bool){
		return 0;
	}
	int     recordpath(int i, NODE *){
		return i;
	}
}; 

/* NEWPOS */

class NEWPOS :public POS{
public:
	int		i; 
	void	hash(){}; 
}; 

//����������������������Ӱ��ԭ���룬���Զ��ѿ�
NEWPOS	make_pair(int a, unsigned b); 

/* class TEST */

class TEST {
public:
	U_TEST		tu; 
	TEST_BB		tbb; 
	TEST_GO		tgo; 
	TEST_INFO	tinfo; 
	TEST_IO		tio; 
	TEST_ITR	titr; 
	TEST_S		ts; 
	TEST_TS		tts; 
	TEST_TT		ttt; 
	TEST_NN		tnn; 
	TEST_L		tl; 
	TEST_BR		tbr; 
	TEST_F		tf; 
	TEST_TG		ttg; 
public:
    int&	add100(int& a) const; 
	int		minus5(int& a) const; 
	void	time (string s = ""); 
	void	active(); 
}; 

/* global function */

void testtree(); 
void testnewpos(); 

void t_t_teststatic(); 
void t_teststatic(); 
void teststatic(); 

void testc (); 
void testcpp(); 
void teststl(); 

void testsize(); 
void testload (); 
void testcounter (); 
void testrand (); 
void test_array_if (); 
void testindex8 (); 
void testtransptable(); 

void test5 (); 
void test6 (); 
void test7 (); 

#endif



