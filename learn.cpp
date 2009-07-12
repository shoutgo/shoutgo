#include "learn.h"
#include "inout.h"
#include <unistd.h>	// sleep()	// VS2008

/* class LEARN */

LEARN::LEARN(string sgfpath){
	// ��Ϊ��ʽ����Ĺ�ϵ��Ҫ��֤�� load(), Ӧ���� &&
	if ( g_init.load_xorand_key2rand (LIBPATH + "xorand") &&
		lib_pemis.load( LIBPATH + "pemis") &&
		lib_zobrist[0].load( LIBPATH + "zobrist0") &&
		lib_zobrist[1].load( LIBPATH + "zobrist1") &&
		lib_zobrist[2].load( LIBPATH + "zobrist2") &&
		lib_zobrist[3].load( LIBPATH + "zobrist3") &&
		lib_zobrist[4].load( LIBPATH + "zobrist4") &&
		lib_zobrist[5].load( LIBPATH + "zobrist5") &&
		lib_zobrist[6].load( LIBPATH + "zobrist6") &&
		lib_zobrist[7].load( LIBPATH + "zobrist7") &&
		lib_zobrist[8].load( LIBPATH + "zobrist8") ){
		clog <<"All FSMs been loaded ."<< endl; 
	}
	else {
		VS vf = INOUT().getfilename( sgfpath, "*.sgf"); 
		clog <<"Learning from these file :"<< endl << vf; 
		learn (vf); 
	}
}

LEARN::~LEARN(){
	VS  vf = INOUT().getfilename(LIBPATH, "*"); 
	if (find(vf.begin (), vf.end (), LIBPATH + "xorand") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "pemis") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist0") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist1") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist2") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist3") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist4") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist5") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist6") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist7") == vf.end() || 
		find(vf.begin (), vf.end (), LIBPATH + "zobrist8") == vf.end() ) {
			g_init.save_xorand (LIBPATH + "xorand"); 
			lib_pemis.save( LIBPATH + "pemis"); 
			lib_zobrist[0 ].save( LIBPATH + "zobrist0"); 
			lib_zobrist[1 ].save( LIBPATH + "zobrist1"); 
			lib_zobrist[2 ].save( LIBPATH + "zobrist2"); 
			lib_zobrist[3 ].save( LIBPATH + "zobrist3"); 
			lib_zobrist[4 ].save( LIBPATH + "zobrist4"); 
			lib_zobrist[5 ].save( LIBPATH + "zobrist5"); 
			lib_zobrist[6 ].save( LIBPATH + "zobrist6"); 
			lib_zobrist[7 ].save( LIBPATH + "zobrist7"); 
			lib_zobrist[8 ].save( LIBPATH + "zobrist8"); 
			clog << "All FSM been saved ." << endl; 
			sleep(2000); 
			
		}
}

void LEARN::learn(VS vf){
	INFOGO* infop; 
	SGFROOT* p; 
	for (int v = 0; v<vf.size (); ++v){
		___COUT1(vf[v]);
		n_zobrist = 0;
		___.tic("zobrist");

		p = INOUT().sgf2tree(vf[v]); 
		// 16 �ֶԳƷ�ʽ���벻һ��
		for (int k = 0; k<2; ++k){
			for (int i = 0; i<8; ++i) {
				infop = new INFOGO; 
				g_init.swap_id8_q8(0, i); 
				p->fuseki(infop); 
				for (int j = 0; j<p->sons.size(); ++j){
					visit(p->sons[j], infop, &LEARN::insert_zobrist); 
				}
				g_init.swap_id8_q8 (0, i); 
				infop->delfromroot(); 
				//___COUT2(k, i); 
			}
			g_init.swap_xorand(); 
		}
		/*
		// 16 �ֶԳƷ�ʽ�±��붼һ��
		infop = new INFOGO; 
		p->fuseki(infop); 
		for (int i = 0; i<p->sons.size(); ++i){
			visit(p->sons[i], infop, &LEARN::insert_pemis); 
			infop->delfromroot(); 
		}
		*/
		delete p; 

		___.toc();
		___COUT1(n_zobrist);
	}
	for (int v = 0; v<vf.size (); ++v){
		___COUT1(vf[v]);
		n_pemis = 0;
		___.tic("pemis");

		p = INOUT().sgf2tree(vf[v]); 
		/*
		// 16 �ֶԳƷ�ʽ���벻һ��
		for (int k = 0; k<2; ++k){
			for (int i = 0; i<8; ++i) {
				infop = new INFOGO; 
				g_init.swap_id8_q8(0, i); 
				p->fuseki(infop); 
				for (int j = 0; j<p->sons.size(); ++j){
					visit(p->sons[j], infop, &LEARN::insert_zobrist); 
				}
				g_init.swap_id8_q8 (0, i); 
				infop->delfromroot(); 
				___COUT2(k, i); 
			}
			g_init.swap_xorand(); 
		}
		*/
		// 16 �ֶԳƷ�ʽ�±��붼һ��
		infop = new INFOGO; 
		p->fuseki(infop); 
		for (int i = 0; i<p->sons.size(); ++i){
			visit(p->sons[i], infop, &LEARN::insert_pemis); 
			infop->delfromroot(); 
		}
		delete p; 

		___.toc();
		___COUT1(n_pemis);
	}
	INOUT().save2txt(vf, LIBPATH + "filenames.txt");
}

