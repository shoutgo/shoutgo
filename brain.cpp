#include "stdafx.h"
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
	// ���¼�����Ȼ�������������ϣ��ѿ���������ϣ����������ѿ����磨ȫ����
	BITB cut = infop->cutpos(infop->getlastclr()).blockon(infop->getlastpos()); 
	if (cut[infop->getlastpos()]) {
		PBC f_his = infop->father->area .getmass (cut, infop->turn()); 
		PBC s_my = infop->area.getmass (cut, infop->getlastclr()); 
		vt.push_back (TARGET(DUAN, INT_MAX, f_his, s_my)); //�����ߣ�����
	}
	// �ѿ����磬���ڸ���ʲôɫ�����п���
	if ( fmass.first .blockcount() > 1)
		vt.push_back (TARGET(LIANLUO, INT_MAX, fmass, smass)); //�����������Ч��
	// �����������ڸ���ʲôɫ�����п���
	if ( fmass_his.first .blockcount() < smass_his.first .blockcount() )
		vt.push_back (TARGET(GE, INT_MAX, fmass_his, smass)); //�������������
	return vt; 
}

VT	BRAIN::surmise () {
	VT vt; 
	//�ֲ�������
	//	block ���(�Է�Բ����֮�ڵ�ÿ����)

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

	//	mass ���(��)(��cluster���ƥ������)
	//
	//	cluster ���(��)(��mass���ƥ������)
	//			��Ⱥ��	��
	//							������		POYAN
	//								������		SUOYANWEI
	//								������
	//					��
	//							������		SHA
	//								������		GAN, SHENGTUI
	//								������
	//					��
	//							������		
	//								������
	//								������
	//			��Ⱥ��	��
	//							������		
	//								������		KUOYANWEI
	//								������		ZUOYAN
	//					��
	//							������		
	//								������		SHENGEN, _SHENGTUI
	//								������		ZUOYAN
	//					��
	//							������
	//								������
	//								������
	//			��Ⱥ��
	//							������		SHA
	//								������		ZUOHUO
	//								������		ZUOHUO
	//		
	//			���գ�					
	//	potential ��Σ�

		
	//ȫ�����˷�����
	//	block ���
	//			�¿�
	//			������
	//			�ڳ���
	//			�ѿ��ں�
	//			������
	//	mass ���
	//			����
	//			������
	//			�ڳ���
	//			�����ں�
	//			���ŷ���
	//	cluster ���
	//			�����������Ⱥ����������(����������������)��
	//			���������빥������Ŀ��
	return vt; 
}

/* _______________________________ ������ _______________________________ */

void TEST_BR::timefunc (string which) {
	RANDER  r; 
}
