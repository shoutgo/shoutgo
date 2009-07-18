#ifndef _TARGET
#define _TARGET

#include "infoboard.h"
#include "search.h"

/* class AIMGRAPH */

enum AIM {
	ROOT, 
		HIS_CLUSTER, 
			GE, //
			DUAN, 
			FENG, 
			GAN, 
			SHENGTUI, //
			SHA, 
				POYAN, 
				SUOYANWEI, 
		HIS_MASS, 
			_ZHENGXING, 
			CHIZI, 
				DA, 
				JIA, 
				GUN, 
				JIEBUGUI, 
				DAOPU, 
				ZHENGZI, 
				DAOTUOXUE, 
		HIS_POTENTIAL, 
			_ZHIGAODIAN, //		//双方近层高位生长子
			QINGXIAO, //一方近层高位生长子
			BI, //一方中距低位生长子
			QINSHI, //一方近层子
			DARU, //势内部子
		MY_CLUSTER, 
			_GE, //
			LIANLUO, 
			TUWEI, 
			_SHENGTUI, //
			SHENGGEN, 
			ZUOHUO, 
				ZUOYAN, 
				KUOYANWEI, 
		MY_MASS, 
			ZHENGXING, 
			JIUZI, 
				_DA, 
				_JIA, 
				_GUN, 
				_JIEBUGUI, 
				_DAOPU, 
				_ZHENGZI, 
				_DAOTUOXUE, 
		MY_POTENTIAL, 
			ZHIGAODIAN, //
			KUOZHANG, 
			CHAI, 
			_QINSHI, 
			_DARU, 
		PUBLICAREA, 
			ZHANDACHANG, 
			FENTOU, //
		COMPLICATE, 
			TENNUO, 
			JIAO, 
			YUWEI, 
			SHIYINGSHOU, 
			ZUOJIE, 
			XUNJIE, 
			CHANGSHENG, 
			DUISHA, 
		SIMPLIFY, 
			DINGXING, 
			LIYONG, 
			ZHENGXIAN, 
			DAJIANG, 
	AIM_SIZE
}; 

class AIMGRAPH {
public:
	static const LIKELIHOOD P_MAX = 100; 
	LIKELIHOOD	p[AIM_SIZE]; 
	AIM			father[AIM_SIZE]; 
	vector<AIM> sons[AIM_SIZE]; 
public:
	AIMGRAPH(); 
	void	setup(vector<AIM> ); 
	AIM		forward(AIM); 
	AIM		backward(AIM); 
	AIM		anti(AIM); 
}; 

/* class TARGET */

const PBC	NULL_PBC = make_pair(NULL_BB, NULL_CLR); 
	
struct TARGET {
	AIM		aim; 
	int		maxdepth;
	PBC		obj; 
	PBC		env; 
	int		probvalue; 
	bool	sentewish; 
	int		passtimes; 
public:
	TARGET(AIM a, int ma = INT_MAX,
		const PBC& ob = NULL_PBC, const PBC& en = NULL_PBC, 
		int pr = 0, bool se = 0, int pa = 0); 
	TARGET	operator ~ (); 
	bool	operator <  (const TARGET& t) const; 
	bool	operator == (const TARGET& t) const; 
	bool	operator != (const TARGET& t) const; 
}; 

typedef vector<TARGET>	VT; 
typedef vector<VT >		VVT; 
typedef set<TARGET>		ST; 
typedef set<VT >		SVT; 

class TARGETS {
public:
	VVT nut; 
public:
	TARGETS(); 
	TARGETS(const VVT& a); 
	TARGETS(const TARGET& t); 
}; 

TARGETS		operator ~ (TARGETS a); 

bool		operator == (const VT& vta, const VT& vtb); 
bool		operator != (const VT& vta, const VT& vtb); 

bool		operator == (const TARGETS& a, const TARGETS& b); 
bool		operator != (const TARGETS& a, const TARGETS& b); 

TARGETS		operator | (const TARGET& a, const TARGET& b); 
TARGETS		operator & (const TARGET& a, const TARGET& b); 

TARGETS		operator | (const TARGETS& a, const TARGET& b); 
TARGETS		operator & (const TARGETS& a, const TARGET& b); 

TARGETS		operator | (const TARGETS& a, const TARGETS& b); 
TARGETS		operator & (const TARGETS& a, const TARGETS& b); 

TARGETS		operator |= (const TARGETS& a, const TARGET& b); 
TARGETS		operator &= (const TARGETS& a, const TARGET& b); 

/* class RESULT */

struct RESULT{
	int			 value;
	MOVE		 move;
	unsigned     a; //合目的
	unsigned     b; //官子
	unsigned     c; //棋形
	unsigned     d; //劫材
	unsigned     e; //余味
	unsigned     f; //先手
	unsigned     g; //兼顾
	unsigned     h; //复杂度
	unsigned     i; //气
	unsigned     j; //轻重
	unsigned     k; //隐蔽性
public:
	RESULT(int v = 0):value(v){}
	bool	operator  < (const RESULT& r) const; 
	bool	operator == (const RESULT& r) const; 
	RESULT	operator += (const RESULT& r) const; 
}; 

/* class TARGETSEARCH */

class TARGETSEARCH  :public SEARCH <RESULT, INFOGO, MOVE, TARGETS>{	
public:
	TRANSPTABLE<BITB, int>	transpt[AIM_SIZE]; 

public:
    TARGETSEARCH(){}; 

	BITB		scope(INFOGO * infop, TARGET tg); 
	bool		isleaf(INFOGO * infop, TARGET tg); 
	RESULT		leafvalue(INFOGO * infop, TARGET tg); 

	VM			findpath(INFOGO * infop, TARGETS tgs); 
	bool		isleaf(INFOGO * infop, TARGETS tgs); 
	RESULT		leafvalue(INFOGO * infop, TARGETS tgs); 
	INFOGO*		findson(INFOGO * infop, MOVE mv); 
	RESULT		upbound(); 
	RESULT		downbound(); 
	bool		ismax(INFOGO * infop, TARGETS tgs); 
	TARGETS     control(INFOGO * infop, TARGETS tgs);
	RESULT      recordpath(RESULT result, MOVE move);

}; 

/* _______________________________ 测试区 _______________________________ */

enum	TS_FUN	{TS_TARGETSEARCH = FTS*FUN_MAX, TS_FUN_SIZE}; 

class TEST_TS {
public:
	void assertall(); 
	void timefunc(string which = ""); 
	void active(){
	}  
}; 

#endif