void LEARN::visit(SGFNODE* p, INFOGO* infop, FPTR f ){
	infop = infop->move_tree (p->pos, p->color ); 
	infop->setzobrist();
	(this->*f)(infop); 
	for (int i = 0; i<p->sons .size (); ++i){
		visit(p->sons[i], infop, f); 
	}
}

void LEARN::insert_zobrist(INFOGO* infop) {
	for (int i = 0; i<ZOBRIST::ZOBRIST_KIND; ++i){
		KEY f = infop->father->getzobrist (i); 
		KEY s = infop->getzobrist (i); 
		if (f != s){
			KEY k = g_init.getrand (infop->getlastpos(), infop->getlastclr () ); 
			lib_zobrist[g_init.getq8(i)].insert (f, k);
			++ n_zobrist;//-//for thesis
		}
	}
}

void LEARN::insert_pemis(INFOGO* infop) {
	 vector<pair<ULL, ULL> > vpuu = pemis(infop); 
	 for (int i = 0; i<vpuu.size (); ++i)
		 if (vpuu[i].first!=0 || vpuu[i].second!=0){
			lib_pemis.insert (vpuu[i].first, vpuu[i].second ); 
			++ n_pemis;//-//for thesis
		 }
}

vector<pair<ULL, ULL> > LEARN::pemis(INFOGO* infop) {
	// �˴������ɸ�������
	vector<pair<ULL, ULL> > vpuu; 
	BITB tmp; 
	ULL f, s; 
	vector<BITB> vecb; 
	//-// Ԥ�ȼ���
	vecb.push_back (BITB(infop->getlastpos()).dilate (1)); 
	vecb.push_back (BITB(infop->getlastpos()).dilate (2)); 
	vecb.push_back (BITB(infop->getlastpos()).dilate (3)); 
	vecb.push_back (BITB(infop->getlastpos()).dilate (4)); 
	for (int i = 0; i<PATFREQ::PEMIS_KIND; ++i){
		tmp = infop->father->xx .blockon(vecb[i]) 
			| infop->father->oo .blockon(vecb[i]); 
		if (tmp.empty ()&& !g_init.getroute(R4)[infop->getlastpos()] ){  // 
			vpuu.push_back(make_pair(0, 0)); //������ֵ�� getfreq()�е���
			continue; 
		}
		f = infop->father->pemis64(tmp); 
		tmp = infop->xx .blockon(vecb[i]) 
			| infop->oo .blockon(vecb[i]); 
		s = infop->pemis64(tmp); 
		vpuu.push_back (make_pair(f, s)); 
	}
	___ASSERT(vecb.size () == PATFREQ::PEMIS_KIND); 
	return vpuu; 
}

