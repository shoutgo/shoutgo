#ifndef _LEARN
#define _LEARN

#include "infoboard.h"						
#include "nn.h"

/* class SGFNODE, SGFROOT */

struct SGFNODE{
	POS					pos; 
	COLOR               color; 
	SGFNODE*			father; 
	vector<SGFNODE*>	sons; 
	~SGFNODE(); 
}; 

struct SGFROOT{
	string				intro; 
	VP					xfuseki; 
	VP					ofuseki; 
	COLOR				turn; 
	vector<SGFNODE*>	sons; 
	~SGFROOT(); 

template <class T>
	void fuseki (T* t_ptr) const {
		for (int i = 0; i<xfuseki.size (); ++i){
			(*t_ptr).move(xfuseki[i], BLACK); 
			(*t_ptr).update(); 
		}
		for (int i = 0; i<ofuseki.size (); ++i){
			(*t_ptr).move(ofuseki[i], WHITE); 
			(*t_ptr).update(); 
		}
		// 因存在 sgfnode->color ，并不需要下一句。
		// t_ptr->setturn(turn); 
	}
}; 

/* class LEARN */

class LEARN	{			

	typedef void (LEARN::*FPTR)(INFOGO*); 

public:
	FSM<KEY>	lib_zobrist[ZOBRIST::ZOBRIST_KIND]; 
	FSM<ULL>	lib_pemis; 
	int	n_zobrist;
	int n_pemis;

public:
	LEARN(string sgfpath = SGFPATH); 
	~LEARN(); 

	void learn(VS vfilename); 
	void visit(SGFNODE* p, INFOGO* infop, FPTR f); 
	void insert_zobrist(INFOGO* infop); 
	void insert_pemis(INFOGO* infop); 

	vector<pair<ULL, ULL> > pemis(INFOGO* infop); 
	void setpatfreq(INFOGO* infop); 

}; 

/* _______________________________ 测试区 _______________________________ */

const enum	L_FUN	{
	L_LEARN = FL* FUN_MAX, L_FUN_SIZE}; 

class TEST_L {
public:
	void assertall(); 
	void timefunc(string which = ""); 
	void lib_zobrist(); 
	void joseki(); 
	void active(){
		lib_zobrist(); 
	}
}; 

#endif








