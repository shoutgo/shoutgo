#include "board.h"
#include "itr.h"
#include "inout.h"
#include <unistd.h>	// sleep()		// VS2008

GO::GO( )
:xx(NULL_BB), 
oo(NULL_BB), 
hotko(NULL_POS), 
history(0), 
lastpos(NULL_POS), 
lastclr(WHITE), 
kill(NULL_BB){  
}

GO::GO(const BITB& x, const BITB& o, const BITB& d, 
	   const POS& k, const POS& l, COLOR c, int h)
:xx(x), 
oo(o), 
kill(d), 
hotko(k), 
lastpos(l), 
lastclr(c), 
history(h){ 
}

GO::GO(const PBB& pbb, const BITB& d, 
	   const POS& k, const POS& l, COLOR c, int h)
:xx(pbb.first), 
oo(pbb.second), 
kill(d), 
hotko(k), 
lastpos(l), 
lastclr(c), 
history(h){  
}

GO::GO(const GO& go)
:xx(go.xx), 
oo(go.oo), 
hotko(go.gethotko() ), 
history(go.gethistory() ), 
lastpos(go.getlastpos() ), 
lastclr(go.getlastclr() ), 
kill(go.getkill()){ 
}

void GO::random(MODE_RANDOMBOARD mode) {  
	switch (mode) {
		case ILLEGALIZE:													
			xx.random (); 
			sleep(50); 
			oo.random (); 
			___ASSERT(xx != oo); 
			break; 
		case LEGALIZE:
			random(ILLEGALIZE); 
			legalize(); 
			break; 
		default:; 
	}
}

void GO::legalize(){  
	BITB repeat = xx&oo; 
	xx ^= repeat; 
	oo ^= repeat; 
	ITR itr(oo); 
	for (POS pos = itr.tagbegin(); !itr.tagend(); pos = itr.tagnext())
		if (libertysite(pos).count() == 0) 
			oo ^= oo.blockat(pos); 
	itr = ITR(xx); 
	for (POS pos = itr.tagbegin(); !itr.tagend(); pos = itr.tagnext())
		if (libertysite(pos).count() == 0) 
			xx ^= xx.blockat(pos); 
}

void GO::add(const POS& pos, COLOR clr) {  
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 
	if (clr == BLACK)   
		xx |= pos; 
	else 
		oo |= pos; 
    ++history; 
	lastpos = pos; 
	lastclr = clr; 
}
/*
GO	 GO::snap() const {  
	return *this; 
}

void	 GO::restore(const GO& go){ 
	*this = go; 
}
*/
GO	 GO::transpose() const {  
	return GO(xx.transpose (), 
		oo.transpose (), 
		kill.transpose (), 
		transp(hotko), 
		transp(lastpos), 
		lastclr, 
		history); 
}

GO    GO::rotate(int angle) const {
	return GO(xx.rotate(angle), 
		oo.rotate(angle), 
		kill.rotate(angle), 
		rot(angle, hotko), 
		rot(angle, lastpos), 
		lastclr, 
		history); 
}

COLOR    GO::color(const POS& pos)  const {  
	___PARASSERT( !outside(pos) ); 
	if (oo[pos]) {
		if (xx[pos]) 
			return BLACK_WHITE; 
		else           
			return WHITE; 
	}
	else if (xx[pos])    
		return BLACK; 
	if (pos == hotko) 		    
		return HOTKO; 
	return EMPTY; 
}

COLOR	GO::turn( )	const { 
	return ~lastclr; 
}

COLOR	GO::getlastclr( )	const { 
	return lastclr; 
}

POS		GO::getlastpos( )	const {
	return  lastpos; 
}

BITB	GO::getbb(COLOR clr) const {  
	___PARASSERT(clr != BLACK_WHITE); 
	switch (clr) {
		case BLACK:
			return xx; 
		case WHITE:
			return oo; 
		case EMPTY:
			return NULL_BB; 
		default:
			return NULL_BB; 
	}
}

int		GO::gethistory() const {
	return history; 
}

POS		GO::gethotko() const {
	return hotko; 
}

BITB	GO::getkill() const {
	return kill; 
}

