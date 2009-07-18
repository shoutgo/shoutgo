#include "infoboard.h"
#include "inout.h"

/* class INCREMENT */

INCREMENT::INCREMENT(){
	tag.assign (BS*BS+1, NULL_TAG); 
	// color 与其它成员不一样，因为无子处的着色是有意义的，
	// 而无子处的其它成员是没有意义的（用相应的NULL量表示）
	color.assign (BS*BS+1, EMPTY); 
	color.back() = NULL_CLR; 
	
	content.assign (BS*BS+1, NULL_BB); 
	enemysite.assign (BS*BS+1, NULL_BB); 
	libertysite.assign (BS*BS+1, NULL_BB); 
	liberty.assign (BS*BS+1, 0); 
}

BITB INCREMENT::getcontent(int i) const {
	return content[tag[i]]; 
}

BITB INCREMENT::getenemysite(int i) const {
	return enemysite[tag[i]]; 
}

BITB INCREMENT::getlibertysite(int i) const {
	return libertysite[tag[i]]; 
}

COLOR INCREMENT::getcolor(int i) const {
	return color[i]; 
}

int INCREMENT::getliberty(int i) const {
	return liberty[tag[i]]; 
}

int INCREMENT::gettag(int i) const {
	return tag[i]; 
}

void INCREMENT::setcontent(int i, const BITB& blk){
	content[tag[i]] = blk; 
}

void INCREMENT::setcolor(int i, COLOR clr){
	color[i] = clr; 
}

void INCREMENT::setenemysite(int i, const BITB& bb){
	enemysite[tag[i]] = bb; 
}

void INCREMENT::setlibertysite(int i, const BITB& bb){
	libertysite[tag[i]] = bb; 
}

void INCREMENT::setliberty(int i, int n){
	liberty[tag[i]] = n; 
}

void INCREMENT::settag(int i, int t){
	tag[i] = t; 
}

// 除了tag, color 其它 4 向量并不一定完全相等；不能直接比较
bool INCREMENT::operator == (const INCREMENT& ic) const {
	for (int i = 0; i<BS*BS + 1; ++i){
		if (ic.gettag(i) != gettag(i)) {
			//___COUT2( ic.gettag(i), gettag(i)); 
			return 0; 
		}
		if (ic.getcolor(i) != getcolor(i)) {
			//___COUT2(ic.getcolor(i), getcolor(i) ); 
			return 0; 
		}
		if (ic.getenemysite(i) != getenemysite(i)) {
			//___COUT2(ic.getenemysite(i), getenemysite(i) ); 
			return 0; 
		}
		if (ic.getcontent(i) != getcontent(i)) {
			//___COUT2(ic.getcontent(i), getcontent(i) ); 
			return 0; 
		}
		if (ic.getlibertysite(i) != getlibertysite(i)) {
			//___COUT2(ic.getlibertysite(i), getlibertysite(i) ); 
			return 0; 
		}
		if (ic.getliberty(i) != getliberty(i)) {
			//___COUT2(ic.getliberty(i), getliberty(i) ); 
			return 0; 
		}
	}
	return 1; 
}

/* class ZOBRIST */

ZOBRIST::ZOBRIST(){
	for (int i = 0; i<ZOBRIST_KIND; ++i)
		count[i] = 0, 
		key[i] = 0; 
}

void ZOBRIST::setzobrist(int i, const POS& lastpos, COLOR lastclr, const BITB& kill){
		key[i] ^= g_init.getrand(lastpos, lastclr); 
		ITR itr(kill); 
		for (POS p = itr.stonebegin (); !itr.stoneend(); p = itr.stonenext ())
			key[i] ^= g_init.getrand(p, ~lastclr); 
		// 未减死子数
		++ count[i]; 
}

KEY ZOBRIST::getzobrist(int i) const{
	return key [i]; 
}

int	ZOBRIST::getcount(int i) const{
	return count[i]; 
}

/* class PATFREQ */
void PATFREQ::setflag(){
	for(int i = 0; i<BS*BS ; ++i)
		for (int j = 0; j<PEMIS_KIND; ++ j)
			x[j][i] = NOINIT,
			o[j][i] = NOINIT;
}

void PATFREQ::setf(const POS& pos, COLOR clr, int i, int freq){
	if (clr == BLACK)
		x[i][pos2i(pos)] = freq; 
	else
		o[i][pos2i(pos)] = freq; 
}

int PATFREQ::getf(const POS& pos, COLOR clr, int i) const{
	if (clr == BLACK)
		return x[i][pos2i(pos)]; 
	else
		return o[i][pos2i(pos)]; 
}

