#ifndef _FIRBOARD
#define _FIRBOARD

#include "infoboard.h"

/* FIR : Four or Five In a Row */

class FIR :public INFOGO {
public:
	static const int	FIRNUM; 
	static const int	FIR_MAX; 
	int			nodenum; 
	VP			vecp; 
public:
	FIR():nodenum(0){}
public:
	PIP			minmax(int n); 
	PIP			alphabeta(int n, int alpha, int beta); 
	void		scope(int = 5); 

	PIP			leafvalue(); 
	bool		over(); 

	void		init(){}		
	void		update(){}  	
	POS			genmove(); 
	bool		moveable(POS pos, COLOR clr); 
	bool		move(POS pos, COLOR clr); 

	GO			snap() const; 
	void		restore(const GO&); 

}; 

/*inline*/ bool		ppless(const PIP & m1, const PIP & m2); 
/*inline*/ VI		link4(BITB b, POS p); 
/*inline*/ int		link(BITB b, POS p); 

/* _______________________________ ²âÊÔÇø _______________________________ */

enum	F_FUN	{F_FIR = FF*FUN_MAX, F_FUN_SIZE}; 

class TEST_F {
public:
	void assertall(); 
	void timefunc(string which = ""); 
	void active(){ 
	}  
}; 

#endif
