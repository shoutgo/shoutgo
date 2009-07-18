#ifndef NN_
#define NN_

#include "define.h"     

class N3WEIGHT{
public:
	float	fitness; 
	VVF		w1; 
	VVF		w2; 
	VF		w3; 
public:
	N3WEIGHT(){}; 
	bool	operator<  (const N3WEIGHT& w) const { 
		return fitness<w.fitness; 
	}
	bool	operator == (const N3WEIGHT& w) const { 
		return ((fitness == w.fitness)
			&&(w1 == w.w1 )
			&&(w2 == w.w2)
			&&(w3 == w.w3 )); 
	}
}; 

class NN{
public:
	VVF			datayx; 
public:
	VVF 		producedata(int m, int n); 
	VF			linear_genetic(); 
	float		linearmodal(VF w); 
	N3WEIGHT	perception(N3WEIGHT w); 
	void		perception_genetic(); 
}; 

#endif