////
BLOCK	 GO::expand(const BLOCK& b, COLOR clr) const {  
	BITB ex; 
	BITB otherbb = getbb( ~clr ); 
	ROW tmp = (b.r[0]|b.r[0]<<1|b.r[0]>>1|b.r[1]); 
	ex.r[0] = tmp^(tmp&otherbb.r[0]); 
	for (int i = 1; i<BS-1; ++i) {
		tmp = ( b.r[i] | b.r[i]>>1 | b.r[i]<<1 | b.r[i-1] | b.r[i+1] ); 
		ex.r[i] = tmp^(tmp&otherbb.r[i]); 
	}
	tmp = ( b.r[BS-1] | b.r[BS-1]<<1 | b.r[BS-1]>>1 | b.r[BS-2] ); 
	ex.r[BS-1] = tmp^(tmp&otherbb.r[BS-1]); 
	return ex&ROWMASK; 
}

// 两者应该无重合，放在 BITB 中不合适
BITB GO::enemy_(const BLOCK& blk, COLOR clr) const {	
	___PARASSERT(blk.ismassof(getbb(clr))); 
	return (getbb(~clr)|blk).blockon(blk)^blk; 
}

// 与 libertysite() 功能互补
BITB GO::enemysite(const BLOCK& blk, COLOR clr) const {			
	___PARASSERT(blk.ismassof(getbb(clr))); 
	return blk.dilate (1) & getbb(~clr); 
}

BITB GO::libertysite(const POS& pos) const {  
	COLOR clr = color(pos); 
	if (clr != BLACK && clr != WHITE) 
		return NULL_BB; 
	BLOCK blk = getbb(clr).blockat(pos); 
	return expand(blk, clr) ^ blk; 
}

int		 GO::liberty_(const POS& pos) const { 
	if (color(pos) != BLACK && color(pos) != WHITE) 
		return 0; //-// -1
	return libertysite(pos).count(); 
}

/* 展开版之 movealbe() */

bool	 GO::moveable_(const POS& pos, COLOR clr)  const {  
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 
	if (color(pos) != EMPTY)
		return 0; 

	/* 计算量最少 */

	POS		pN, pS, pE, pW; 
	COLOR   cN, cS, cE, cW; 
	pN = make_pair(pos.first-1, pos.second); 
	cN = color(pN); 
	if (cN == EMPTY) 
		return 1; 
	pS = make_pair(pos.first+1, pos.second); 
	cS = color(pS); 
	if (cS == EMPTY) 
		return 1; 
	pE = make_pair(pos.first, pos.second>>1); 
	cE = color(pE); 
	if (cE == EMPTY) 
		return 1; 
	pW = make_pair(pos.first, pos.second<<1); 
	cW = color(pW); 
	if (cW == EMPTY) 
		return 1; 
	bool	onlyN, onlyS, onlyE, onlyW; 
	onlyN = libertysite(pN).only (pos); 
	if ( ((cN == clr) && !onlyN) || ((cN != clr) && onlyN) )
		return 1; 
	onlyS = libertysite(pS).only (pos); 
	if ( ((cS == clr) && !onlyS) || ((cS != clr) && onlyS) )
		return 1; 
	onlyE = libertysite(pE).only (pos); 
	if ( ((cE == clr) && !onlyE) || ((cE != clr) && onlyE) )
		return 1; 
	onlyW = libertysite(pW).only (pos); 
	if ( ((cW == clr) && !onlyW) || ((cW != clr) && onlyW) )
		return 1; 
	return 0; 

	/* 逻辑最清晰 */

	/*
	// 有气
	if (cN == EMPTY || cS == EMPTY || cE == EMPTY || cW == EMPTY)        
		return 1; 
	// 可融合成有气的块
	if (   ((cN == clr) && !onlyN)
		|| ((cS == clr) && !onlyS)
		|| ((cE == clr) && !onlyE)
		|| ((cW == clr) && !onlyW)
		)
		return 1; 
	// 可提
	if (   (cN != clr) && onlyN 
		|| (cS != clr) && onlyS 
		|| (cE != clr) && onlyE
		|| (cW != clr) && onlyW
		) 
		return 1; 
	// 自杀
	return 0; 
	*/

	/* 代码最简洁 */

	/*
	return (   cN == EMPTY || (cN != clr && onlyN) || (cN == clr && !onlyN)
			|| cS == EMPTY || (cS != clr && onlyS) || (cS == clr && !onlyS)
			|| cE == EMPTY || (cE != clr && onlyE) || (cE == clr && !onlyE)
			|| cW == EMPTY || (cW != clr && onlyW) || (cW == clr && !onlyW)); 
	*/
}