/* class INFOGO */

INFOGO::INFOGO():father(0) {			//-// 
}

INFOGO::INFOGO(const GO& go):father(0){ //-//
	xx = go.xx; 
	oo = go.oo; 
	kill = go.getkill(); 
 	hotko = go.gethotko(); 
    lastpos = go.getlastpos(); 
	lastclr = go.getlastclr(); 
	history = go.gethistory(); 
}

INFOGO::~INFOGO(){
}

bool INFOGO::operator == (const GO& go) const {
	return (go.xx == xx 
		&& go.oo == oo 
		&& go.getkill () == getkill() 
		&& go.gethotko() == gethotko() 
		&& go.getlastpos () == getlastpos() 
		&& go.getlastclr () == getlastclr() 
		&& go.gethistory () == gethistory() ); 
}

INCREMENT INFOGO::scan() const {	
	INCREMENT tmp; 
	ITR itr; 
	for (POS p = itr.posbegin (); !itr.posend (); p = itr.posnext ()){
		COLOR	clr = color(p); 
		BITB	blk = getbb(clr).blockat(p); 
		int		tag = blk.tag() == NULL_POS ? 
					NULL_TAG : pos2i(blk.tag()); 
		int		m = pos2i(p); 
		tmp.settag(m, tag); 
		/*
		___ASSERT2( 
			inc.gettag (m) == tag, 
			___COUT4(*this, p, tag, inc.gettag (m))); 
		*/
	}
	for (POS p = itr.posbegin (); !itr.posend (); p = itr.posnext ()){
		COLOR	clr = color(p); 
		BITB	blk = getbb(clr).blockat(p); 
		BITB	libsite = libertysite(p); 
		int		lib = libsite.count(); 
		BITB	ene = enemysite(blk, clr); 
		int		m = pos2i(p); 
		tmp.setcolor (m, clr); 
		tmp.setcontent(m, blk); 
		tmp.setlibertysite(m, libsite); 
		tmp.setliberty (m, lib); 
		tmp.setenemysite(m, ene); 
		/*
		___ASSERT2( 
			inc.getcontent (m) == blk, 
			___COUT4(*this, p, blk, inc.getcontent (m))); 
		___ASSERT2( 
			inc.getenemysite (m) == ene, 
			___COUT4(*this, p, ene, inc.getenemysite (m))); 
		___ASSERT2( 
			inc.getlibertysite (m) == libsite, 
			___COUT4(*this, p, libsite, inc.getlibertysite (m))); 
		___ASSERT2(
			inc.getliberty (m) == lib, 
			___COUT4(*this, p, lib, inc.getliberty (m))); 
		___ASSERT2(
			inc.getcolor (m) == clr, 
			___COUT4(*this, p, clr, inc.getcolor (m))); 
		*/
	}
	return tmp; 
}

