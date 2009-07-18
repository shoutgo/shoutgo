#include "brain.h"
#include "inout.h"

PRINCIPLE PLAYER::now(){
	for (int i = 0; i< PRINCIPLE_SIZE; ++i) 
		if (prob(p[i], PROB_MAX))
			return static_cast<PRINCIPLE>(i); 
	return PRINCIPLE_SIZE; 
}

bool	BRAIN::endgame() const {
	return 0; 
}

POS		BRAIN::genmove() {
	tgsearch.numb = 0;
	___.tic("search by minmax ...");
    RESULT rslt = tgsearch.minmax(this, TARGETS(TARGET(ROOT, 2)));
	___.toc();
	___COUT1(tgsearch.numb);
	___ASSERT2(rslt.move.second == turn(), 
		___COUT3(rslt.move.first, rslt.move.second, rslt.value););
	___COUT1(rslt.move.first);
	return rslt.move.first;
}

/*
bool	BRAIN::move( const POS& pos, COLOR clr){
	INFOGO* sonp = infop->move_tree(pos, clr); 
	if (sonp == infop)
		return 0; 
	else {
		infop = sonp; 
		return 1; 
	}
}
*/

string	BRAIN::interpret(){
	return " "; 
}

VT BRAIN::top( ) const{
	VT vt; 
	PBC smass = infop->area .getmass (infop->getlastpos(), infop->getlastclr()); 
	PBC smass_his = infop->area.getmass ( smass.first, infop->turn()); 
	PBC fmass = infop->father->area .getmass (smass.first, infop->getlastclr()); 
	PBC fmass_his = infop->father->area .getmass (smass.first, infop->turn()); 
	// 断事件，必然有异块半联（即断）已块半联（即断），可能有已块联络（全联）
	BITB cut = infop->cutpos(infop->getlastclr()).blockon(infop->getlastpos()); 
	if (cut[infop->getlastpos()]) {
		PBC f_his = infop->father->area .getmass (cut, infop->turn()); 
		PBC s_my = infop->area.getmass (cut, infop->getlastclr()); 
		vt.push_back (TARGET(DUAN, INT_MAX, f_his, s_my)); //被断者，断者
	}
	// 已块联络，落在父的什么色区都有可能
	if ( fmass.first .blockcount() > 1)
		vt.push_back (TARGET(LIANLUO, INT_MAX, fmass, smass)); //联络对象，联络效果
	// 异块隔开，落在父的什么色区都有可能
	if ( fmass_his.first .blockcount() < smass_his.first .blockcount() )
		vt.push_back (TARGET(GE, INT_MAX, fmass_his, smass)); //隔开对象，阻隔者
	return vt; 
}

VT	BRAIN::surmise () {
	VT vt; 
	//局部分析：
	//	block 层次(对方圆三步之内的每个块)

	/*
	BITB view = BITB(infop->getlastpos()).dilate (3); 
	ITR itr = ITR(infop->xx.blockon(view)); 
	for (BITB tmp = itr.blockbegin(); !itr.blockend(); tmp = itr.blocknext())
        if (infop->getlastclr () == BLACK)
			ts |= TARGET(tmp, BLACK, JIUZI); 
		else
			ts |= TARGET(tmp, BLACK, CHIZI); 
	itr = ITR(infop->oo.blockon(view)); 
	for (BITB tmp = itr.blockbegin(); !itr.blockend(); tmp = itr.blocknext())
        if (infop->getlastclr () == BLACK)
			ts |= TARGET(tmp, WHITE, CHIZI); 
		else
			ts |= TARGET(tmp, WHITE, JIUZI); 
	*/

	//	mass 层次(父)(与cluster层次匹配运用)
	//
	//	cluster 层次(父)(与mass层次匹配运用)
	//			异群：	地
	//							新生团		POYAN
	//								外生团		SUOYANWEI
	//								内生团
	//					海
	//							新生团		SHA
	//								外生团		GAN, SHENGTUI
	//								内生团
	//					空
	//							新生团		
	//								外生团
	//								内生团
	//			已群：	地
	//							新生团		
	//								外生团		KUOYANWEI
	//								内生团		ZUOYAN
	//					海
	//							新生团		
	//								外生团		SHENGEN, _SHENGTUI
	//								内生团		ZUOYAN
	//					空
	//							新生团
	//								外生团
	//								内生团
	//			公群：
	//							新生团		SHA
	//								外生团		ZUOHUO
	//								内生团		ZUOHUO
	//		
	//			公空：					
	//	potential 层次：

		
	//全局拓扑分析：
	//	block 层次
	//			新块
	//			生长块
	//			内长块
	//			已块融合
	//			异块分裂
	//	mass 层次
	//			新团
	//			生长团
	//			内长团
	//			已团融合
	//			异团分裂
	//	cluster 层次
	//			对落点与所有群按距离排序(至少与所属者相邻)，
	//			生成联络与攻击可能目标
	return vt; 
}

