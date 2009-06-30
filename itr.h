#ifndef ITR_
#define ITR_

#include "bitboard.h"
//#include "board.h"	

class ITR {
private:
	BITB		bb; 
	POS			pp; 
	BLOCK		oneblock; 
	VP			vecpos; 
	/*
	VB			vecbb; 
	map<POS, VB>	mapmask; 
	*/
public:
	ITR(); 
	ITR(const BITB& b); 
public:
    POS			ioposbegin(); 
	POS			ioposnext(); 
	bool		ioposend(); 

    POS			posbegin(); 
	POS			posnext(); 
	bool		posend(); 

    POS			stonebegin(); 
	POS			stonenext(); 
	bool		stoneend(); 

	POS			tagbegin(); 
	POS			tagnext(); 
	bool		tagend(); 
    
	BITB		blockbegin(); 
	BITB		blocknext(); 
	bool		blockend(); 

	POS			randomposbegin(); 
	bool		randomposend(); 
	POS			randomposnext(); 

	POS			randomstonebegin(); 
	bool		randomstoneend(); 
	POS			randomstonenext(); 

	POS			randomemptybegin(); 
	bool		randomemptyend(); 
	POS			randomemptynext(); 
	/*
	VB			makemask(const POS& p, int width1, int width2); 
	map<POS, VB>	makemapmask(int width1, int width2); 
	map<POS, VB>	makejosekimask(int width = MIDBS -1); 
	BITB		randommaskbegin(const POS& p, int width1, int width2); 
	bool		randommaskend(); 
	BITB		randommasknext(); 
	*/
}; 

/* _______________________________ ²âÊÔÇø _______________________________ */

const enum	ITR_FUN	{
	ITR_ITR1 = FITR*FUN_MAX, 
	ITR_ITR2, ITR_IOPOSBEGIN, ITR_IOPOSNEXT, 
	ITR_IOPOSEND, ITR_POSBEGIN, ITR_POSNEXT, ITR_POSEND, 
	ITR_STONEBEGIN, ITR_STONENEXT, ITR_STONEEND, ITR_ORIGINBEGIN, 
	ITR_ORIGINNEXT, ITR_ORIGINEND, ITR_BLOCKBEGIN, ITR_BLOCKNEXT, 
	ITR_BLOCKEND, ITR_RANDOMPOSBEGIN, ITR_RANDOMPOSEND, 
	ITR_RANDOMPOSNEXT, ITR_RANDOMSTONEBEGIN, ITR_RANDOMSTONENEXT, 
	ITR_RANDOMSTONEEND, ITR_RANDOMEMPTYBEGIN, 
	ITR_RANDOMEMPTYEND, ITR_RANDOMEMPTYNEXT, ITR_MAKEMASK, 
	ITR_MAKEMAPMASK, ITR_MAKEJOSEKIMASK, ITR_RANDOMMASKBEGIN, 
	ITR_RANDOMMASKEND, ITR_RANDOMMASKNEXT, ITR_FUN_SIZE}; 

class TEST_ITR {
public:
	void assertall(); 
	void timefunc(string which = ""); 
	void active(){ 
	} 
}; 

#endif
