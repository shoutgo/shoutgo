#include "stdafx.h"
#include "test.h"
#include "inout.h"

NEWPOS make_pair(int a, unsigned b)
{
	NEWPOS np; 
	np.first = a; 
	np.second = b; 
	return np; 
}

void testtree(){ //for thesis
		// ��Щ�����㷨��ֻ����Ҷ����ֵ���ݣ�����ǰ���㷨Ӱ��
	___REPORT(
		TREE tree; 
		srand((unsigned)time(0)); 
		NODE* p = tree.root();
		___COUT1(tree.nodes);
		);

	VALUE a,b,c,u,v,w,x,y;
	
		// Ҷ���ֵΪ��ʵֵ��δ�ж��Ƿ�Ϊ��תֵ
	VALUE widea = TREE::VALUE_MIN; 
	VALUE wideb = TREE::VALUE_MAX; 
	VALUE narrowa = 10; 
	VALUE narrowb = 20; 
	
	___REPORT(  
		tree.visited = 0;
		___.tic("minmax");
		a = tree.minmax(p); 
		___.toc();
		___COUT1(tree.visited);
		);

						// ����ֵ����������ǳ��Ҷ���ʱ����ֵ����Խ�磬
						// �����������ֵ�ᱻĨƽ���򷵻�ԭʼ��ֵ
	___REPORT(  
		tree.visited = 0;
		___.tic("alphabeta_minmax");
		b = tree.alphabeta_minmax(p, widea, wideb); 
		___.toc();
		___COUT1(tree.visited);
		); 
						// ����ֵ����������ֵ����ʵֵ�߹��͹����п���
						// ��һ�����ڽ���ͬ��
	___REPORT(  
		tree.visited = 0;
		___.tic("failsoft_minmax");
		c = tree.failsoft_minmax(p, widea, wideb); 
		___.toc();
		___COUT1(tree.visited);
		); 
/*
		// ________ class TREESEARCH _________
		// Ҷ���ֵΪ��ʵֵ
	TREESEARCH treesearch; 
	___REPORT( VALUE aa = treesearch.minmax(p); cout<< aa << endl; ); 
	___ASSERT(a == aa); 
		// ________ class TREESEARCH _________
*/
		// ���� min Ҷ���Ϊ��תֵ���緭ת�����ٷ�ת
		// ��ͷ���Ϊ min ���ʱ������Ľ�ֵʵΪ(-beta, -alpha)
		// ������������㹻�󣬽������һ�£�������ܲ���һ��
	VALUE newwidea = p->ismax ? widea : -wideb; 
	VALUE newwideb = p->ismax ? wideb : -widea; 
	VALUE newnarrowa = p->ismax ? narrowa : -narrowb; 
	VALUE newnarrowb = p->ismax ? narrowb : -narrowa; 
	___REPORT(  
		tree.visited = 0;
		___.tic("negamax");
		u = tree.negamax(p); 
		___.toc();
		___COUT1(tree.visited);
		); 
	___REPORT(  
		tree.visited = 0;
		___.tic("alphabeta_negamax");
		v = tree.alphabeta_negamax(p, newwidea, newwideb); 
		___.toc();
		___COUT1(tree.visited);
		); 
	___REPORT(  
		tree.visited = 0;
		___.tic("failsoft_negamax");
		w = tree.failsoft_negamax(p, newwidea, newwideb); 
		___.toc();
		___COUT1(tree.visited);
		); 
						// ���ֵܽ������ʱ�����Ľ����������
	___REPORT(  
		tree.visited = 0;
		___.tic("negascout");
		x = tree.negascout(p, newwidea, newwideb); 
		___.toc();
		___COUT1(tree.visited);
		); 
	___REPORT(  
		tree.visited = 0;
		___.tic("mtdf");
		y = tree.mtdf(p); 
		___.toc();
		___COUT1(tree.visited);
		); 
						// һ�������ԣ�����ֵ�����ڽ����
	//___REPORT(		   tree.negamax2(p); ); 
		
		// ������Ϊ widea, wideb ʱ����ʽ������
	___ASSERT(a == b && a == c); 
	___ASSERT(u == v && u == p->value && u == w && u == x && u == y); 
	___ASSERT(a == u * ( p->flip ? -1 : 1)); 
	___ASSERT(c == w * ( p->flip ? -1 : 1)); 
	___COUT8(a,b,c,u,v,w,x,y);
}
/*
void testtree(){
		// ��Щ�����㷨��ֻ����Ҷ����ֵ���ݣ�����ǰ���㷨Ӱ��
	___REPORT(
		TREE tree; 
		srand((unsigned)time(0)); 
		NODE* p = tree.root(); 
	)

		// Ҷ���ֵΪ��ʵֵ��δ�ж��Ƿ�Ϊ��תֵ
	VALUE widea = TREE::VALUE_MIN; 
	VALUE wideb = TREE::VALUE_MAX; 
	VALUE narrowa = 10; 
	VALUE narrowb = 20; 
	___REPORT( VALUE a = tree.minmax(p); ); 
						// ����ֵ����������ǳ��Ҷ���ʱ����ֵ����Խ�磬
						// �����������ֵ�ᱻĨƽ���򷵻�ԭʼ��ֵ
	___REPORT( VALUE b = tree.alphabeta_minmax(p, widea, wideb); ); 
						// ����ֵ����������ֵ����ʵֵ�߹��͹����п���
						// ��һ�����ڽ���ͬ��
	___REPORT( VALUE c = tree.failsoft_minmax(p, widea, wideb); ); 

		// ________ class TREESEARCH _________
		// Ҷ���ֵΪ��ʵֵ
	TREESEARCH treesearch; 
	___REPORT( VALUE aa = treesearch.minmax(p); cout<< aa << endl; ); 
	___ASSERT(a == aa); 
		// ________ class TREESEARCH _________

		// ���� min Ҷ���Ϊ��תֵ���緭ת�����ٷ�ת
		// ��ͷ���Ϊ min ���ʱ������Ľ�ֵʵΪ(-beta, -alpha)
		// ������������㹻�󣬽������һ�£�������ܲ���һ��
	VALUE newwidea = p->ismax ? widea : -wideb; 
	VALUE newwideb = p->ismax ? wideb : -widea; 
	VALUE newnarrowa = p->ismax ? narrowa : -narrowb; 
	VALUE newnarrowb = p->ismax ? narrowb : -narrowa; 
	___REPORT( VALUE u = tree.negamax(p); ); 
	___REPORT( VALUE v = tree.alphabeta_negamax(p, newwidea, newwideb); ); 
	___REPORT( VALUE w = tree.failsoft_negamax(p, newwidea, newwideb); ); 
						// ���ֵܽ������ʱ�����Ľ����������
	___REPORT( VALUE x = tree.negascout(p, newwidea, newwideb); ); 
	___REPORT( VALUE y = tree.mtdf(p); ); 
						// һ�������ԣ�����ֵ�����ڽ����
	___REPORT(		   tree.negamax2(p); ); 
		
		// ������Ϊ widea, wideb ʱ����ʽ������
	___ASSERT(a == b && a == c); 
	___ASSERT(u == v && u == p->value && u == w && u == x && u == y); 
	___ASSERT(a == u * ( p->flip ? -1 : 1)); 
	___ASSERT(c == w * ( p->flip ? -1 : 1)); 
	___COUT8(a,b,c,u,v,w,x,y);
}
*/
void testnewpos(){
	___REPORT(
		NEWPOS newpos; 
		newpos.first = 1; 
		newpos.second = 0x10; 
		___COUT1(newpos); 
		___ASSERT (typeid(PII) == typeid(make_pair(4, 5))); 
		___ASSERT (typeid(NEWPOS) == typeid(make_pair(4, (unsigned)5))); 
		___ASSERT (typeid(PUU) == typeid(make_pair((unsigned)4, (unsigned)5))); 
		unsigned n = 5; 
		___ASSERT (typeid(NEWPOS) == typeid(make_pair(4, n))); 
		); 
}