bool INFOGO::move  ( const POS& pos, COLOR clr) {	
	
	int ip = pos2i(pos); 
	COLOR ic = inc.getcolor (ip); 
	
	// ___________________________________
	// 
	// tmp : mvable, tmphotko
	// kill
	// ___________________________________

	// 子禁或劫禁时，不能落子，返回
	if (ic != EMPTY)	             
		return 0; 
	
	// 取各相邻位，检查各相邻块
	POS		tmphotko; 
	bool	mvable = 0; 
	BITB	killenemysite = NULL_BB; 
	VP		near4p = near4(pos); 
	VI		n; 
	for	(int i = 0; i<near4p.size (); ++i)
		n.push_back (pos2i(near4p[i])); 
	kill = NULL_BB; 
	for (int i = 0; i<near4p.size (); ++i){
		COLOR c = inc.getcolor (n[i]); 
		// 如果为空位，可落子  // 或劫位？不必，不可能落在劫位之旁。
		if (c == EMPTY ) 
			mvable = 1; 
		// 如果为 1 气的异色块
		else if ( (c == ~clr) && (inc.getliberty (n[i]) == 1) ) {
			// 可落子
			mvable = 1; 
			// 记录可能劫位
			tmphotko = near4p[i]; 
			// 记录为可提块（合成了每块，可能有块相同）
			kill |= inc.getcontent (n[i]); 
			// 记录此可提块的敌块
			killenemysite |= inc.getenemysite (n[i]); 
		}
		// 否则如果为大于 1 气的同色块，可落子
		else if ((c == clr) && (inc.getliberty(n[i]) > 1))
			mvable = 1; 
		// 不中断，各相邻块检测完
	}

	// 如果不能落子，返回
	if (mvable == 0) 
		return 0; 

	// _____________________________________
	// tag			-> 
	// color		-> 
	// ____________________________
	// content		-> 
	// enemysite	->
	// libertysite	->
	// liberty		->
	// _____________________________________
	
	// 对各可提块的各位置的对应颜色及块头
    ITR itr; 
	itr = ITR(kill); 
	for(POS p = itr.stonebegin(); !itr.stoneend(); p = itr.stonenext()) {
		int m = pos2i(p); 
		// 更新颜色
		inc.setcolor (m, EMPTY); 
		// 擦除原记录
		inc.settag (m, NULL_TAG); 
	}
	
	// 对可提块的敌块（因落子发生了融合的会在更新落子处时继续更新）
	itr = ITR(killenemysite); 
	for(POS p = itr.tagbegin(); !itr.tagend(); p = itr.tagnext()) {
		int m = pos2i(p); 
		//  tmp:此敌块的敌块　与　可提块的重合部
		BITB tmp = (inc.getenemysite (m) & kill); 
		//  更新此敌块的敌块：去掉 tmp
		inc.setenemysite(m, inc.getenemysite (m)^tmp); 
		//  更新此敌块的气点：压入tmp 
		inc.setlibertysite (m, inc.getlibertysite(m)|tmp); 
		//  更新此敌块的气：加上tmp的点数
		inc.setliberty(m, inc.getliberty (m)+tmp.count ()); 
	}

	// _____________________________________
	// tag			-> 
	// color		-> 
	// ____________________________
	// content		-> 
	// enemysite	->
	// libertysite	->
	// liberty		->
	// _____________________________________
	
	// 更新落子处及其周边位置信息
	BITB  newcontent = BITB(pos); 
	BITB  newlibertysite = NULL_BB; 
	BITB  newenemysite = NULL_BB; 
	int	  newtag = ip; 
	// 对各相邻位
	for(int i = 0; i<near4p.size (); ++i){
		COLOR c = inc.getcolor (n[i]); 
		// 如果为空位 // 或劫位？不必，因为劫位旁边一定是子
		if ((c == EMPTY))  
			// 记录为新气点之一
			newlibertysite |= near4p[i]; 
		// 如果为异色块
		else if (c == ~clr) {
			// 记录为新敌人之一
			newenemysite |= near4p[i]; 
			// 同时更新为敌块的敌人之一
			inc.setenemysite (n[i], inc.getenemysite (n[i])|pos); 
			// 修正：如果落子处是敌块之气点（同一块确保只操作一次）
			if (inc.getlibertysite(n[i])[pos]){			
				// 去掉此气点
				inc.setlibertysite(n[i], inc.getlibertysite(n[i])^pos); 
				// 减掉一气
				inc.setliberty (n[i], inc.getliberty (n[i])-1); 
			}
		}
		// 如果是同色块
		else {
			// 生成新棋头　//-// < 对吗？ 在目前的 tag() 定义下正确
			if (inc.gettag(n[i]) < newtag)
				newtag = inc.gettag(n[i]); 
			// 生成新棋块：压入原棋块
			newcontent |= inc.getcontent (n[i]); 
			// 生成新气块：压入原气块
			newlibertysite |= inc.getlibertysite(n[i]); 
			// 生成新敌块：压入原敌块
			newenemysite |= inc.getenemysite (n[i]); 
			// 擦除原记录信息，同一块的信息只需利用一次，重复也没错
			// inc.settag(n[i], NULL_TAG); 
		}
	}
	// 修正：如果新气块包含落子处，剔除（当落子为独子时不能操作）
	if (newlibertysite[pos]) 
		newlibertysite ^= pos; 

	// 更新新棋块的各位置的棋头（与落子发生了融合的会在此进继续更新）
	itr = ITR(newcontent); 
	for(POS pp = itr.stonebegin(); !itr.stoneend(); pp = itr.stonenext()) {
		int m = pos2i(pp); 
		inc.settag (m, newtag); 
	}
	// 更新新棋块的其它信息
	inc.setcolor(ip, clr); 
	inc.setcontent (ip, newcontent); 
	inc.setlibertysite (ip, newlibertysite); 
	inc.setliberty (ip, newlibertysite.count ()); 
	inc.setenemysite (ip, newenemysite); 

	// ______________________________________
	// 
	// xx, oo, hotko, history, lastpos, lastclr
	// ______________________________________

	// 盘面更新：落子，记录历史，走步，劫
	if (clr == BLACK) 
		xx |= pos, oo ^= kill; 
	else 
		oo |= pos, xx ^= kill; 
    ++history; 
	lastpos = pos; 
	lastclr = clr; 
	// 如果落子为独子，提子为独子，且落子为 1 气，则原提子位为劫位
	if (hotko != NULL_POS)
		inc.setcolor (pos2i(hotko), EMPTY); 
	if (kill.count() == 1 
		&& inc.getcontent (ip).count() == 1 
		&& inc.getliberty (ip) == 1 ) { 
			inc.setcolor (pos2i(tmphotko), HOTKO); 
			hotko = tmphotko; 
		}
	else
		hotko = NULL_POS; 

	return 1; 
}