/* 未展开版 moveable() */

bool GO::moveable (const POS& pos, COLOR clr) const {   
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 
	
	if ( color(pos) != EMPTY ) 
		return 0; 
	VP	 near4p = near4(pos); 
	for (int i = 0; i < near4p.size (); ++i) {
		COLOR c = color(near4p[i]); 
		// 有气
		if (c == EMPTY)
			return 1; 
		// 可提
		BITB blk = libertysite(near4p[i]); 
		if ((c != clr) && blk.only(pos) ) 
			return 1; 
		// 可融合成有气的块
		else if (( c == clr) && blk.count() > 1)
			return 1; 
	}
	// 自杀
	return 0; 
}

/* 调用版 move() */

bool GO::move_(const POS& pos, COLOR clr) {   
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 

	if (!moveable(pos, clr)) 
		return 0; 
	// 更新 kill
	BITB otherbb = getbb( ~clr ); 
	VP near4p = near4(pos); 
	kill = NULL_BB; 
	for (int i = 0; i < near4p.size (); ++i) {
		if (( color(near4p[i]) != clr) 
			&& libertysite(near4p[i]).only(pos) ) {
			hotko = near4p[i], 
			kill |= otherbb.blockat ( near4p[i]); 
		}
	}
	// 更新 ...
	if (clr == BLACK)  
		xx |= pos, oo ^= kill; 
	else  
		oo |= pos, xx ^= kill; 
	++history; 
	lastpos = pos; 
	lastclr = clr; 
	// 更新劫点
	if (kill.count() == 1){
		BITB tmpbb = getbb(clr).blockat(pos); 
		if (tmpbb.count() == 1 
			&& (expand(tmpbb, clr)^tmpbb).count() == 1)   
; 
		else
			hotko = NULL_POS; 
	}
	else
		hotko = NULL_POS; 

	return 1; 
}

/* 展开之独立版 move() */

bool GO::move__(const POS& pos, COLOR clr) {   
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 

	if (color(pos) != EMPTY)
		return 0; 

	POS		pN, pS, pE, pW; 
	COLOR   cN, cS, cE, cW; 
	bool	onlyN, onlyS, onlyE, onlyW, mvable = 0; 
	BITB	otherbb = getbb(~clr), tmpbb = NULL_BB; 
	
	pN = make_pair(pos.first-1, pos.second); 
	pS = make_pair(pos.first+1, pos.second); 
	pE = make_pair(pos.first, pos.second>>1); 
	pW = make_pair(pos.first, pos.second<<1); 
	cN = color(pN); 
	cS = color(pS); 
	cE = color(pE); 
	cW = color(pW); 
	onlyN = libertysite(pN).only (pos); 
	onlyS = libertysite(pS).only (pos); 
	onlyE = libertysite(pE).only (pos); 
	onlyW = libertysite(pW).only (pos); 
	// 有气
	if (cN == EMPTY || cS == EMPTY || cE == EMPTY || cW == EMPTY)        
		mvable = 1; 
	// 可融合成有气的块
	if (   ((cN == clr) && !onlyN)
		|| ((cS == clr) && !onlyS)
		|| ((cE == clr) && !onlyE)
		|| ((cW == clr) && !onlyW)
		)
		mvable = 1; 
	// 可提
	if ((cN != clr) && onlyN) {
		tmpbb |= otherbb.blockat (pN); 
		hotko = pN; 
		mvable = 1; 
	}
	if ((cS != clr) && onlyS) {
		tmpbb |= otherbb.blockat (pS); 
		hotko = pS; 
		mvable = 1; 
	}
	if ((cE != clr) && onlyE) {
		tmpbb |= otherbb.blockat (pE); 
		hotko = pE; 
		mvable = 1; 
	}
	if ((cW != clr) && onlyW) {
		tmpbb |= otherbb.blockat (pW); 
		hotko = pW; 
		mvable = 1; 
	}
	// 自杀
	if (!mvable)
		return 0; 
	else 
		kill = tmpbb; 
	// 更新
	if (clr == BLACK)  
		xx |= pos, oo ^= kill; 
	else  
		oo |= pos, xx ^= kill; 
	++history; 
	lastpos = pos; 
	lastclr = clr; 
	if (kill.count() == 1){
		tmpbb = getbb(clr).blockat(pos); 
		if (tmpbb.count() == 1 
			&& (expand(tmpbb, clr)^tmpbb).count() == 1)   
; 
		else
			hotko = NULL_POS; 
	}
	else
		hotko = NULL_POS; 

	return 1; 
}

