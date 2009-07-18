#include "learn.h"
#include "inout.h"
#include <unistd.h>	// sleep()	// VS2008

/* class LEARN */

LEARN::LEARN(string sgfpath){
	// 因为缓式计算的关系，要保证都 load(), 应该用 &&
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
		// 16 种对称方式编码不一样
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
		// 16 种对称方式下编码都一样
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
		// 16 种对称方式编码不一样
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
		// 16 种对称方式下编码都一样
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
	// 此处可生成各种掩码
	vector<pair<ULL, ULL> > vpuu; 
	BITB tmp; 
	ULL f, s; 
	vector<BITB> vecb; 
	//-// 预先计算
	vecb.push_back (BITB(infop->getlastpos()).dilate (1)); 
	vecb.push_back (BITB(infop->getlastpos()).dilate (2)); 
	vecb.push_back (BITB(infop->getlastpos()).dilate (3)); 
	vecb.push_back (BITB(infop->getlastpos()).dilate (4)); 
	for (int i = 0; i<PATFREQ::PEMIS_KIND; ++i){
		tmp = infop->father->xx .blockon(vecb[i]) 
			| infop->father->oo .blockon(vecb[i]); 
		if (tmp.empty ()&& !g_init.getroute(R4)[infop->getlastpos()] ){  // 
			vpuu.push_back(make_pair(0, 0)); //此特殊值在 getfreq()中调用
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

// 扫描式更新。实际应用时是否可以局部增量式更新？
// 尽管有处不正确，但如果只是用于排序，应该也可以接受。
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