VG	INFOGO::partition(int n)  const {	
	VG vg; 
	vg.push_back (*this); 
	BITB tmpx[BS], tmpo[BS], contact; // [n] ?
	for (int i = 0; i<n; ++i) {
		contact = vg[i].xx & vg[i].oo; 
		tmpx[i] = expand(vg[i].xx^contact, BLACK); 
		tmpx[i] = tmpx[i]^(tmpx[i]&vg[i].oo)|vg[i].xx; 
        tmpo[i] = expand(vg[i].oo^contact, WHITE); 
		tmpo[i] = tmpo[i]^(tmpo[i]&vg[i].xx)|vg[i].oo; 
	    if ((tmpx[i] == vg[i].xx) && (tmpo[i] == vg[i].oo)) 
			break; 
        vg.push_back (GO(tmpx[i], tmpo[i])); 
	}
	return vg; 
}
/*
// 色域精确化
PBB	INFOGO::lastpartition(int n) const {	
	GO     gp = partition(n).back(); 
	BITB  tmp; 
	int p, q; 
	ITR itr = ITR(gp.xx & gp.oo); 
	for (BITB bb = itr.blockbegin(); !itr.blockend(); bb = itr.blocknext()){
		PII r = bb.range(); 
		if ((r.first < SMALLBLOCK_RANGE) && (r.second < SMALLBLOCK_RANGE)){						
			tmp = bb.dilate (1); 
			p = (tmp&xx).count (); 
			q = (tmp&oo).count (); 
			if (p == q){
				gp.xx ^= bb; 
				gp.oo ^= bb; 
			}
			else if (p>q)  
				gp.oo ^= bb; 
			else           
				gp.xx ^= bb; 
		}
		else {
			gp.xx ^= bb; 
			gp.oo ^= bb; 
		}
	}
	return make_pair(gp.xx, gp.oo); 
}

pair<VB, VB >	INFOGO::getallmass(const PBB& pbb) const {	
	VB vecx, veco; 
	ITR itr = ITR(pbb.first ); 
	for (BITB bb = itr.blockbegin(); !itr.blockend(); bb = itr.blocknext())
		vecx.push_back (bb&xx); 
	itr = ITR(pbb.second ); 
	for (BITB bb = itr.blockbegin(); !itr.blockend(); bb = itr.blocknext())
		veco.push_back (bb&oo); 
	return make_pair(vecx, veco); 
}

AREA	INFOGO::setarea(int pt, int dt, int at) const {	
	pair<VB, VB > pv = getallmass(lastpartition(pt)); 
	PBB clrbb = lastpartition(BS); //pt
	AREA area; 
	BITB tmp, xbb, obb, xclr, oclr; 
	for (int i = 0; i<pv.first .size (); ++i){
		xclr = clrbb.first.blockon (pv.first [i]); 
		tmp = pv.first [i].dilate (dt); 
		area.xmass		.push_back (pv.first [i]); 
        area.xrawland	.push_back (tmp.dry (pv.first[i], dt)); 
		area.xrawsea		.push_back (tmp.adsorb (pv.first[i], dt)); 
		area.xrawair		.push_back (tmp.adsorb (pv.first[i], dt-at)); 
		area.xland	.push_back (area.xrawland[i] & xclr); 
		area.xsea		.push_back (area.xrawsea[i]		  & xclr); 
		area.xair		.push_back (area.xrawair[i]		  & xclr); 
		xbb |= area.xair [i]; 
		___ASSERT(area.xair[i].contain (area.xsea[i])); 
		___ASSERT(area.xsea[i].contain (area.xland[i])); 
	}
	for (int i = 0; i<pv.second .size (); ++i){
		oclr = clrbb.second.blockon (pv.second [i]); 
		tmp = pv.second [i].dilate (dt); 
		area.omass		.push_back (pv.second [i]); 
        area.orawland	.push_back (tmp.dry (pv.second[i], dt)); 
		area.orawsea		.push_back (tmp.adsorb (pv.second[i], dt)); 
		area.orawair		.push_back (tmp.adsorb (pv.second[i], dt-at)); 
		area.oland	.push_back (area.orawland[i] & oclr); 
		area.osea		.push_back (area.orawsea[i]       & oclr); 
		area.oair		.push_back (area.orawair[i]		  & oclr); 
		obb |= area.oair [i]; 
		___ASSERT(area.oair[i].contain (area.osea[i])); 
		___ASSERT(area.osea[i].contain (area.oland[i])); 
	}
	tmp = ~(xbb|obb); 
	area.publicsea = (tmp & ~square(star(5), 6)).ruboff(2, 0); 
	area.publicair = (tmp & square(star(5), 6)).ruboff(2, 0); 
	area.undecided = ~(xbb|obb|area.publicsea|area.publicair); 
	return area; 
}
*/
INFOGO* INFOGO::move_tree(const POS& pos, COLOR clr){
	INFOGO* infop = new INFOGO(*this); 
	if ( infop->move(pos, clr) ){
		sons.push_back (infop); 
		infop->father = this; 
		infop->sons.clear (); 
		infop->move (pos, clr); 
		return infop; 
	}
	else {
		delete infop; 
		return this; 
	}
}

