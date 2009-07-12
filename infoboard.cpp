#include "infoboard.h"
#include "inout.h"

#if		INFO_MACRO == 0
#undef	___FUNCOUNT
#define ___FUNCOUNT(fun) {}
#undef	___TIME
#define ___TIME(code)	{}
#endif

/* class INCREMENT */

INCREMENT::INCREMENT(){
	tag.assign (BS*BS+1, NULL_TAG); 
	// color ��������Ա��һ������Ϊ���Ӵ�����ɫ��������ģ�
	// �����Ӵ���������Ա��û������ģ�����Ӧ��NULL����ʾ��
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

// ����tag, color ���� 4 ��������һ����ȫ��ȣ�����ֱ�ӱȽ�
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
		// δ��������
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
	___FUNCOUNT( INFO_INFOGO1); 
}

INFOGO::INFOGO(const GO& go):father(0){ //-//
	___FUNCOUNT( INFO_INFOGO2); 
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
	___FUNCOUNT( INFO_SCAN); 
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
	___FUNCOUNT( INFO_MOVE); 
	
	int ip = pos2i(pos); 
	COLOR ic = inc.getcolor (ip); 
	
	// ___________________________________
	// 
	// tmp : mvable, tmphotko
	// kill
	// ___________________________________

	// �ӽ���ٽ�ʱ���������ӣ�����
	if (ic != EMPTY)	             
		return 0; 
	
	// ȡ������λ���������ڿ�
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
		// ���Ϊ��λ��������  // ���λ�����أ����������ڽ�λ֮�ԡ�
		if (c == EMPTY ) 
			mvable = 1; 
		// ���Ϊ 1 ������ɫ��
		else if ( (c == ~clr) && (inc.getliberty (n[i]) == 1) ) {
			// ������
			mvable = 1; 
			// ��¼���ܽ�λ
			tmphotko = near4p[i]; 
			// ��¼Ϊ����飨�ϳ���ÿ�飬�����п���ͬ��
			kill |= inc.getcontent (n[i]); 
			// ��¼�˿����ĵп�
			killenemysite |= inc.getenemysite (n[i]); 
		}
		// �������Ϊ���� 1 ����ͬɫ�飬������
		else if ((c == clr) && (inc.getliberty(n[i]) > 1))
			mvable = 1; 
		// ���жϣ������ڿ�����
	}

	// ����������ӣ�����
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
	
	// �Ը������ĸ�λ�õĶ�Ӧ��ɫ����ͷ
    ITR itr; 
	itr = ITR(kill); 
	for(POS p = itr.stonebegin(); !itr.stoneend(); p = itr.stonenext()) {
		int m = pos2i(p); 
		// ������ɫ
		inc.setcolor (m, EMPTY); 
		// ����ԭ��¼
		inc.settag (m, NULL_TAG); 
	}
	
	// �Կ����ĵп飨�����ӷ������ںϵĻ��ڸ������Ӵ�ʱ�������£�
	itr = ITR(killenemysite); 
	for(POS p = itr.tagbegin(); !itr.tagend(); p = itr.tagnext()) {
		int m = pos2i(p); 
		//  tmp:�˵п�ĵп顡�롡�������غϲ�
		BITB tmp = (inc.getenemysite (m) & kill); 
		//  ���´˵п�ĵп飺ȥ�� tmp
		inc.setenemysite(m, inc.getenemysite (m)^tmp); 
		//  ���´˵п�����㣺ѹ��tmp 
		inc.setlibertysite (m, inc.getlibertysite(m)|tmp); 
		//  ���´˵п����������tmp�ĵ���
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
	
	// �������Ӵ������ܱ�λ����Ϣ
	BITB  newcontent = BITB(pos); 
	BITB  newlibertysite = NULL_BB; 
	BITB  newenemysite = NULL_BB; 
	int	  newtag = ip; 
	// �Ը�����λ
	for(int i = 0; i<near4p.size (); ++i){
		COLOR c = inc.getcolor (n[i]); 
		// ���Ϊ��λ // ���λ�����أ���Ϊ��λ�Ա�һ������
		if ((c == EMPTY))  
			// ��¼Ϊ������֮һ
			newlibertysite |= near4p[i]; 
		// ���Ϊ��ɫ��
		else if (c == ~clr) {
			// ��¼Ϊ�µ���֮һ
			newenemysite |= near4p[i]; 
			// ͬʱ����Ϊ�п�ĵ���֮һ
			inc.setenemysite (n[i], inc.getenemysite (n[i])|pos); 
			// ������������Ӵ��ǵп�֮���㣨ͬһ��ȷ��ֻ����һ�Σ�
			if (inc.getlibertysite(n[i])[pos]){			
				// ȥ��������
				inc.setlibertysite(n[i], inc.getlibertysite(n[i])^pos); 
				// ����һ��
				inc.setliberty (n[i], inc.getliberty (n[i])-1); 
			}
		}
		// �����ͬɫ��
		else {
			// ��������ͷ��//-// < ���� ��Ŀǰ�� tag() ��������ȷ
			if (inc.gettag(n[i]) < newtag)
				newtag = inc.gettag(n[i]); 
			// ��������飺ѹ��ԭ���
			newcontent |= inc.getcontent (n[i]); 
			// ���������飺ѹ��ԭ����
			newlibertysite |= inc.getlibertysite(n[i]); 
			// �����µп飺ѹ��ԭ�п�
			newenemysite |= inc.getenemysite (n[i]); 
			// ����ԭ��¼��Ϣ��ͬһ�����Ϣֻ������һ�Σ��ظ�Ҳû��
			// inc.settag(n[i], NULL_TAG); 
		}
	}
	// ���������������������Ӵ����޳���������Ϊ����ʱ���ܲ�����
	if (newlibertysite[pos]) 
		newlibertysite ^= pos; 

	// ���������ĸ�λ�õ���ͷ�������ӷ������ںϵĻ��ڴ˽��������£�
	itr = ITR(newcontent); 
	for(POS pp = itr.stonebegin(); !itr.stoneend(); pp = itr.stonenext()) {
		int m = pos2i(pp); 
		inc.settag (m, newtag); 
	}
	// ����������������Ϣ
	inc.setcolor(ip, clr); 
	inc.setcontent (ip, newcontent); 
	inc.setlibertysite (ip, newlibertysite); 
	inc.setliberty (ip, newlibertysite.count ()); 
	inc.setenemysite (ip, newenemysite); 

	// ______________________________________
	// 
	// xx, oo, hotko, history, lastpos, lastclr
	// ______________________________________

	// ������£����ӣ���¼��ʷ���߲�����
	if (clr == BLACK) 
		xx |= pos, oo ^= kill; 
	else 
		oo |= pos, xx ^= kill; 
    ++history; 
	lastpos = pos; 
	lastclr = clr; 
	// �������Ϊ���ӣ�����Ϊ���ӣ�������Ϊ 1 ������ԭ����λΪ��λ
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
	___FUNCOUNT( INFO_PARTITION); 
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
// ɫ��ȷ��
PBB	INFOGO::lastpartition(int n) const {	
	___FUNCOUNT( INFO_LASTPARTITION); 
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
	___FUNCOUNT( INFO_CLUSTER); 
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
	___FUNCOUNT( INFO_AREA); 
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

// ǰ 4 ��Ӧ�Ƕ�ʽ���� 4 ��Ӧ�߶�ʽ�����Ϊȫ�ֶ�ʽ
// δ��������
// δ���ǽٵ�
// δ�������ӷ�(����ѧϰʱ���ݼ��㹻�󣬸��ֶԳ�������ѳ��ֹ���
// ������ɫ��������û�б�Ҫ������Щ, ��Ӧ�÷�ʽû��Ӱ�죩 
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

// ��������ť�����򣬿�����Щ����ֹ����
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

// δ�޳�����㼰���ӵ㣬��������Ǳ��ť������
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
	// ���¼�����Ȼ�������������ϣ��ѿ���������ϣ����������ѿ����磨ȫ����
	BITB cut = cutpos(getlastclr()).blockon(getlastpos()); 
	if (cut[getlastpos()]) {
		PBC f_his = father->area .getmass (cut, turn()); 
		PBC s_my = area.getmass (cut, getlastclr()); 
		vt.push_back (TARGET(DUAN, f_his, s_my)); //�����ߣ�����
	}
	// �ѿ����磬���ڸ���ʲôɫ�����п���
	if ( fmass.first .blockcount() > 1)
		vt.push_back (TARGET(LIANLUO, fmass, smass)); //�����������Ч��
	// �����������ڸ���ʲôɫ�����п���
	if ( fmass_his.first .blockcount() < smass_his.first .blockcount() )
		vt.push_back (TARGET(GE, fmass_his, smass)); //�������������
	return vt; 
}
*/

/* class AREA */

PBC		AREA::getmass(const BITB& mask, COLOR clr) const {
	___PARASSERT(clr == BLACK || clr == WHITE); 
	return make_pair(massmaps[worknumb].getbb(clr).blockon (mask), clr); 
}

	// ����ֵ�е� COLOR ָBITB �� clustermap �ϵ� color
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

	// д����������ǹ�����������massmapд�룬
	// ����д�롰�ʵ����֡�massmap����
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

	// ���룺�����ص�
void	AREA::setpotentialmap( ) {
	potentialmap.xx = massmaps.back ().xx 
		^ (massmaps.back ().xx & clustermap.oo)		// ��ȥ������δ����
		| clustermap.xx; // ���ϻ�����δ�������ͶԷ�����
	potentialmap.oo = massmaps.back ().oo 
		^ (massmaps.back ().oo & clustermap.xx)		// ��ȥ������δ����
		| clustermap.oo; // ���ϻ�����δ�������ͶԷ�����
	// ������δ������Ϊ�Է�ɫ
	// ע�������˫��δ�����త
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

	//��ע�������˫��δ�����త����Ӱ����㷨
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
	// ���Բ������޳������ǲ��м����Ч�������������Ⱥ�Ĳ���һ��ʱ��
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
	// ���ݾֲ���ȫ������Ѱ��һ�����ʵ�λ��
	worknumb = massmaps.size()/2; 
}

bool	AREA::newmass(const POS& pos, COLOR clr) const{
	return (getmass(pos, clr).first & getbb(clr)) .empty(); 
}

//vector< pair<int, PBC> > AREA::dist_mass(const POS& pos, COLOR clr) const{
//	// ֻ����������
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
//	// ��ȡ�� cluster ��׼ȷ
//vector< pair<int, PBC> > AREA::dist_cluster(const POS& pos) const{
//	// ֻ���������� 
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

/* _______________________________ ������ _______________________________ */

void TEST_INFO::timefunc (string which) {
	  
	RANDER  r; 
	
	INFOGO	info = INFOGO(r.vg[0]); 

	//___TIME( INFO_SCAN, info.scan(); ); 
	//___TIME( INFO_MOVE, info.move  ( r.vp[0], r.vc[0] ); ); 
	___TIME( INFO_PARTITION, info.partition(r.vi[0] ); ); 
	/*
	___TIME( INFO_LASTPARTITION, info.lastpartition(r.vi[0]); ); 
	___TIME( INFO_CLUSTER, info.getallmass(make_pair(r.vb[0], r.vb[1])); ); 
	___TIME( INFO_AREA, info.setarea(r.vi [0], r.vi[1], r.vi[2]); ); 
	*/
}

