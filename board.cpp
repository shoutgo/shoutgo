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
	___FUNCOUNT( GO_GO1); 
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
	___FUNCOUNT( GO_GO2); 
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
	___FUNCOUNT( GO_GO3); 
}

GO::GO(const GO& go)
:xx(go.xx), 
oo(go.oo), 
hotko(go.gethotko() ), 
history(go.gethistory() ), 
lastpos(go.getlastpos() ), 
lastclr(go.getlastclr() ), 
kill(go.getkill()){ 
	___FUNCOUNT( GO_GO4); 
}

void GO::random(MODE_RANDOMBOARD mode) {  
	___FUNCOUNT( GO_RANDOM); 
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
	___FUNCOUNT( GO_LEGALIZE); 
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
	___FUNCOUNT( GO_ADD); 
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
	___FUNCOUNT( GO_SNAP); 
	return *this; 
}

void	 GO::restore(const GO& go){ 
	___FUNCOUNT( GO_RESTORE); 
	*this = go; 
}
*/
GO	 GO::transpose() const {  
	___FUNCOUNT( GO_TRANSPOSE); 
	return GO(xx.transpose (), 
		oo.transpose (), 
		kill.transpose (), 
		transp(hotko), 
		transp(lastpos), 
		lastclr, 
		history); 
}

GO    GO::rotate(int angle) const {
	___FUNCOUNT( GO_ROTATE); 
	return GO(xx.rotate(angle), 
		oo.rotate(angle), 
		kill.rotate(angle), 
		rot(angle, hotko), 
		rot(angle, lastpos), 
		lastclr, 
		history); 
}

COLOR    GO::color(const POS& pos)  const {  
	___FUNCOUNT( GO_COLOR); 
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
	___FUNCOUNT( GO_TURN); 
	return ~lastclr; 
}

COLOR	GO::getlastclr( )	const { 
	___FUNCOUNT( GO_GETLASTCLR); 
	return lastclr; 
}

POS		GO::getlastpos( )	const {
	___FUNCOUNT( GO_GETLASTPOS); 
	return  lastpos; 
}