// 前 4 对应角定式，后 4 对应边定式，最后为全局定式
// 未考虑征子
// 未考虑劫点
// 未考虑着子方(假设学习时数据集足够大，各种对称情况都已出现过，
// 包括颜色，故现在没有必要考虑这些, 对应用方式没有影响） 
void INFOGO::setzobrist () {
	int m = pos2i(lastpos); 
	BITB bb = (xx|oo).blockon( inc.getcontent(m) | inc.getenemysite(m)) | kill; 
	for (int i = 0; i<4; ++i) {
		if ( zobrist.getcount (i) < CORNERJOSEKI_MAX)
			if (g_init.getjosekimask (i).contain(bb)){
				zobrist.setzobrist (i, lastpos, lastclr, kill); 
				break; 
			}
	}
	for (int i = 4; i<8; ++i) 
		if ( zobrist.getcount (i) < EDGEJOSEKI_MAX)
			if (g_init.getjosekimask (i).contain(bb)){
				zobrist.setzobrist (i, lastpos, lastclr, kill); 
			}
	if (zobrist.getcount (8) < FUSEKI_MAX)
		zobrist.setzobrist (8, lastpos, lastclr, kill); 
}

KEY INFOGO::getzobrist(int n) const {
	return zobrist.getzobrist (n); 
}

VI INFOGO::getf_zobrist(int n, const BITB mask, COLOR clr) const {
	VI vi(BS*BS, 0);
	MOVE move;
	for (int i =0; i<g_learn.lib_zobrist[n].getsonssize(getzobrist(n)); ++i) {
		move = g_learn.lib_zobrist[n].getmove(getzobrist(n), i);
		if ( mask[move.first] && move.second == clr)
			vi[pos2i(move.first)] = g_learn.lib_zobrist[n].getfreq(getzobrist(n), i);
	}
	return vi;
}

/*
VI INFOGO::getf_patfreq(int n, const BITB mask, COLOR clr) const {
	VI vi(BS*BS, 0);
	ITR itr = ITR(mask);
	for (POS pos = itr.posbegin(); !itr.posend(); pos = itr.posnext())
		vi[pos2i(pos)] = patfreq.getf(pos, clr, n);
	return vi;
}
*/
VI INFOGO::getf_patfreq(int n, const BITB mask, COLOR clr) {
	VI vi(BS*BS,0);
	PUU puu;
	INFOGO snap = *this;
	snap.father = 0;
	ITR itr = ITR(mask);
	int tablef;
	for (POS pos = itr.posbegin(); !itr.posend(); pos = itr.posnext()) {
		tablef = patfreq.getf(pos, clr, n);	//
		if ( tablef != NOINIT){				//
			vi[pos2i(pos)] = tablef;		//
			continue;						//
		}									//
		INFOGO* sonp = snap.move_tree(pos, clr);
		if (sonp == &snap){
			vi[pos2i(pos)] = -1;
			patfreq.setf(pos, clr, n, -1);	//
			continue;
		}
		else {
			puu = sonp->pemis(n);
			tablef = g_learn.lib_pemis.getfreq(puu.first, (ULL)puu.second);
			vi[pos2i(pos)] = tablef; 
			patfreq.setf(pos, clr, n, tablef);	//
			delete sonp;
		}
	}
	return vi;
}