/* 独立版 move() */

bool GO::move(const POS& pos, COLOR clr) {   
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 
	
	if ( color(pos) != EMPTY ) 
		return 0; 
	BITB otherbb = getbb( ~clr ); 
	VP	 near4p = near4(pos); 
	BITB tmpbb = NULL_BB; 
	bool mvable = 0; 
	for (int i = 0; i < near4p.size (); ++i) {
		COLOR c = color(near4p[i]); 
		// 有气
		if (c == EMPTY)
			mvable = 1; 
		BITB blk = libertysite(near4p[i]); 
		// 可提
		if ((c != clr) && blk.only(pos) ) {
			mvable = 1; 
			hotko = near4p[i]; 
			tmpbb |= otherbb.blockat ( near4p[i]); 
		}
		// 可融合成有气的块
		else if (( c == clr) && blk.count() > 1)
			mvable = 1; 
	}
	// 自杀
	if (!mvable)
		return 0; 
	else 
		kill = tmpbb; 
	// 更新
	if (clr == BLACK)  
		xx |= pos, oo ^= kill; 
	else  
		oo |= pos, xx ^= kill; 
	++history; 
	lastpos = pos; 
	lastclr = clr; 
	if (kill.count() == 1){
		tmpbb = getbb(clr).blockat(pos); 
		if (tmpbb.count() == 1 
			&& (expand(tmpbb, clr)^tmpbb).count() == 1)   
; 
		else
			hotko = NULL_POS; 
	}
	else
		hotko = NULL_POS; 

	return 1; 
}

bool		GO::endgame( )	const {  
	return history > 200; 
}

POS			GO::genmove() const {
	RANDER rnd; 
	return rnd.vp [0]; 
}

void		GO::init () {
}

void		GO::update () {
}

PUU		GO::pemis3232 (const BITB& patmask) const { 
	PUU a = (xx&patmask).pemis3232 (); 
	PUU b = (oo&patmask).pemis3232 (); 
	PUU c = ((xx|oo)&patmask).pemis3232 (); 
	PUU t; 
	PUU R; 
	t.first = a.first *b.second 
		+ a.second *b.first 
		+ mulhigh32(a.second, b.second ); 
	t.second = a.second *b.second; 
	R.first = t.first *c.second 
		+ t.second *c.first 
		+ mulhigh32(t.second, c.second ); 
	R.second = t.second *c.second; 
	return  R; 
}

ULL			GO::pemis64 (const BITB& patmask) const {  
	return  (xx&patmask).pemis64() 
		* (oo&patmask).pemis64() 
		*((xx|oo)&patmask).pemis64(); 
}

COLOR		GO::operator[](const POS& pos) const { 
	return color(pos); 
}

GO			GO::operator()(const BITB& mark, const POS& markp) const{
	GO tmp = *this; 
	tmp.kill = mark; 
	tmp.hotko = markp; 
	return tmp; 
}
GO	GO::operator&   (const GO& go) const {
	return GO(xx&go.xx, oo&go.xx); 
}

GO	GO::operator|   (const GO& go) const {
	return GO(xx|go.xx, oo|go.oo); 
}

GO	GO::operator^   (const GO& go) const {
	return GO(xx^go.xx, oo^go.oo); 
}

int			GO::delta () const {
	return xx.count () - oo.count(); 
}

