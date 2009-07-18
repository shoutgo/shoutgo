#ifndef _TSUMEGO
#define _TSUMEGO

#include "infoboard.h"

class TSUMEGO:public INFOGO{
public:
	PBB	randomeye(
		int, 
		int, 
		MODE_RANDOMEYE mode = LESS_LESS
		); 
	void randomtsumgo(
		int publicliberty, 
		int, 
		int, 
		MODE_RANDOMEYE mode = LESS_LESS
		); 
	pair<EYEKIND, VP >	tsumego(
		COLOR sente = BLACK
		); 
	PBB	update_RESCUE(
		POS pos, 
		COLOR sente, 
		BITB space, BITB enemy
		); 
	PBB	update_KILL(
		POS pos, 
		COLOR sente, 
		BITB space, BITB enemy
		); 
	pair<EYEKIND, VP >	eyekind_RESCUE(
		COLOR sente, 
		BITB space, 
		BITB enemy, 
		EYEKIND passbest = UNKOWN_WHENPASS
		); 
	pair<EYEKIND, VP >	eyekind_KILL(
		COLOR sente, 
		BITB space, 
		BITB enemy, 
		EYEKIND passbest = UNKOWN_WHENPASS
		); 

		// 这两个函数以前在 BITB 里，应该移过来，
		// 不过暂未改造其实现部分
	pair<EYEKIND, VP >	eyekind(
		ACTION action, 
		BITB enemy, 
		EYEKIND passbest = UNKOWN_WHENPASS
		); 
	PBB	update(
		ACTION action, 
		BITB enemy, 
		POS pos
		); 

	GO		snap() const; 
	void	restore(const GO&); 

}; 

#endif