PUU	INFOGO::pemis(int n) const{
	BITB bb = BITB(getlastpos()).dilate(n+1);//
	BITB tmp = father->xx.blockon(bb) | father->oo.blockon(bb);
	if (tmp.empty()&& !g_init.getroute(R4)[getlastpos()] ){ //
		return make_pair(0,0);
	}
	ULL f = father->pemis64(tmp);
	tmp = xx.blockon(bb) | oo.blockon(bb);
	ULL s = pemis64(tmp);
	return make_pair(f,s);
}

POS INFOGO::genmove() const {
	RANDER rnd; 
	return rnd.vp [0]; 
}

void INFOGO::delfromhere() {
	for (int i = 0; i<sons.size (); ++i)
		sons[i]->delfromhere(); 
	//clog << "delete the INFOGO !"<< endl; 
	delete this; 
}

void INFOGO::delfromroot() {
	INFOGO* infop = this; 
	while (infop->father)
		infop = infop->father; 
	infop->delfromhere (); 
}

int	INFOGO::samemassmapsnumb( ) const{
	int m = min(father->area.massmaps.size(), area.massmaps.size()); 
	for (int i = 0; i<m; ++i)
		if (father->area .massmaps [i].xx == area.massmaps[i].xx
			&& father->area .massmaps [i].oo == area.massmaps[i].oo)
			return i; 
	return -1; 
}

// 返回所有钮形区域，可能有些区域不止四子
BITB INFOGO::cutingpos( ) const{
	BITB tmpx, tmpo, tmp; 
	tmpx.r[0] = xx.r[0] & xx.r[1]<<1 & oo.r[0]<<1 & oo.r[1]
			|	xx.r[0] & xx.r[1]>>1 & oo.r[0]>>1 & oo.r[1]; 
	tmpo.r[0] = oo.r[0] & oo.r[1]<<1 & xx.r[0]<<1 & xx.r[1]
			|	oo.r[0] & oo.r[1]>>1 & xx.r[0]>>1 & xx.r[1]; 
	for (int i = 1; i<BS-1; ++i){
		tmpx.r[i] = xx.r[i] & xx.r[i-1]<<1 & oo.r[i]<<1 & oo.r[i-1]
				|	xx.r[i] & xx.r[i-1]>>1 & oo.r[i]>>1 & oo.r[i-1] 
				|	xx.r[i] & xx.r[i+1]<<1 & oo.r[i]<<1 & oo.r[i+1]
				|	xx.r[i] & xx.r[i+1]>>1 & oo.r[i]>>1 & oo.r[i+1]; 
		tmpo.r[i] = oo.r[i] & oo.r[i-1]<<1 & xx.r[i]<<1 & xx.r[i-1]
				|	oo.r[i] & oo.r[i-1]>>1 & xx.r[i]>>1 & xx.r[i-1] 
				|	oo.r[i] & oo.r[i+1]<<1 & xx.r[i]<<1 & xx.r[i+1]
				|	oo.r[i] & oo.r[i+1]>>1 & xx.r[i]>>1 & xx.r[i+1]; 
	}
	tmpx.r[BS-1] = xx.r[BS-1] & xx.r[BS-2]<<1 & oo.r[BS-1]<<1 & oo.r[BS-2]
			|	xx.r[BS-1] & xx.r[BS-2]>>1 & oo.r[BS-1]>>1 & oo.r[BS-2]; 
	tmpo.r[BS-1] = oo.r[BS-1] & oo.r[BS-2]<<1 & xx.r[BS-1]<<1 & xx.r[BS-2]
			|	oo.r[BS-1] & oo.r[BS-2]>>1 & xx.r[BS-1]>>1 & xx.r[BS-2]; 
	return tmpx|tmpo; 
}

// 未剔除禁入点及提子点，返回所有潜在钮形区域
BITB INFOGO::cutpos(COLOR sente) const{
	INFOGO tmp = *this; 
	switch (sente){
		case BLACK:
			tmp.xx |= xx.fly (1, 1); 
			break; 
		case WHITE:
			tmp.oo |= oo.fly (1, 1); 
			break; 
	}
	return tmp.cutingpos (); 
}

void INFOGO::update () {
	setzobrist(); 
	patfreq.setflag();
	area.massmaps = partition(); 
}
/*
VT INFOGO::top( ) const{
	VT vt; 
	PBC smass = area .getmass (getlastpos(), getlastclr()); 
	PBC smass_his = area.getmass ( smass.first, turn()); 
	PBC fmass = father->area .getmass (smass.first, getlastclr()); 
	PBC fmass_his = father->area .getmass (smass.first, turn()); 
	// 断事件，必然有异块半联（即断）已块半联（即断），可能有已块联络（全联）
	BITB cut = cutpos(getlastclr()).blockon(getlastpos()); 
	if (cut[getlastpos()]) {
		PBC f_his = father->area .getmass (cut, turn()); 
		PBC s_my = area.getmass (cut, getlastclr()); 
		vt.push_back (TARGET(DUAN, f_his, s_my)); //被断者，断者
	}
	// 已块联络，落在父的什么色区都有可能
	if ( fmass.first .blockcount() > 1)
		vt.push_back (TARGET(LIANLUO, fmass, smass)); //联络对象，联络效果
	// 异块隔开，落在父的什么色区都有可能
	if ( fmass_his.first .blockcount() < smass_his.first .blockcount() )
		vt.push_back (TARGET(GE, fmass_his, smass)); //隔开对象，阻隔者
	return vt; 
}
*/