void testsize(){
	___COUT2(
		(ULL) 512 * 1024 * 1024 * sizeof(long double), // memory 500M
		UINT_MAX
		); 
	___COUT10(		
		sizeof(bool ), 
		sizeof(int ), 
		sizeof(long ), 
		sizeof(float ), 
		sizeof(double ), 
		sizeof(long double), 
		sizeof(BITB), 
		sizeof(GO), 
		sizeof(INFOGO),
		sizeof(INCREMENT)
		); 
	___COUT10(
		sizeof(INOUT ), 
		sizeof(ITR ), 
		sizeof(FIR ), 
		sizeof(TSUMEGO ), 
		sizeof(NN ), 
		sizeof(TRANSPTABLE<BITB, int> ), 
		sizeof(SEARCH<RESULT, INFOGO, MOVE, TARGETS> ), 
		sizeof(TARGETSEARCH ), 
		sizeof(LEARN), 
		sizeof(BRAIN )
		); 
}

		// ͬһʵ���ļ��ڵĺ��� static ����ֵ�ſ��Ա���
void t_t_teststatic(){						
	___REPORT(
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		st_g_ctr.fcount [U_RANDU] += 100; 
		g_ctr.fcount [U_RANDU] += 100; 
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		t_teststatic(); 
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		); 
}

