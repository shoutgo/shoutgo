#ifndef _BRAIN
#define _BRAIN

#include "targetsearch.h"
#include "learn.h"

const enum PRINCIPLE {
	DUIKANG, //�Կ�ԭ��
	HUANWEISIKAO, //��λ˼��ԭ��
	RENNAI, //����ԭ��
	SHITAN, //��̽ԭ��
	DUIKANGHOUXIAO, //�Կ��óѺ�Чԭ��
	TUOXIAN, //����ԭ��
	SHUNSHI, //˳��ԭ��
	JISUO, //����ԭ��
	LIYIZUIDA, //�������ԭ��
	LIYIPINGHENG, //����ƽ��ԭ��
	LIYIYUZHI, //��ֵԭ��
	PRINCIPLE_SIZE
}; 

class PLAYER {
	static const LIKELIHOOD PROB_MAX = 100; 
	AIMGRAPH		  aimtree; 
	LIKELIHOOD	  p[PRINCIPLE_SIZE]; 
public :
	PRINCIPLE	  now(); 
}; 

class BRAIN
	:public INFOGO
//	:public SEARCH<RESULT, INFOGO, MOVE, TARGETS>
{		
private:
	INFOGO*				infop; 
	TARGETS*			tgs; 

	PLAYER				xxer; 
	PLAYER				ooer; 

	TARGETSEARCH		tgsearch; 
	//LEARN				learn; 

	NN					nn; 
	TRANSPTABLE<BITB, BITB>	transpt; 

public:

	void		event(); //�¼�
	void		cluster(); //��Ⱥ����	
	void		area(); //���򻮷�

	VT			top( ) const; 
	VT			surmise( ); //�߲���ͼ�²�

	TARGETS		inspire( ); //Ŀ������
	TARGETS		prepare(const TARGET&); //ΪĳĿ�����̵�
	TARGETS		follow(const TARGET&); //ĳĿ��֮����
	TARGETS		compose(const VT&); //Ŀ��ϳɣ������

	TARGETS*	plan(); //�ƻ�
	TARGETS		decide(); //����
	POS			genmove(); //�߲�����

	TARGETS		sabaki(); //��Ų		
	TARGETS		suteru(); //����
	TARGETS		tenuki(); //����
	int			yoseki(); //���Ӽ���
	int			judgement(); //�����ж�

	void		model(); //���ֽ�ģ
		
	void		autolearn(); //��ѧϰ	
	void		sgflearn(); //����ѧϰ
	void		teach(); //����ʾ��

	bool		endgame() const; 
	//bool		move( const POS&, COLOR ); 
	string		interpret(); //�߲�����

	friend ostream& operator<<(ostream& os, const BRAIN& ); 
}; 

/* _______________________________ ������ _______________________________ */

const enum	BR_FUN	{
	BR_BRAIN = FBR*FUN_MAX, BR_FUN_SIZE}; 

class TEST_BR {
public:
	void assertall(); 
	void timefunc(string which = ""); 
	void active(){ 
	} 
}; 

#endif