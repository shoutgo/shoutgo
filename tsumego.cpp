#include "tsumego.h"
#include "inout.h"

PBB
TSUMEGO::randomeye (int m, int n, MODE_RANDOMEYE mode)
{
  ___PARASSERT (m > n);
  BITB enemy;
  for (;;)
    {
      random (ILLEGALIZE);
      ITR itr (oo);
      for (BITB space = itr.blockbegin (); !itr.blockend (); space = itr.blocknext ())
	{
	  enemy = space & xx;
	  if ((!space.touchboarder ()) && (enemy != space))
	    {
	      if ((mode == LESS_LESS) && (space.count () <= m) && (enemy.count () <= n))
		return make_pair (space, enemy);
	      if ((mode == BIGGER_LESS) && ((space.count () > m) || (space.count () < n)))
		oo ^= space;
	      if ((mode == LESS_LESS_CENTER) && (space.count () <= m) && (enemy.count () <= n) && space[star (5)])
		return make_pair (space, enemy);
	      if ((mode == EQ_EQ) && (space.count () == m) && (enemy.count () == n))
		return make_pair (space, enemy);
	    }

	  else
	    oo ^= space;
	}
      if (mode == BIGGER_LESS)
	return make_pair (oo, xx & oo);
    }
}

void
TSUMEGO::randomtsumgo (int publicliberty, int m, int n, MODE_RANDOMEYE mode)
{
  PBB pbb = randomeye (m, n, mode);
  xx = pbb.second;
  oo = pbb.first;
  BITB di2 = oo.dilate (2);
  oo ^= di2;
  xx |= (di2.dilate (2) ^ di2);
  ITR itr (di2.dilate (1) ^ di2);
  for (POS pos = itr.randomstonebegin (); !itr.randomstoneend (); pos = itr.randomstonenext ())
    {
      if ((publicliberty--) <= 0)
	break;
      xx ^= pos;
    }
  legalize ();
}