// ɨ��ʽ���¡�ʵ��Ӧ��ʱ�Ƿ���Ծֲ�����ʽ���£�
// �����д�����ȷ�������ֻ����������Ӧ��Ҳ���Խ��ܡ�
void LEARN::setpatfreq(INFOGO* infop) {
	vector<pair<ULL, ULL> > vpuu; 
	ITR itr; 
	INFOGO* sonp; 
	for (POS pos = itr.posbegin(); !itr.posend(); pos = itr.posnext()){
		sonp = infop->move_tree(pos, BLACK); 
		if (sonp != infop){
			vpuu = pemis(sonp); 
			for (int i = 0; i< PATFREQ::PEMIS_KIND; ++i)
				infop->patfreq.setf(pos, BLACK, i, 
					lib_pemis.getfreq(vpuu[i].first, vpuu[i].second)); 
			delete sonp; 
			infop->sons .pop_back (); 
		}
		else 
			for (int i = 0; i< PATFREQ::PEMIS_KIND; ++i)
				infop->patfreq.setf (pos, BLACK, i, -1); 
        sonp = infop->move_tree(pos, WHITE); 
		if (sonp != infop){
			vpuu = pemis(sonp); 
			for (int i = 0; i< PATFREQ::PEMIS_KIND; ++i)
				infop->patfreq.setf(pos, WHITE, i, 
					lib_pemis.getfreq(vpuu[i].first, vpuu[i].second)); 
			delete sonp; 
			infop->sons .pop_back (); 
		}
		else 
			for (int i = 0; i< PATFREQ::PEMIS_KIND; ++i)
				infop->patfreq.setf (pos, WHITE, i, -1); 
	}
}

/* class SGFROOT and SGFNODE */

SGFNODE::~SGFNODE(){ 
	for (int i = 0; i<sons.size (); ++i)
		delete sons[i]; 
	//clog<< "delete the sgfnode !"<< endl; 
}

SGFROOT::~SGFROOT(){ 
	for (int i = 0; i<sons.size (); ++i)
		delete sons[i]; 
	//clog<< "delete the sgfroot !"<< endl; 
}

/* _______________________________ ������ _______________________________ */

void TEST_L::joseki(){
}

void TEST_L::lib_zobrist(){
	//VS vf = INOUT().getfilename( SGFPATH, "*.sgf"); 
	//cout << vf; 
	//LEARN learn; 
	//learn.learn (vf); 
	//g_learn.lib_pemis.save (LIBPATH + "pemis"); //
	/*
	FSM<ULL> loaded; //
	loaded.load (LIBPATH + "pemis"); //
	___COUT2(g_learn.lib_pemis, loaded); //
	___ASSERT(g_learn.lib_pemis == loaded); //
	*/

	for(; ; ) {
		string sw, sn; 
		INFOGO* infop = new INFOGO; 
		cout << *infop; 
		for(; ; ) {
			for (int i = 0; i<ZOBRIST::ZOBRIST_KIND; ++i){
				int a = g_learn.lib_zobrist[i].getsonssize(infop->getzobrist(i)); 
				cout<< "lib_zobrist["<<i<<"] branchs: "; 
				if (a == 0)
					cout << endl; 
				else if (a == 1)
					cout << "[0]" << endl; 
				else
					cout << "[0]-[" << a-1 << "]" << endl; 
			}
			cout << "> "; 
			//cin >> sw;
			sw = getline()[0];
			if (sw == "end" )
				break; 
			cout << "> "; 
			//cin >> sn;
			sn = getline()[0];
			if (sn == "end" )
				break; 
			int which = -1, n = -1; 
			if (isdigit(sw[0]) && isdigit(sn[0])){
				which = atoi(sw.c_str() ); 
				n = atoi(sn.c_str() ); 
			}
			if (which >= 0 && which<ZOBRIST::ZOBRIST_KIND  )
				if ( n >= 0 && n < g_learn.lib_zobrist[which].getsonssize(infop->getzobrist (which))) {
					MOVE m = g_learn.lib_zobrist[which].getmove(infop->getzobrist (which), n); 
					infop = infop->move_tree(m.first, m.second); 
					//___COUT2(m.first, m.second ); 
					cout << *infop; 
					/*
					g_learn.setpatfreq (infop); //
					___COUT1(infop->patfreq); //
					*/
				}
		}
		infop->delfromroot(); 
		if (sw == "end" || sn == "end")
			break; 
	}
}

void TEST_L::timefunc (string which) {
	RANDER  r; 
}