void t_teststatic(){
	___REPORT(
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		st_g_ctr.fcount [U_RANDU] += 400; 
		g_ctr.fcount [U_RANDU] += 400; 
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		teststatic(); 
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		); 
}


void teststatic(){
	___REPORT(
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		st_g_ctr.fcount [U_RANDU] += 100; 
		g_ctr.fcount [U_RANDU] += 100; 
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		//TIMER()(10, "rand_u", randu, 33); 
		___COUT2(st_g_ctr.fcount [U_RANDU], g_ctr.fcount [U_RANDU]); 
		___ASSERT(st_g_ctr.fcount [U_RANDU] == 5); 
		); 
}

void testc (){
		// time() ������ 1970 �� 1 �� 1 �� 00:00:00����ǰʱ���������
		// clock() �����Գ������е���ʱ�� CPU ʱ�Ӽ�ʱ��Ԫ����
		// time.h ���� typedef long clock_t; CLOCKS_PER_SEC
	
	___REPORT(
		___COUT1(CLOCKS_PER_SEC); 
		int w = 4; 
		clock_t goal; 
		goal = w * CLOCKS_PER_SEC + clock(); 
		cout<<"wait for "<<w <<" second ..."<< endl; 
		while( goal > clock() ); 
		cout<<"time over now"<< endl; 
		
		long double i = 50000000; 
		//clock_t start, finish; // ������֮��Ķ�������ͻ
		clock_t start; 
		clock_t finish; 
		start = clock(); 
		while( i-- ); 
		finish = clock(); 
		___COUT1((double)(finish - start)/CLOCKS_PER_SEC); 
		); 

		// �����͵��໥��ֵ
		// ���������'s''r''r''h'ƴ��, ���ܳ��� 5 ��
		// a Ϊ char ����ʱȡ��� 8 λ���ʻ��� 'h'
	___REPORT(
		int a = 'srrh'; 
		char b = a; 
		___COUT2(a, b); 
		); 
		// ��������λ��Ϊunsigned
	___REPORT(							
		float f = 374.342; 
		___COUT4(						
			(*(unsigned *)&f),			// ��ȷ
			(*(unsigned *)((void*)&f)), // ��ȷ
			((unsigned)f),				// ����
			(static_cast<unsigned>(f))  // ����
			); 
		); 
}

void testcpp(){
		// ��ֵ�밴���÷���
	___REPORT(							
		TEST t; 
		int a = 30; 
		int b = t.add100(a); 
		___COUT2(a, b); 
		___ASSERT(a == 130 && b == 130); 
		b += 7; 
		___COUT2(a, b); 
		___ASSERT(a == 130 && b == 137); 
		int& c = t.add100(a); 
		___COUT2(a, c); 
		___ASSERT(a == 230 && c == 230); 
		c += 7; 
		___COUT2(a, c); 
		___ASSERT(a == 237 && c == 237); 
		int k = a+3; 
		___COUT2(a, c); 
		___ASSERT(a == 237 && c == 237); 
		a += 3; 
		___COUT2(a, c); 
		___ASSERT(a == 240 && c == 240); 
		); 
}