/*		// 这部分从 GO 里搬来
pair<EYEKIND, VP > TSUMEGO::tsumego(COLOR sente) {
	COLOR surrounder; 
	int max = 0; 
	BITB space, enemy; 
	VG vecgo = partition(); 
	ITR itr = ITR(vecgo.back() .xx); 
	for (BLOCK part = itr.blockbegin(); !itr.blockend(); part = itr.blocknext()){
		int partc = part.count(); 
		if(max<partc){
			surrounder = BLACK; 
			max = partc; 
			space = xx.inner(7); 
			enemy = space&oo; 
		}
	}
	itr = ITR(vecgo.back() .oo); 
	for (BLOCK part = itr.blockbegin(); !itr.blockend(); part = itr.blocknext()){
		int partc = part.count(); 
		if(max<partc){
			surrounder = WHITE; 
			max = partc; 
			space = oo.inner(7); 
			enemy = space&xx; 
		}
	}										
	if (sente == surrounder)				
		return eyekind_RESCUE(sente, space, enemy); 
	else
		return eyekind_KILL(sente, space, enemy); 
}

pair<EYEKIND, VP > TSUMEGO::eyekind_KILL(COLOR sente, BITB space, BITB enemy, EYEKIND passbest) {
	if (enemy == NULL_BB)										
		return space.eyekind(KILL); 
	BITB sp = enemy.inner (5); 
	//pair<EYEKIND, VP > te = sp.eyekind (RESCUE, sp&(~space)); 
	pair<EYEKIND, VP > te = eyekind_RESCUE(sente, sp, sp & (~space)); 
	if ((sp != NULL_BB)&&(te.first>ONEEYE))
		return make_pair(ENEMY_ALIVE, te.second ); 
	if ((space.blockcount()>1) || ((space.count()>13) && (enemy.count()<7)))	
		return make_pair(SOMEEYE, NULL_VP); 

	PBB upd; 
	pair<EYEKIND, VP >	 tmp; 
	VP  vecp[MANYEYE]; 

	EYEKIND best = SOMEEYE; 
	GO snapgo = snap(); 
	ITR itr = ITR(space^enemy); 
    for (POS pos = itr.stonebegin (); !itr.stoneend(); pos = itr.stonenext()){
		upd = update_KILL(pos, sente, space, enemy); 
		if (upd == make_pair(FULL_BB, FULL_BB)) continue; 
		tmp = eyekind_RESCUE(~sente, upd.first, upd.second); 
		if (tmp.first <= best) {
			vecp[tmp.first ].push_back (pos); 
			best = tmp.first; 
		}
		restore(snapgo); 
	}
	if (vecp[best].empty()) {															
		return make_pair(enemy.eyekind (AFTERDELETE).first, NULL_VP); }
	if (passbest == UNKOWN_WHENPASS){			
		te = eyekind_RESCUE(~sente, space, enemy, best); 
		passbest = te.first; 
	}
	if (passbest< best) return make_pair(CO_ONEEYE, NULL_VP); 
	else if (passbest == best) return make_pair(passbest, NULL_VP); 
	////if (passbest <= best) return make_pair(passbest, NULL_VP); 
	else return make_pair(best, vecp[best]); 
}

pair<EYEKIND, VP > TSUMEGO::eyekind_RESCUE(COLOR sente, BITB space, BITB enemy, EYEKIND passbest) {
	if (enemy == NULL_BB)			
		return space.eyekind(RESCUE); 
	BITB sp = enemy.inner (5); 
	//pair<EYEKIND, VP > te = sp.eyekind (KILL, sp&(~space)); 
	pair<EYEKIND, VP > te = eyekind_KILL(sente, sp, sp & (~space)); 
	if ((sp != NULL_BB)&&(te.first>ONEEYE))									//这里可以分得再细
		return make_pair(ENEMY_ALIVE, te.second ); 
	if ((space.blockcount()>1) || ((space.count()>13) && (enemy.count()<7)))	{cout<<1111<< endl; 
		return make_pair(SOMEEYE, NULL_VP); }//似乎错误在此, 可能是基本函数有问题, 但为什么_KILL总正确?

	PBB upd; 
	pair<EYEKIND, VP >	 tmp; 
	VP  vecp[MANYEYE]; 

	EYEKIND best = ENEMY_ALIVE; /////////////////////
	GO snapgo = snap(); 
	ITR itr = ITR(space^enemy); 
    for (POS pos = itr.stonebegin (); !itr.stoneend(); pos = itr.stonenext()){
		upd = update_RESCUE(pos, sente, space, enemy); 
		if (upd == make_pair(FULL_BB, FULL_BB)) continue; 
		tmp = eyekind_KILL(~sente, upd.first, upd.second); 
		if (tmp.first >= best) {
			vecp[tmp.first ].push_back (pos); 
			best = tmp.first; 
		}
		restore(snapgo); 
	}
	if (vecp[best].empty()) 
		return make_pair(ENEMY_ALIVE, NULL_VP); 
	if (passbest == UNKOWN_WHENPASS){			
		te = eyekind_KILL(~sente, space, enemy, best); 
		passbest = te.first; 
	}				
	if (passbest> best)															
		return make_pair(CO_ONEEYE, NULL_VP); 
	else if (passbest == best)													
		return make_pair(passbest, NULL_VP); 
	////if (passbest >= best) return make_pair(passbest, NULL_VP); 
	else return 
		make_pair(best, vecp[best]); 
}

PBB			TSUMEGO::update_KILL(POS pos, COLOR sente, BITB space, BITB enemy){
	BITB delblock = move(pos, sente); 
	if (delblock == (FULL_BB)) return make_pair(FULL_BB, FULL_BB); 
	else return make_pair(space|delblock, enemy|pos); 
}

PBB			TSUMEGO::update_RESCUE(POS pos, COLOR sente, BITB space, BITB enemy){
	BITB delblock = move(pos, sente); 
	if (delblock == (FULL_BB)) return make_pair(FULL_BB, FULL_BB); 
	else return make_pair(space^pos, enemy^delblock); 
}
*/