/* class AREA */

PBC		AREA::getmass(const BITB& mask, COLOR clr) const {
	___PARASSERT(clr == BLACK || clr == WHITE); 
	return make_pair(massmaps[worknumb].getbb(clr).blockon (mask), clr); 
}

	// 返回值中的 COLOR 指BITB 在 clustermap 上的 color
PBC		AREA::getcluster(const BITB& mask) const {
	/*
	COLOR clr = clustermap[mask]; 
	BITB unclear = clustermap.xx & clustermap.oo; 
	switch (clr) {
		case BLACK:
			return make_pair( (clustermap.xx^unclear).blockon(mask), BLACK); 
		case WHITE:
			return make_pair( (clustermap.oo^unclear).blockon(mask), WHITE); 
		case BLACK_WHITE:
			if (massmaps.back()[mask] == BLACK)
				return make_pair( 
				(massmaps.back ().xx & unclear).blockon(mask), BLACK_WHITE); 
			else if (massmaps.back()[mask] == WHITE)
				return make_pair( 
				(massmaps.back ().oo & unclear).blockon(mask), BLACK_WHITE); 
		default:
			return make_pair(NULL_BB, NULL_CLR); 
	}
	*/
	PBC pbc; 
	return pbc; 
}

PBC		AREA::getpotential(const BITB& mask) const {
	/*
	COLOR clr = potentialmap[mask]; 
	switch (clr) {
		case BLACK:
			return make_pair(potentialmap.xx .blockon(mask), BLACK); 
		case WHITE:
			return make_pair(potentialmap.oo .blockon(mask), WHITE); 
		default:
			return make_pair(NULL_BB, NULL_CLR); 
	}
	*/
	PBC pbc; 
	return pbc; 
}

	// 写公共区如果是孤立区则连其massmap写入，
	// 否则写入“适当部分”massmap　。
void	AREA::setclustermap(const BITB& mask, COLOR clr){
	switch (clr) {
		case BLACK:
			if (clustermap.oo.contain (mask))
				clustermap.oo ^= mask; 
			clustermap.xx |= mask; 
			break; 
		case WHITE:
			if (clustermap.xx.contain (mask))
				clustermap.xx ^= mask; 
			clustermap.oo |= mask; 
			break; 
		case BLACK_WHITE:
			clustermap.xx |= mask; 
			clustermap.oo |= mask; 
			break; 
		default:; 
	}
}

	// 理想：势无重叠
void	AREA::setpotentialmap( ) {
	potentialmap.xx = massmaps.back ().xx 
		^ (massmaps.back ().xx & clustermap.oo)		// 减去死区和未定区
		| clustermap.xx; // 加上活区和未定区　和对方死区
	potentialmap.oo = massmaps.back ().oo 
		^ (massmaps.back ().oo & clustermap.xx)		// 减去死区和未定区
		| clustermap.oo; // 加上活区和未定区　和对方死区
	// 孤立的未定区设为对方色
	// 注意可能有双方未定区相挨
	BITB unclear = clustermap.xx & clustermap.oo; 
	ITR itr = ITR( massmaps.back ().xx & unclear ); 
	for (BITB tmp = itr.blockbegin(); !itr.blockend(); tmp = itr.blocknext()){
		if (isolated(make_pair(tmp, BLACK)))
			potentialmap.xx ^= tmp, 
			potentialmap.oo |= tmp; 
	}
	itr = ITR( massmaps.back ().oo & unclear ); 
	for (BITB tmp = itr.blockbegin(); !itr.blockend(); tmp = itr.blocknext()){
		if (isolated(make_pair(tmp, WHITE)))
			potentialmap.xx |= tmp, 
			potentialmap.oo ^= tmp; 
	}
}