void teststl(){
	//___REPORT(				
		map<int, int> mapm; 
		mapm[3] = 30; 
		mapm[4] = 40; 
		VI	v; 
		v.assign (100, 33); 
		set<int>    sets(v.begin (), v.end ()); 
		___COUT6(					
			mapm[1], 
			mapm[3], 
			mapm[4], 
			mapm.size(), 
			v.size(), 
			sets.size()
			); 
	//); 
}

void testload (){/*
	___REPORT(
		VI vi; 
		VI loadedvi; 
		vi.assign (4, -3); 
		vi.resize(7); 
		INOUT().save<int>(vi, "vi"); 
		INOUT().load<int>(loadedvi, "vi"); 
		___COUT2(vi, loadedvi); 
		___ASSERT(vi == loadedvi); 

		VVI vvi; 
		VVI loadedvvi; 
		vi.resize(4); 
		vvi.push_back (vi); 
		vi.resize(2); 
		vvi.push_back (vi); 
		vi.resize(7); 
		vvi.push_back (vi); 
		INOUT().save<int>(vvi, "vvi"); 
		INOUT().load<int>(loadedvvi, "vvi"); 
		___COUT2(vvi, loadedvvi); 
		___ASSERT(vvi == loadedvvi); 
	); */
}

void testcounter (){
	___REPORT(
		st_g_ctr.reportfcount(); 
		g_ctr.reportfcount(); 
		GO go; 
		go.random (); 
		st_g_ctr.reportfcount(); 
		g_ctr.reportfcount(); 
		U_TEST().timefunc(); 
		g_ctr.reportfcount(); 
		g_ctr.clear (); 
		g_ctr.reportfcount(); 
	); 
}

void testrand (){
	___COUT2(ULLONG_MAX, UINT_MAX); 
	UL seed32 = 333; 
	for (int i = 0; i<100; ++i)
		cout<< rand32(seed32) << endl; 

	for (int i = 0; i<100; ++i)
		cout<<randf()<< endl; 
}

void test_array_if (){
		// DEBUG ʱ������������Ч��turncolor() ʹ���ܴ���½������겻��
		// RELEASE ʱ���ߴ���һ����������Ч��
		// �� COLOR ���������~ �� debug ʱ��������turncolor()��
		// RELEASE ʱҲ����½����������������������
	srand(unsigned(time(0))); 
	bool p = rand()&1; 
	COLOR clr; 
	if (p)
		clr = BLACK; 
	else 
		clr = WHITE; 
	GO go; 
	go.random (); 
	BITB xo[2] = {go.xx, go.oo}; 
	BITB a; 
	int t = 100000000; 
	___.tic("from []", t); 
	for (int i = 0; i<t; ++i){
		a = xo[p]; 
		p = !p; 
	}
	___.toc(); 
	___.tic("from if", t); 
	for (int i = 0; i<t; ++i){
		if (clr == BLACK)
			a = go.xx; 
		else 
			a = go.oo; 
		//clr = clr == WHITE ? BLACK : WHITE; 
		clr = ~clr; 
	}
	___.toc(); 
}

void testindex8 (){
	VVI vvi = index8(5, 5); 
	for (int i = 0; i<8; ++i)
		___COUT1(vvi[i]); 
}

void testtransptable(){
	TRANSPTABLE<BITB, int> tt; 
	RANDER rnd; 
	___COUT1(tt.recorded(rnd.vb[0])); 
	tt.record (rnd.vb[0], 32434); 
	___COUT1(tt.recorded(rnd.vb[0])); 
	___COUT1(tt.get(rnd.vb[0])); 

	for (int i = 0; i<5; ++i)
		//ע����������
		cout << HASH<BITB>()(rnd.vb[1]) << endl; 
}

