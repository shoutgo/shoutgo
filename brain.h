#ifndef _BRAIN
#define _BRAIN

#include "targetsearch.h"
#include "learn.h"

enum PRINCIPLE
{ DUIKANG,			//对抗原则
  HUANWEISIKAO,			//换位思考原则
  RENNAI,			//忍耐原则
  SHITAN,			//试探原则
  DUIKANGHOUXIAO,		//对抗得逞后效原则
  TUOXIAN,			//脱先原则
  SHUNSHI,			//顺势原则
  JISUO,			//急所原则
  LIYIZUIDA,			//利益最大原则
  LIYIPINGHENG,			//收益平衡原则
  LIYIYUZHI,			//阈值原则
  PRINCIPLE_SIZE
};

class PLAYER
{
  static const LIKELIHOOD PROB_MAX = 100;
  AIMGRAPH aimtree;
  LIKELIHOOD p[PRINCIPLE_SIZE];
public:
  PRINCIPLE now ();
};

class BRAIN: public INFOGO
{
private:
  INFOGO * infop;
  TARGETS *tgs;
  PLAYER xxer;
  PLAYER ooer;
  TARGETSEARCH tgsearch;

  //LEARN                         learn; 
  NN nn;
  TRANSPTABLE < BITB, BITB > transpt;
public:
  void event ();		//事件
  void cluster ();		//棋群聚类       
  void area ();			//地域划分
  VT top () const;
  VT surmise ();		//走步意图猜测
  TARGETS inspire ();		//目标生成
  TARGETS prepare (const TARGET &);	//为某目标作铺垫
  TARGETS follow (const TARGET &);	//某目标之后续
  TARGETS compose (const VT &);	//目标合成，如缠绕
  TARGETS *plan ();		//计划
  TARGETS decide ();		//决定
  POS genmove ();		//走步生成
  TARGETS sabaki ();		//腾挪              
  TARGETS suteru ();		//弃子
  TARGETS tenuki ();		//脱先
  int yoseki ();		//官子计算
  int judgement ();		//形势判断
  void model ();		//棋手建模
  void autolearn ();		//自学习        
  void sgflearn ();		//棋谱学习
  void teach ();		//人类示教
  bool endgame () const;

  //bool          move( const POS&, COLOR ); 
  string interpret ();		//走步解释
  friend ostream & operator<< (ostream & os, const BRAIN &);
};


#endif
