#include "define.h"
#include "bitboard.h"
#include "board.h"
#include "inout.h"

void ___assert(bool exp, string s, string _f_, long _l_){
	if (!exp){
		cerr << setx(R) << "___assert(" << s << ") is failure ! "; 
		if ( ! _f_.empty ())
			cout << " ["<< _f_ <<" ("<< _l_ <<")]"; 
		cout << setx(W) << endl; 
		//err_ctr = snap_ctr; 
		//g_ctr = snap_ctr; 
	} 
}

void ___parassert(bool exp, string s){
	if (!exp){
		cerr << setx(R) << "___parassert(" << s << ") is failure ! "
			<< setx(W) << endl; 
		err_ctr = g_ctr; //-//
		//err_ctr = snap_ctr; 
		//g_ctr = snap_ctr; 
		g_ctr.reportftrack(30); 
	} 
}

void TIMER::set_(){
}

void TIMER::tic(string s, int n)	{
	cout<<std::setw(20) << std::left << s 
		<<std::setw(10) << std::right << n <<" times :"; 
	ticat = clock(); 
}

void TIMER::toc(){
	dif = clock()-ticat; 
	cout<<std::setw(10) << std::right << dif <<" CPUs"<< endl; // ms
	ticat = clock(); 
}

void TIMER::silenttoc_(){
	dif = clock()-ticat; 
	ticat = clock(); 
}		

const int COUNTER::TRACK_MAX = 1000;
const int COUNTER::LINES_MAX = 100;

COUNTER::COUNTER(){
	lcount.assign (LINES_MAX, 0); 
	fcount.assign (FSIZE*FUN_MAX, 0); 
	fname.assign (FSIZE*FUN_MAX, ""); 
	ftrack.assign (TRACK_MAX, -1); 
}

void COUNTER::setfcountall(int n){
	fcount.assign (FSIZE*FUN_MAX, n); 
}

void COUNTER::setlcount(int p){
	___PARASSERT( p < LINES_MAX ); 
	++ lcount[p]; 
	cout<<"lcount["<< setx(G) <<p <<setx(W) <<"]: "
		<< setx(G) << lcount[p] <<setx(W) << endl; 
}

void COUNTER::clear (){	 	
	COUNTER cntr = COUNTER(); //2009//
	swap(cntr, *this); 
}

void COUNTER::reportfcount(FUNGROUP fg){
	static int x[FSIZE]; 
	x[FU] = U_FUN_SIZE; 
	x[FBB] = BB_FUN_SIZE; 
	x[FGO] = GO_FUN_SIZE; 
	x[FINFO] = INFO_FUN_SIZE; 
	x[FIO] = IO_FUN_SIZE; 
	x[FITR] = ITR_FUN_SIZE; 
	x[FS] = S_FUN_SIZE; 
	x[FTS] = TS_FUN_SIZE; 
	x[FTT] = TT_FUN_SIZE; 
	x[FL] = L_FUN_SIZE; 
	x[FNN] = NN_FUN_SIZE; 
	x[FBR] = BR_FUN_SIZE; 
	x[FF] = F_FUN_SIZE; 
	x[FTG] = TG_FUN_SIZE; 
	if (fg != FSIZE){
		for (int i = fg * FUN_MAX; i< x[fg]; ++i)
			cout<<::setw(20)<<::left<< fname[i]	
				<<::setw(10)<<::right<< fcount[i] << endl; 
	}
	else 
		for (int i = 0; i<FSIZE; ++i){
			reportfcount((FUNGROUP)i); 
			cout<< endl; 
		}
}

void COUNTER::reportftrack(int n){
	___ASSERT( ftrack.size () == TRACK_MAX ); 
	for (int i = ftrack.size ()-1; n>0 && i >= 0; --i, --n) {
		int width = ftrack[i]/FUN_MAX; 
		if (ftrack[i] != -1)
			cout<< ::setw( 20 + width*4 ) << fname[ftrack[i]] << endl; 
	}
}