////////////////////////////////// 这部分从 BITB 搬来
/*

pair<EYEKIND, VP > TSUMEGO::eyekind(ACTION action, BITB enemy, EYEKIND passbest) {
	if (enemy == NULL_BB)										return eyekind(action); 
	BITB sp = enemy.inner (7); 
	if ((sp != NULL_BB)&&(sp.eyekind (turnaction(action), sp & ( ~ BITB(r))).first>ONEEYE))
																	return make_pair(ENEMY_ALIVE, NULL_VP); 
	if ((blockcount()>1) || ((count()>17) && (enemy.count()<7)))	return make_pair(SOMEEYE, NULL_VP); 

	PBB upd; 
	pair<EYEKIND, VP >	 tmp; 
	VP  vecp[MANYEYE]; 

	if (action == KILL){
		EYEKIND best = SOMEEYE; 
		ITR itr = ITR(BITB(r)^enemy); 
        for (POS pos = itr.stonebegin (); !itr.stoneend(); pos = itr.stonenext()){
			upd = update(KILL, enemy, pos); //cout<<444444444444444444<< endl<<GO(enemy, BITB(r))<<GO(upd.second, upd.first ); 
			if (upd == make_pair(FULL_BB, FULL_BB)) continue; 
			tmp = upd.first.eyekind(RESCUE, upd.second); 
			if (tmp.first <= best) {
				vecp[tmp.first ].push_back (pos); 
				best = tmp.first; 
			}
			else continue; 
		}
		if (vecp[best].empty()) {							     //cout<<11111111111111<< endl<<GO(enemy, BITB(r)); 
			return make_pair(enemy.eyekind (AFTERDELETE).first, NULL_VP); }
		if (passbest == UNKOWN_WHENPASS)			
			passbest = eyekind(RESCUE, enemy, best).first; //cout<<5555555555<< endl<<GO(enemy, BITB(r))<<"action:"<<action<<"  best:"<<eyekindname[best]<<"  passbest:"<<eyekindname[passbest]<< endl; 
		if (passbest< best) return make_pair(CO_ONEEYE, NULL_VP); /////////////////////问题在此，涉及到公气问题。
		else if (passbest == best) return make_pair(passbest, NULL_VP); 
		else return make_pair(best, vecp[best]); 
	}
	if (action == RESCUE){
		EYEKIND best = ENEMY_ALIVE; ///////////////////////////////
		ITR itr = ITR(BITB(r)^enemy); 
        for (POS pos = itr.stonebegin (); !itr.stoneend(); pos = itr.stonenext()){
			upd = update(RESCUE, enemy, pos); //cout<<7777777777777777777<< endl<<GO(enemy, BITB(r))<<GO(upd.second, upd.first ); 
			if (upd == make_pair(FULL_BB, FULL_BB)) continue; 
			tmp = upd.first.eyekind(KILL, upd.second); 
			if (tmp.first >= best) {
				vecp[tmp.first ].push_back (pos); 
				best = tmp.first; 
			}
			else continue; 
		}
		if (vecp[best].empty()) {						        //cout<<22222222222222<< endl<<GO(enemy, BITB(r)); 
		return make_pair(ENEMY_ALIVE, NULL_VP); }
		if (passbest == UNKOWN_WHENPASS)			
			passbest = eyekind(KILL, enemy, best).first; //cout<<6666666666666<< endl<<GO(enemy, BITB(r))<<"action:"<<action<<"  best:"<<eyekindname[best]<<"  passbest:"<<eyekindname[passbest]<< endl; 
		if (passbest> best) return make_pair(CO_ONEEYE, NULL_VP); 
		else if (passbest == best) return make_pair(passbest, NULL_VP); 
		else return make_pair(best, vecp[best]); 
	}
}

PBB			TSUMEGO::update(ACTION action, BITB enemy, POS pos){
	GO go(enemy, BITB(r)); 
	go.oo ^= go.oo.dilate (1); //cout<<action<< endl<<"go:"<< endl<<go; 
	if (action == KILL) {
		BITB delblock = go.move(pos, BLACK); //cout<<"kill delblock:"<< endl<<delblock; 
		if (delblock == (FULL_BB)) return make_pair(FULL_BB, FULL_BB); 
		else return make_pair(BITB(r)|delblock, enemy|pos); 
	}
	if (action == RESCUE) {
		BITB delblock = go.move(pos, WHITE); //cout<<"rescue delblock:"<< endl<<delblock; 
		if (delblock == (FULL_BB)) return make_pair(FULL_BB, FULL_BB); 
		else return make_pair(BITB(r)^pos, enemy^delblock); 
	}											
}
*/

GO
TSUMEGO::snap () const
{
  return *this;
}

void
TSUMEGO::restore (const GO & go)
{

  // *this = go; 
}
