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

#endif
