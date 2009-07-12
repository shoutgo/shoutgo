#include "itr.h"  
#include "inout.h"

#if		ITR_MACRO == 0
#undef	___FUNCOUNT
#define ___FUNCOUNT(fun) {}
#undef	___TIME
#define ___TIME(code)	{}
#endif

ITR::ITR():pp(make_pair(0, 0)){	
	___FUNCOUNT( ITR_ITR1); 
}
ITR::ITR(const BITB& b):bb(b){	
	___FUNCOUNT( ITR_ITR2); 
}

////
POS ITR::ioposbegin(){	
	___FUNCOUNT( ITR_IOPOSBEGIN); 
	pp.first = 0; 
	pp.second = LEFTEST; 
	return pp; 
}
bool ITR::ioposend(){	
	___FUNCOUNT( ITR_IOPOSEND); 
	return pp.first == BS; 
}
POS ITR::ioposnext(){	
	___FUNCOUNT( ITR_IOPOSNEXT); 
	if (pp.second == 1)    
		++pp.first, 
		pp.second <<= (BS-1); 
	else pp.second >>= 1; 
	return pp; 
}
////
POS ITR::posbegin(){	
	___FUNCOUNT( ITR_POSBEGIN); 
	pp.first = 0; 
	pp.second = 1; 
	return pp; 
}
bool ITR::posend(){	
	___FUNCOUNT( ITR_POSEND); 
	return pp.first == BS; 
}
POS ITR::posnext(){	
	___FUNCOUNT( ITR_POSNEXT); 
	if (pp.second == LEFTEST)    
		++pp.first, 
		pp.second = 1; 
	else pp.second <<= 1; 
	return pp; 
}
////
POS ITR::stonebegin(){	
	___FUNCOUNT( ITR_STONEBEGIN); 
	for(int i = 0; i<BS; ++i) 
		if (bb.r[i]>0){
			pp.first = i, pp.second = (((bb.r[i]^(bb.r[i]-1))+1)>>1); 
			return pp; 
		}
	return NULL_POS; 
}
bool ITR::stoneend(){	
	___FUNCOUNT( ITR_STONEEND); 
	return bb == NULL_BB; 
}
POS ITR::stonenext(){	
	___FUNCOUNT( ITR_STONENEXT); 
	bb ^= pp; 
	for(; !posend(); posnext())
		if (bb[pp]) 
			return pp; 
	return NULL_POS; 
}
////
POS ITR::tagbegin(){	
	___FUNCOUNT( ITR_ORIGINBEGIN); 
	for(int i = 0; i<BS; ++i) 
		if (bb.r[i]>0){
			pp.first = i, pp.second = (((bb.r[i]^(bb.r[i]-1))+1)>>1); 
			return pp; 
		}
	return NULL_POS; 
}
bool ITR::tagend(){	
	___FUNCOUNT( ITR_ORIGINEND); 
	return bb == NULL_BB; 
}
POS ITR::tagnext(){	
	___FUNCOUNT( ITR_ORIGINNEXT); 
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
	___FUNCOUNT( ITR_BLOCKBEGIN); 
	return 	oneblock = bb.blockat(stonebegin()); 
}

bool ITR::blockend(){	
	___FUNCOUNT( ITR_BLOCKEND); 
	return bb == NULL_BB; 
}

