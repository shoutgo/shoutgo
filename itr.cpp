#include "itr.h"  
#include "inout.h"

#if		ITR_MACRO == 0
#undef	___TIME
#define ___TIME(code)	{}
#endif

ITR::ITR():pp(make_pair(0, 0)){	
}
ITR::ITR(const BITB& b):bb(b){	
}

////
POS ITR::ioposbegin(){	
	pp.first = 0; 
	pp.second = LEFTEST; 
	return pp; 
}
bool ITR::ioposend(){	
	return pp.first == BS; 
}
POS ITR::ioposnext(){	
	if (pp.second == 1)    
		++pp.first, 
		pp.second <<= (BS-1); 
	else pp.second >>= 1; 
	return pp; 
}
////
POS ITR::posbegin(){	
	pp.first = 0; 
	pp.second = 1; 
	return pp; 
}
bool ITR::posend(){	
	return pp.first == BS; 
}
POS ITR::posnext(){	
	if (pp.second == LEFTEST)    
		++pp.first, 
		pp.second = 1; 
	else pp.second <<= 1; 
	return pp; 
}
////
POS ITR::stonebegin(){	
	for(int i = 0; i<BS; ++i) 
		if (bb.r[i]>0){
			pp.first = i, pp.second = (((bb.r[i]^(bb.r[i]-1))+1)>>1); 
			return pp; 
		}
	return NULL_POS; 
}
bool ITR::stoneend(){	
	return bb == NULL_BB; 
}
POS ITR::stonenext(){	
	bb ^= pp; 
	for(; !posend(); posnext())
		if (bb[pp]) 
			return pp; 
	return NULL_POS; 
}
////
POS ITR::tagbegin(){	
	for(int i = 0; i<BS; ++i) 
		if (bb.r[i]>0){
			pp.first = i, pp.second = (((bb.r[i]^(bb.r[i]-1))+1)>>1); 
			return pp; 
		}
	return NULL_POS; 
}
bool ITR::tagend(){	
	return bb == NULL_BB; 
}
POS ITR::tagnext(){	
	bb ^= bb.blockat(pp); 
	for(int i = 0; i<BS; ++i) 
		if (bb.r[i]>0){
			pp.first = i, pp.second = (((bb.r[i]^(bb.r[i]-1))+1)>>1); 
			return pp; 
		}
	return NULL_POS; 
}
////
BITB ITR::blockbegin(){	
	return 	oneblock = bb.blockat(stonebegin()); 
}

bool ITR::blockend(){	
	return bb == NULL_BB; 
}

BITB ITR::blocknext(){	
	bb ^= oneblock; 
	return oneblock = bb.blockat(stonebegin()); 
}
////
POS ITR::randomposbegin(){	
	for (int i = 0; i<BS*BS; ++i)
		vecpos.push_back (i2pos(i)); 
	srand((unsigned)time(0)); 
	random_shuffle(vecpos.begin (), vecpos.end ()); 
    return vecpos.back (); 
}
bool ITR::randomposend(){	
	return vecpos.empty(); 
}
POS ITR::randomposnext(){	
	vecpos.pop_back (); 
	return vecpos.back (); 
}
////
POS ITR::randomstonebegin(){	
	for(stonebegin(); !stoneend(); stonenext())
		vecpos.push_back (pp); 
	srand((unsigned)time(0)); 
	random_shuffle(vecpos.begin(), vecpos.end ()); 
    return vecpos.back (); 
}
bool ITR::randomstoneend(){	
	return vecpos.empty(); 
}
POS ITR::randomstonenext(){	
	vecpos.pop_back (); 
	return vecpos.back (); 
}
////
POS ITR::randomemptybegin(){	
	bb = ~bb; 
	for(stonebegin(); !stoneend(); stonenext())
		vecpos.push_back (pp); 
	srand((unsigned)time(0)); 
	random_shuffle(vecpos.begin (), vecpos.end ()); 
    return vecpos.back (); 
}
bool ITR::randomemptyend(){	
	return vecpos.empty(); 
}
POS ITR::randomemptynext(){	
	vecpos.pop_back (); 
	return vecpos.back (); 
}
////
/*
map<POS, VB >		ITR::makemapmask(int a, int b){	
	for (POS pos = posbegin(); !posend (); pos = posnext())
		mapmask[pos] = makemask(pos, a, b); 
	return mapmask; 
}
VB ITR::makemask(const POS& p, int a, int b){	//-// 待优化
	vecbb.clear (); 
	for (int i = 0; i<a; ++i)
		for(int j = 0; j<b; ++j){
			bb = strip(p, i, j, a-i-1, b-j-1); 
			if (bb.count () == a*b)
				vecbb.push_back(bb ); //strip(p, i, j, a-i-1, b-j-1)
		}
	if (a != b) {
		swap(a, b); 
		for (int i = 0; i<a; ++i)
			for(int j = 0; j<b; ++j){
			bb = strip(p, i, j, a-i-1, b-j-1); 
			if (bb.count () == a*b)
				vecbb.push_back( bb); //strip(p, i, j, a-i-1, b-j-1)
		}
	}
	return vecbb; 
}
BITB ITR::randommaskbegin(const POS& p, int a, int b){	
	makemask(p, a, b); 
	srand((unsigned)time(0)); 
	random_shuffle(vecbb.begin (), vecbb.end ()); 
    return vecbb.back (); 
}
bool ITR::randommaskend(){	
	return vecbb.empty(); 
}
BITB ITR::randommasknext(){	
	vecbb.pop_back (); 
	return vecbb.back (); 
}
map<POS, VB >	ITR::makejosekimask(int width){	// 其实每个pos只对应一个mask？
	BITB wn = strip(corner(WN), width, width, ES); 
	BITB ws = strip(corner(WS), width, width, EN); 
	BITB en = strip(corner(EN), width, width, WS); 
	BITB es = strip(corner(ES), width, width, WN); 
	for (POS pos = posbegin(); !posend (); pos = posnext()){
		vecbb.clear(); 
		if		(wn[pos]) vecbb.push_back(wn); 
		else if (ws[pos]) vecbb.push_back(ws); 
		else if (en[pos]) vecbb.push_back(en); 
		else if (es[pos]) vecbb.push_back(es); 
		//else            vecbb.push_back(NULL_BB); 
		mapmask[pos] = vecbb; 
	}
	return mapmask; 
}
*/