COUNTER operator - (const COUNTER& a, const COUNTER& c) { 
	//-// ����������Աδ����
	COUNTER tmp = a; 
	transform(a.lcount.begin(), a.lcount.end(), c.lcount .begin (), 
		tmp.lcount.begin (), minus<int>()); 
	transform(a.fcount.begin(), a.fcount.end(), c.fcount .begin (), 
		tmp.fcount .begin (), minus<int>()); 
	return tmp; 
}

RANDER::RANDER(int n, int nb, int ng){
	srand((unsigned)time(0)); 
	for (int i = 0; i<n; ++i){
		vi.push_back (rand()); 
		vu.push_back (rand()); 
		vp.push_back (make_pair( randu(BS), 1<<randu(BS) )); //ok
		vs.push_back ("sdfsdf+sdf"); 
		vd.push_back (CENTER); 
		vc.push_back (WHITE); 
		va.push_back (KILL); 
	}
	BITB bb; 
	for (int i = 0; i<nb; ++i){
		bb.random (); 
		vb.push_back (bb); 
	}
	GO go; 
	for (int i = 0; i<ng; ++i){
		go.random (); 
		vg.push_back (go); 
	}
}

INIT::INIT():inited(0){
	for (int i = 0; i<BS*BS; ++i){
		i2p.push_back (i2pos(i)); 
		i2near4.push_back (near4(i2pos(i))); 
	}
	ITR itr; 
	for (POS p = itr.posbegin (); !itr.posend(); p = itr.posnext ()){
		p2i[p] = pos2i(p); 
		p2near4[p] = near4(p); 
	}
	set_id8_q8(); 
	setzobristmask(); 
	set_xorand_key2rand(); // ���ܻᱻ LEARN() ���ǡ�
	setroute(); 
	inited = 1; 
}

POS INIT::getp(int i) const {
	return i2p[i]; 
}

VP INIT::getnear4(int i) const {
	return i2near4[i]; 
}

int INIT::geti(const POS& p) {
	return p2i[p]; 
}

VP& INIT::getnear4(const POS& p) {
	return p2near4[p]; 
}

void INIT::setzobristmask(int width){
	josekimask.assign (ZOBRIST::ZOBRIST_KIND, NULL_BB); 
	// �Ƕ�ʽ 
	josekimask[0] = strip(corner(EN), width, width, WS); 
	josekimask[1] = strip(corner(WN), width, width, ES); 
	josekimask[3] = strip(corner(WS), width, width, EN); 
	josekimask[2] = strip(corner(ES), width, width, WN); 
	// �߶�ʽ 
	josekimask[4] = strip(corner(EN), BS-1, width, WS); 
	josekimask[5] = strip(corner(WN), width, BS-1, ES); 
	josekimask[7] = strip(corner(WS), BS-1, width, EN); 
	josekimask[6] = strip(corner(ES), width, BS-1, WN); 
	// ���ֶ�ʽ 
	// �� mask Ŀǰδ����
	//josekimask[8] = ~(square(star(5), 3)); 
	//josekimask[8] = ~NULL_BB; 
	
	//for (int i = 0; i<ZOBRIST::ZOBRIST_KIND; ++i)
	//	___COUT2(i, josekimask[i]); 
}

// ___________
//
//     4
//   1---0
// 5 |   |  6
//   3---2
//     7
// ___________
void INIT::set_id8_q8 (){
	// ȫ��λ�������任
	id8 = index8(BS, BS); 
	// �Ƕ�ʽ mask
	q8 = index8(2, 2); 
	// �߶�ʽ mask
	for (int which = 0; which <8; ++which){
		for (int i = 4; i<8; ++i){
			int x, n; 
			switch (i) {
				case 4:
					x = max(q8[which][0], q8[which][1]); 
					n = min(q8[which][0], q8[which][1]); 
					break; 
				case 5:
					x = max(q8[which][1], q8[which][3]); 
					n = min(q8[which][1], q8[which][3]); 
					break; 
				case 6:
					x = max(q8[which][2], q8[which][0]); 
					n = min(q8[which][2], q8[which][0]); 
					break; 
				case 7:
					x = max(q8[which][3], q8[which][2]); 
					n = min(q8[which][3], q8[which][2]); 
					break; 
			}
			if (x == 1 && n == 0)
				q8[which].push_back( 4); 
			if (x == 3 && n == 1)
				q8[which].push_back( 5); 
			if (x == 3 && n == 2)
				q8[which].push_back( 7); 
			if (x == 2 && n == 0)
				q8[which].push_back( 6); 
		}
		// ȫ�� mask ��ĿǰδӦ��
		q8[which].push_back(8); 
	}
}