BITB ITR::blocknext(){	
	___FUNCOUNT( ITR_BLOCKNEXT); 
	bb ^= oneblock; 
	return oneblock = bb.blockat(stonebegin()); 
}
////
POS ITR::randomposbegin(){	
	___FUNCOUNT( ITR_RANDOMPOSBEGIN); 
	for (int i = 0; i<BS*BS; ++i)
		vecpos.push_back (i2pos(i)); 
	srand((unsigned)time(0)); 
	random_shuffle(vecpos.begin (), vecpos.end ()); 
    return vecpos.back (); 
}
bool ITR::randomposend(){	
	___FUNCOUNT( ITR_RANDOMPOSEND); 
	return vecpos.empty(); 
}
POS ITR::randomposnext(){	
	___FUNCOUNT( ITR_RANDOMPOSNEXT); 
	vecpos.pop_back (); 
	return vecpos.back (); 
}
////
POS ITR::randomstonebegin(){	
	___FUNCOUNT( ITR_RANDOMSTONEBEGIN); 
	for(stonebegin(); !stoneend(); stonenext())
		vecpos.push_back (pp); 
	srand((unsigned)time(0)); 
	random_shuffle(vecpos.begin(), vecpos.end ()); 
    return vecpos.back (); 
}
bool ITR::randomstoneend(){	
	___FUNCOUNT( ITR_RANDOMSTONEEND); 
	return vecpos.empty(); 
}
POS ITR::randomstonenext(){	
	___FUNCOUNT( ITR_RANDOMSTONENEXT); 
	vecpos.pop_back (); 
	return vecpos.back (); 
}
////
POS ITR::randomemptybegin(){	
	___FUNCOUNT( ITR_RANDOMEMPTYBEGIN); 
	bb = ~bb; 
	for(stonebegin(); !stoneend(); stonenext())
		vecpos.push_back (pp); 
	srand((unsigned)time(0)); 
	random_shuffle(vecpos.begin (), vecpos.end ()); 
    return vecpos.back (); 
}
bool ITR::randomemptyend(){	
	___FUNCOUNT( ITR_RANDOMEMPTYEND); 
	return vecpos.empty(); 
}
POS ITR::randomemptynext(){	
	___FUNCOUNT( ITR_RANDOMEMPTYNEXT); 
	vecpos.pop_back (); 
	return vecpos.back (); 
}
////
/*
map<POS, VB >		ITR::makemapmask(int a, int b){	
	___FUNCOUNT( ITR_MAKEMAPMASK); 
	for (POS pos = posbegin(); !posend (); pos = posnext())
		mapmask[pos] = makemask(pos, a, b); 
	return mapmask; 
}
VB ITR::makemask(const POS& p, int a, int b){	//-// 待优化
	___FUNCOUNT( ITR_MAKEMASK); 
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
	___FUNCOUNT( ITR_RANDOMMASKBEGIN); 
	makemask(p, a, b); 
	srand((unsigned)time(0)); 
	random_shuffle(vecbb.begin (), vecbb.end ()); 
    return vecbb.back (); 
}
bool ITR::randommaskend(){	
	___FUNCOUNT( ITR_RANDOMMASKEND); 
	return vecbb.empty(); 
}
BITB ITR::randommasknext(){	
	___FUNCOUNT( ITR_RANDOMMASKNEXT); 
	vecbb.pop_back (); 
	return vecbb.back (); 
}
map<POS, VB >	ITR::makejosekimask(int width){	// 其实每个pos只对应一个mask？
	___FUNCOUNT( ITR_MAKEJOSEKIMASK); 
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
/* _______________________________ 测试区 _______________________________ */

void TEST_ITR::timefunc (string which) {
	RANDER  r; 
	
	ITR itr = ITR(r.vb[0]); 

    ___TIME( ITR_IOPOSBEGIN, itr.ioposbegin(); ); 
	___TIME( ITR_IOPOSNEXT, itr.ioposnext(); ); 
	___TIME( ITR_IOPOSEND, itr.ioposend(); ); 

    ___TIME( ITR_POSBEGIN, itr.posbegin(); ); 
	___TIME( ITR_POSNEXT, itr.posnext(); ); 
	___TIME( ITR_POSEND, itr.posend(); ); 

    ___TIME( ITR_STONEBEGIN, itr.stonebegin(); ); 
	___TIME( ITR_STONENEXT, itr.stonenext(); ); 
	___TIME( ITR_STONEEND, itr.stoneend(); ); 

	___TIME( ITR_ORIGINBEGIN, itr.tagbegin(); ); 
	//___TIME( ITR_ORIGINNEXT, itr.tagnext(); ); 
	___TIME( ITR_ORIGINEND, itr.tagend(); ); 
    
	___TIME( ITR_BLOCKBEGIN, itr.blockbegin(); ); 
	___TIME( ITR_BLOCKNEXT, itr.blocknext(); ); 
	___TIME( ITR_BLOCKEND, itr.blockend(); ); 

	___TIME( ITR_RANDOMPOSBEGIN, itr.randomposbegin(); ); 
	___TIME( ITR_RANDOMPOSEND, itr.randomposend(); ); 
	___TIME( ITR_RANDOMPOSNEXT, itr.randomposnext(); ); 

	___TIME( ITR_RANDOMSTONEBEGIN, itr.randomstonebegin(); ); 
	___TIME( ITR_RANDOMSTONEEND, itr.randomstoneend(); ); 
	___TIME( ITR_RANDOMSTONENEXT, itr.randomstonenext(); ); 

	___TIME( ITR_RANDOMEMPTYBEGIN, itr.randomemptybegin(); ); 
	___TIME( ITR_RANDOMEMPTYEND, itr.randomemptyend(); ); 
	___TIME( ITR_RANDOMEMPTYNEXT, itr.randomemptynext(); ); 
	/*
	___TIME( ITR_MAKEMASK, itr.makemask(r.vp[0], r.vi[0], r.vi[1]); ); 
	___TIME( ITR_MAKEMAPMASK, itr.makemapmask(r.vi[0], r.vi[1]); ); 
	___TIME( ITR_MAKEJOSEKIMASK, itr.makejosekimask(r.vi[0]); ); 

	___TIME( ITR_RANDOMMASKBEGIN, itr.randommaskbegin(r.vp[0], r.vi[0], r.vi[1]); ); 
	___TIME( ITR_RANDOMMASKEND, itr.randommaskend(); ); 
	___TIME( ITR_RANDOMMASKNEXT, itr.randommasknext(); ); 
	*/
}