BITB	GO::getbb(COLOR clr) const {  
	___FUNCOUNT( GO_GETBB); 
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
	___FUNCOUNT( GO_EXPAND); 
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

// ����Ӧ�����غϣ����� BITB �в�����
BITB GO::enemy_(const BLOCK& blk, COLOR clr) const {	
	___PARASSERT(blk.ismassof(getbb(clr))); 
	return (getbb(~clr)|blk).blockon(blk)^blk; 
}

// �� libertysite() ���ܻ���
BITB GO::enemysite(const BLOCK& blk, COLOR clr) const {			
	___PARASSERT(blk.ismassof(getbb(clr))); 
	return blk.dilate (1) & getbb(~clr); 
}

BITB GO::libertysite(const POS& pos) const {  
	___FUNCOUNT( GO_LIBERTYSITE); 
	COLOR clr = color(pos); 
	if (clr != BLACK && clr != WHITE) 
		return NULL_BB; 
	BLOCK blk = getbb(clr).blockat(pos); 
	return expand(blk, clr) ^ blk; 
}

int		 GO::liberty_(const POS& pos) const { 
	___FUNCOUNT( GO_LIBERTY); 
	if (color(pos) != BLACK && color(pos) != WHITE) 
		return 0; //-// -1
	return libertysite(pos).count(); 
}

/* չ����֮ movealbe() */

bool	 GO::moveable_(const POS& pos, COLOR clr)  const {  
	___FUNCOUNT( GO_MOVEABLE); 
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 
	if (color(pos) != EMPTY)
		return 0; 

	/* ���������� */

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

	/* �߼������� */

	/*
	// ����
	if (cN == EMPTY || cS == EMPTY || cE == EMPTY || cW == EMPTY)        
		return 1; 
	// ���ںϳ������Ŀ�
	if (   ((cN == clr) && !onlyN)
		|| ((cS == clr) && !onlyS)
		|| ((cE == clr) && !onlyE)
		|| ((cW == clr) && !onlyW)
		)
		return 1; 
	// ����
	if (   (cN != clr) && onlyN 
		|| (cS != clr) && onlyS 
		|| (cE != clr) && onlyE
		|| (cW != clr) && onlyW
		) 
		return 1; 
	// ��ɱ
	return 0; 
	*/

	/* �������� */

	/*
	return (   cN == EMPTY || (cN != clr && onlyN) || (cN == clr && !onlyN)
			|| cS == EMPTY || (cS != clr && onlyS) || (cS == clr && !onlyS)
			|| cE == EMPTY || (cE != clr && onlyE) || (cE == clr && !onlyE)
			|| cW == EMPTY || (cW != clr && onlyW) || (cW == clr && !onlyW)); 
	*/
}

/* δչ���� moveable() */

bool GO::moveable (const POS& pos, COLOR clr) const {   
	___FUNCOUNT( GO_MOVE); 
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 
	
	if ( color(pos) != EMPTY ) 
		return 0; 
	VP	 near4p = near4(pos); 
	for (int i = 0; i < near4p.size (); ++i) {
		COLOR c = color(near4p[i]); 
		// ����
		if (c == EMPTY)
			return 1; 
		// ����
		BITB blk = libertysite(near4p[i]); 
		if ((c != clr) && blk.only(pos) ) 
			return 1; 
		// ���ںϳ������Ŀ�
		else if (( c == clr) && blk.count() > 1)
			return 1; 
	}
	// ��ɱ
	return 0; 
}

/* ���ð� move() */

bool GO::move_(const POS& pos, COLOR clr) {   
	___FUNCOUNT( GO_MOVE); 
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 

	if (!moveable(pos, clr)) 
		return 0; 
	// ���� kill
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
	// ���� ...
	if (clr == BLACK)  
		xx |= pos, oo ^= kill; 
	else  
		oo |= pos, xx ^= kill; 
	++history; 
	lastpos = pos; 
	lastclr = clr; 
	// ���½ٵ�
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

/* չ��֮������ move() */

bool GO::move__(const POS& pos, COLOR clr) {   
	___FUNCOUNT( GO_MOVE); 
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
	// ����
	if (cN == EMPTY || cS == EMPTY || cE == EMPTY || cW == EMPTY)        
		mvable = 1; 
	// ���ںϳ������Ŀ�
	if (   ((cN == clr) && !onlyN)
		|| ((cS == clr) && !onlyS)
		|| ((cE == clr) && !onlyE)
		|| ((cW == clr) && !onlyW)
		)
		mvable = 1; 
	// ����
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
	// ��ɱ
	if (!mvable)
		return 0; 
	else 
		kill = tmpbb; 
	// ����
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

/* ������ move() */

bool GO::move(const POS& pos, COLOR clr) {   
	___FUNCOUNT( GO_MOVE); 
	___PARASSERT( !outside(pos) && (clr == BLACK || clr == WHITE) ); 
	
	if ( color(pos) != EMPTY ) 
		return 0; 
	BITB otherbb = getbb( ~clr ); 
	VP	 near4p = near4(pos); 
	BITB tmpbb = NULL_BB; 
	bool mvable = 0; 
	for (int i = 0; i < near4p.size (); ++i) {
		COLOR c = color(near4p[i]); 
		// ����
		if (c == EMPTY)
			mvable = 1; 
		BITB blk = libertysite(near4p[i]); 
		// ����
		if ((c != clr) && blk.only(pos) ) {
			mvable = 1; 
			hotko = near4p[i]; 
			tmpbb |= otherbb.blockat ( near4p[i]); 
		}
		// ���ںϳ������Ŀ�
		else if (( c == clr) && blk.count() > 1)
			mvable = 1; 
	}
	// ��ɱ
	if (!mvable)
		return 0; 
	else 
		kill = tmpbb; 
	// ����
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
	___FUNCOUNT( GO_ENDGAME); 
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
	___FUNCOUNT( GO_PEMIS3232); 
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
	___FUNCOUNT( GO_PEMIS64); 
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

/* _______________________________ ������ _______________________________ */

void TEST_GO::morphology(){
	GO  go; 
	go.random (); 
	//GO go = INOUT().frommatlab (); 
	cout<<go; 
	go.xx &= square(star(EN), 5); 
	int times = 5; 
	for(int i = 1; i <= times; ++i){
		BITB bb = go.xx.dilate (i); 
		cout<<"dilate times: "<<i<< endl<<GO(go.xx, bb^go.xx); 
		cout<<"dry    times: "<<i<< endl<<GO(go.xx, bb.dry (go.xx, i)^go.xx); 
		cout<<"adsorb times: "<<i<< endl<<GO(go.xx, bb.adsorb (go.xx, i)^go.xx); 
		cout<<"inner  times: "<<i<< endl<<GO(go.xx, go.xx.inner (i)); 
	}
}

void TEST_GO::timefunc (string which) {
 	 
	RANDER  r; 
	
	GO go = r.vg [0]; 

	___TIME( GO_RANDOM, go.random( LEGALIZE ); ); 
	___TIME( GO_ADD, go.add(r.vp[0], r.vc[0] ); ); 
	___TIME( GO_LEGALIZE, go.legalize(); ); 
	/*
	___TIME( GO_SNAP, go.snap(); ); 
	___TIME( GO_RESTORE, go.restore(r.vg[0]); ); 
	*/
	___TIME( GO_PEMIS3232, go.pemis3232(r.vb[0]); ); 
	___TIME( GO_PEMIS64, go.pemis64(r.vb[0]); ); 
	___TIME( GO_TRANSPOSE, go.transpose( ); )		
	___TIME( GO_ROTATE, go.rotate(r.vi[0]); )		
	___TIME( GO_TURN, go.turn( ); )					
	___TIME( GO_GETLASTCLR, go.getlastclr( ); )				
	___TIME( GO_COLOR, go.color( r.vp[0] ); )			
	___TIME( GO_GETLASTPOS, go.getlastpos( ); )			
	___TIME( GO_GETBB, go.getbb( r.vc[0] ); )			
	___TIME( GO_EXPAND, go.expand( r.vb[0], r.vc[0] ); )	
	___TIME( GO_LIBERTYSITE, go.libertysite( r.vp[0] ); )		
	___TIME( GO_LIBERTY, go.liberty_( r.vp[0] ); )			
	___TIME( GO_MOVEABLE, go.moveable( r.vp[0], r.vc[0] ); )	
	___TIME( GO_MOVE, go.move( r.vp[0], r.vc[0] ); ); 
	___TIME( GO_ENDGAME, go.endgame( ); ); 
 
}