void INIT::set_xorand_key2rand(){
	UL seed = (unsigned)clock(); 
	for(int i = 0; i<BS*BS; ++i){									
			xrand.push_back (rand32(seed)); 
			key2rand[xrand[i]] = make_pair(i2pos(i), BLACK); 
			orand.push_back (rand32(seed)); 
			key2rand[orand[i]] = make_pair(i2pos(i), WHITE); 
	}
	key2rand[0] = NULL_MOVE; 

	// ����ظ���
	___ASSERT(key2rand.size () == (2*BS*BS+1)); 
	if (key2rand.size () != 2*BS*BS+1){
		key2rand.clear (); 
		set_xorand_key2rand(); 
	}
}

bool INIT::load_xorand_key2rand(string filename){
	ifstream file(filename.c_str (), ios::binary); 
	if (!file) {
		cerr<< filename << " : open fail !"<< endl; 
		return 0; 
	}
	INOUT().load(xrand, file); 
	INOUT().load(orand, file); 
	file.close(); 
	//
	for(int i = 0; i<BS*BS; ++i){									
			key2rand[xrand[i]] = make_pair(i2pos(i), BLACK); 
			key2rand[orand[i]] = make_pair(i2pos(i), WHITE); 
	}
	key2rand[0] = NULL_MOVE; 
	return 1; 
}

bool INIT::save_xorand(string filename){
	ofstream file(filename.c_str (), ios::binary); 
	if (!file) {
		cerr<< filename << " : open fail !"<< endl; 
		return 0; 
	}
	INOUT().save(xrand, file); 
	INOUT().save(orand, file); 
	file.close(); 
	return 1; 
}

void INIT::swap_xorand(){
	swap(xrand, orand); 
}

void INIT::swap_id8_q8(int i, int j){
	swap(id8[i], id8[j]); 
	swap(q8[i], q8[j]); 
}

BITB INIT::getjosekimask(int i) const {
	return josekimask[i]; 
}

// ��ǰ�ԳƷ�ʽ��ʵ�ʶ�Ӧ�Ķ�ʽ mask ����
// �˺�������һ�������ã�˵���Գ��Դ�����
int INIT::getq8(int i) const {
	return q8[0][i]; 
}

KEY	INIT::getrand(const POS& pos, COLOR clr) {
	if (clr == BLACK)
		return xrand[id8[0][geti(pos)]]; 
	else 
		return orand[id8[0][geti(pos)]]; 
}

MOVE& INIT::getmove(KEY key) {
	return key2rand[key]; 
}

void INIT::setroute(){
	route.push_back( frame (star(CENTER), MIDBS-2)); //R3
	route.push_back( frame (star(CENTER), MIDBS-3)); //R4
	route.push_back( frame (star(CENTER), MIDBS-4)); //R5
	route.push_back( square (star(CENTER), MIDBS-4)); //R4UP
	route.push_back( ~route[R4UP]); //R4DOWN
	route.push_back( strip (star(EAST), MIDBS, 0, MIDBS, MIDBS)); //R4DOWN_EAST
	route.push_back( strip (star(SOUTH), 0, MIDBS, MIDBS, MIDBS)); //R4DOWN_SOUTH
	route.push_back( strip (star(WEST), MIDBS, MIDBS, MIDBS, 0)); //R4DOWN_WEST
	route.push_back( strip (star(NORTH), MIDBS, MIDBS, 0, MIDBS)); //R4DOWN_NORTH
	//for (int i = 0; i<route.size(); ++i)
	//	___COUT1(route[i]); 
}

BITB INIT::getroute(ROUTE r){
	return route[r]; 
}

ostream& operator<< (ostream & os, setx s){
	os << s.c;
	return os; 
}