/* class TEST */

		// �������� �� ����ָ�� ԭ�����һ����
		// �﷨�ϲ�����Ҫ * ��ȡַ������
		// �������� ���ܷ�����ʱ��������Ч����
		// �������� �ʺ��ڷ��ؽϴ� sizeof �Ķ���
		// ������ 4 BYTE����Ϊָ��һ��� 4 BYTE
		// �����ô��ݲ��� �� ��ֵ���ݲ���  ����Ҳһ����
		// ע���Ҫʱ�� const ��ֹ�ı����ֵ
		// int& q = t.minus5(p); ����䲻�Ϸ�
int& TEST::add100(int& a) const { 
	a += 100; 
	return a; 
}

int	 TEST::minus5(int& a) const {
	a -= 5; 
	return a; 
}

		// s == ""          ����ȫ������
		// s == "u""bb"...  ������Ӧ��
		// s == "U_RANDU"	  ����ָ������
void TEST::time (string s){
	tested_ctr.clear(); 
	swap(snap_ctr, g_ctr); 
	clog << setx(G) << "testing ..." << setx(W)<< endl; 

	if ((s == "") ||(s == "u") || (s.find("U_") != s.npos))	
		tu.timefunc(s); 
	if ((s == "") ||(s == "bb") || (s.find("BB_") != s.npos))	
		tbb.timefunc(s); 
	if ((s == "") ||(s == "go") || (s.find("GO_") != s.npos))	
		tgo.timefunc(s); 
	if ((s == "") ||(s == "info") || (s.find("INFO_") != s.npos))	
		tinfo.timefunc(s); 
	if ((s == "") ||(s == "io") || (s.find("IO_") != s.npos))	
		tio.timefunc(s); 
	if ((s == "") ||(s == "itr") || (s.find("ITR_") != s.npos))	
		titr.timefunc(s); 
	if ((s == "") ||(s == "s") ||(s.find("_S_SIZE") == s.npos)  
		|| ((s.find("S_") != s.npos) && (s.find("_SIZE") == s.npos) ))	
		ts.timefunc(s); 
	if ((s == "") ||(s == "ts") || (s.find("TS_") != s.npos))	
		tts.timefunc(s); 
	if ((s == "") ||(s == "tt") || (s.find("TT_") != s.npos))	
		ttt.timefunc(s); 
	if ((s == "") ||(s == "nn") || (s.find("NN_") != s.npos))	
		tnn.timefunc(s); 
	if ((s == "") ||(s == "l") || (s.find("L_") != s.npos))	
		tl.timefunc(s); 
	if ((s == "") ||(s == "br") || (s.find("BR_") != s.npos))	
		tbr.timefunc(s); 
	if ((s == "") ||(s == "f") || (s.find("F_") != s.npos))	
		tf.timefunc(s); 
	if ((s == "") ||(s == "tg") || (s.find("TG_") != s.npos))	
		ttg.timefunc(s); 

	clog<<setx(R)<<"no tested :"<<setx(W)<< endl; 
	(snap_ctr - tested_ctr).reportfcount(); 
	swap(snap_ctr, g_ctr); 
}

void TEST::active() {
	//tu.active(); 
	//tbb.active(); 
	tgo.active(); 
	tinfo.active(); 
	//tio.active(); 
	titr.active(); 
	ts.active(); 
	tts.active(); 
	//ttt.active(); 
	tnn.active(); 
	//tl.active(); 
	tbr.active(); 
	tf.active(); 
	ttg.active(); 

	//testnewpos(); 
	//testtree(); 
	//t_t_teststatic(); 
	//t_teststatic(); 
	//teststatic(); 
	//testc (); 
	//testcpp(); 
	//teststl(); 
	//testsize(); 
	//testload (); 
	//testcounter (); 
	//testrand (); 
	//test_array_if (); 
	//testcout (); 
	//testindex8 (); 
	//testtransptable(); 

	test5 (); 
	test6 (); 
	test7 (); 
}

void test5 (){
}

void test6 (){
}

void test7 (){
}