BITB AREA::getclustermap(COLOR clr, STATE st) const {
	BITB unclear = clustermap.xx & clustermap.oo; 
	switch (clr){
		case BLACK:
			switch (st){
				case ALIVE:
					return (clustermap.xx & massmaps.back().xx).ruboff(unclear); 
				case DEAD:
					return (clustermap.oo & massmaps.back().xx).ruboff(unclear); 
				case UNCLEAR:
					return massmaps.back().xx & unclear; 
			}
		case WHITE:
			switch (st){
				case ALIVE:
					return (clustermap.oo & massmaps.back().oo).ruboff(unclear); 
				case DEAD:
					return (clustermap.xx & massmaps.back().oo).ruboff(unclear); 
				case UNCLEAR:
					return massmaps.back().oo & unclear; 
			}
	}
}

BITB	AREA::publicarea( ) const{
	int i = massmaps.size ()/2; 
	return ~(massmaps[i].xx | massmaps[i].oo ); 
}

int		AREA::delta() const{
	return clustermap.delta ()
		// + potentialmap.delta()
		// + massmaps[worknumb].delta()
; 
}

	//　注意可能有双方未定区相挨，不影响此算法
bool		AREA::isolated(const PBC& pbc) const {
	switch (pbc.second ) {
		case BLACK:
			return clustermap.xx.blockon(pbc.first ) == pbc.first; 
		case WHITE:
			return clustermap.oo.blockon(pbc.first ) == pbc.first; 
	}
}

STATE	AREA::clusterstate(const PBC& pbc) const {
	if (pbc.second == BLACK_WHITE)
		return UNCLEAR; 
	if ( !(pbc.first & getbb( pbc.second )) .empty () )
		return ALIVE; 
	else 
		return DEAD; 
}

bool	AREA::onland(const PBC& pbc, const POS& pos) const {
	// 试试不单独剔出来而是并行计算的效果（适用于求各群的参数一样时）
	return (pbc.first & getbb(pbc.second )).land (3)[pos]; 
}

BITB	AREA::getbb( COLOR clr) const {
	___PARASSERT(clr == BLACK || clr == WHITE); 
	if (clr == BLACK)
		return massmaps.front ().xx; 
	else 
		return massmaps.front ().oo; 
}

void	AREA::setworknumb (const POS& pos) {
	// 根据局部和全局特征寻找一个合适的位置
	worknumb = massmaps.size()/2; 
}

bool	AREA::newmass(const POS& pos, COLOR clr) const{
	return (getmass(pos, clr).first & getbb(clr)) .empty(); 
}

//vector< pair<int, PBC> > AREA::dist_mass(const POS& pos, COLOR clr) const{
//	// 只考虑相邻者
//	vector< pair<int, PBC> > vec; 
//	PBC pbc = getmass(pos, clr); 
//	GO tmp = massmaps[worknumb]; 
//	ITR itr = ITR(tmp.xx.blockon(pbc.first .dilate(1))); 
//	for (BITB bb = itr.blockbegin (); !itr.blockend(); bb = itr.blocknext())
//		vec.push_back (make_pair(bb.dist (pos), make_pair(bb, BLACK))); 
//	itr = ITR(tmp.oo.blockon(pbc.first.dilate(1))); 
//	for (BITB bb = itr.blockbegin (); !itr.blockend(); bb = itr.blocknext())
//		vec.push_back (make_pair(bb.dist (pos), make_pair(bb, WHITE))); 
//	sort(vec.begin (), vec.end ()); 
//	return vec; 
//}
//
//	// 提取的 cluster 不准确
//vector< pair<int, PBC> > AREA::dist_cluster(const POS& pos) const{
//	// 只考虑相邻者 
//	vector< pair<int, PBC> > vec; 
//	BITB unclear = clustermap.xx & clustermap.oo; 
//	PBC pbc = getcluster(pos); 
//	ITR itr = ITR((clustermap.xx^unclear).blockon(pbc.first .dilate(1))); 
//	for (BITB bb = itr.blockbegin (); !itr.blockend(); bb = itr.blocknext())
//		vec.push_back (make_pair(bb.dist (pos), make_pair(bb, BLACK))); 
//	itr = ITR((clustermap.oo^unclear) .blockon(pbc.first.dilate(1))); 
//	for (BITB bb = itr.blockbegin (); !itr.blockend(); bb = itr.blocknext())
//		vec.push_back (make_pair(bb.dist (pos), make_pair(bb, WHITE))); 
//	itr = ITR(unclear.blockon(pbc.first .dilate (1))); 
//	for (BITB bb = itr.blockbegin (); !itr.blockend(); bb = itr.blocknext())
//		vec.push_back (make_pair(bb.dist (pos), make_pair(bb, BLACK_WHITE))); 
//	sort(vec.begin (), vec.end ()); 
//	return vec; 
//}

