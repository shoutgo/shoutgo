#ifndef _TRANSPTABLE
#define _TRANSPTABLE

#include "define.h"
#include "bitboard.h"

/* class FSM */

// _________________________________
// father:#1	sons: #1 #2 #3
//				freq: #1 #2 #3
// father:#2	sons: #1 #2 
// 				freq: #1 #2 
// father:
// ...
// __________________________________

template <class T>
class FSM {
	vector<T>			father; 
	vector<vector<T> >	sons; 
	VVI					freq; 
private:
	//-// 有重复计算
	bool fatherfind(T fa){
		return find(father.begin (), father.end (), fa) != father.end (); 
	}

	int fathertag(T fa){
		return find (father.begin (), father.end(), fa)-father.begin (); 
	}

	bool sonfind(int id, T son){
		return find(sons[id].begin (), sons[id].end(), son) != sons[id].end(); 
	}

	int sontag(int id, T son){
		return find(sons[id].begin (), sons[id].end(), son)-sons[id].begin(); 
	}
public:
	void insert(T fa, T son){
		int id = fathertag(fa); 
		if (fatherfind(fa))
			if (sonfind(id, son)) 
				++ freq [id][sontag(id, son)]; 
			else {
				sons[id].push_back (son); 
				freq[id].push_back (1); 
			}
		else {
			father.push_back(fa); 
			sons.push_back (vector<T>(1, son)); 
			freq.push_back (VI(1, 1)); 
		}
		___ASSERT(father.size () == sons.size ()
			&& sons.size () == freq.size ())
	}
	MOVE  getmove(T fa, int i){
		//-// 没有判断参数有效性
		int id = fathertag(fa); 
		return g_init.getmove (sons[id][i]); 
	}
	int getfreq(T fa, int i){
		return freq[fathertag(fa)][i];
	}
	int getfreq(T fa, T son){
		// 特殊值 make_pair(0, 0); 
		if (fa == 0 && son == 0)
			return 0; 
		int id = fathertag(fa); 
		if (fatherfind(fa) && sonfind(id, son))
			return freq[id][sontag(id, son)]; 
		else 
			return 0; 
	}
	int getsonssize(T fa) {
        if (fatherfind(fa))
			return sons[fathertag(fa)].size(); 
		else
			return 0; 
	}
	void sort_(){
	}
	bool save(string filename){
		ofstream	file(filename.c_str (), ios::binary); 
		if (!file) {
			cerr<< filename << " : open fail !"<< endl; 
			return 0; 
		}
		//INOUT().save(father, file); 
		//INOUT().save(sons, file); 
		//INOUT().save(freq, file); 
		file.close (); 
		clog<< "A FSM been saved in "<< filename << " ."<< endl; 
		return 1; 
	}
	bool load(string filename){
		ifstream file(filename.c_str (), ios::binary); 
		if (!file) {
			cerr<< filename << " : open fail !"<< endl; 
			return 0; 
		}
		//INOUT().load(father, file); 
		//INOUT().load(sons, file); 
		//INOUT().load(freq, file); 
		file.close (); 
		clog<< "A FSM been loaded from "<<filename <<" ."<< endl; 
		return 1; 
	}
	bool operator == (const FSM<T> f2){
		return (father == f2.father 
			&& sons == f2.sons
			&& freq == f2.freq); 
	}
    friend ostream& operator<<(ostream& os, const FSM<T>& fsm){
		os<<fsm.father<< endl<< endl; 
		os<<fsm.sons<< endl<< endl; 
		os<<fsm.freq<< endl<< endl; 
		return os; 
	}
}; 

/* class HASH */

template<class T = BITB>
class HASH {
public:
	size_t operator()(const BITB& bb) const	{
        return (size_t)rand(); 
	}

	bool operator()(const BITB& bb1, const BITB& bb2) const	{	
		return bb1 < bb2; 
	}
}; 

/* class TRANSPTABLE */

template <class K, class V, class T_HASH = bool>
class TRANSPTABLE {
public:
	//hash_map<K, V, T_HASH> lib; 
	map<K, V> lib; 

	bool recorded(const K& k) const {
		return lib.count(k); 
	}
	V	 get(const K& k) {
		return lib[k]; 
	}
	void record(const K& k, const V& v) {
		typedef typename map<K, V>::value_type MVT; 
		lib.insert (lib.lower_bound (k), MVT(k, v)); 
	}
}; 

/* class TERM */

enum SHAPETERM {
	CHONG_TM, //冲
	JIA2_TM, //夹
	YA_TM, //压
	BAN_TM, //扳
	NIU_TM, //扭
	DING_TM, //顶
	CI_TM, //刺
	WA_TM, //挖
	DUAN_TM, //断
	ZHEN_TM, //镇
	DA1_TM, //搭
	KUA_TM, //跨
	PENG_TM, //碰
	KAO_TM, //靠
	QU_TM, //曲
	TUO_TM, //托
	LIAN_TM, //连
	HU_TM, //虎
	TING_TM, //挺
	DANG_TM, //挡	
	TIE_TM, //贴
	GONG_TM, //拱
	JIE_TM, //接
	ZHAN_TM, //粘
	PU_TM, //扑
	ZHENG_TM, //征
	JIA1_TM, //枷
	JI_TM, //挤
	GUN_TM, //滚
	JU_TM, //聚
	BIDING_TM, //鼻顶
	FANJIA_TM, //反夹
	FEIYA_TM, //飞压
	LIANBAN_TM, //连扳
	PA_TM, //爬
	TUI_TM, //退
	CHANG_TM, //长
	SHUANG_TM, //双
	LI_TM, //立
	JIAN_TM, //尖
	BING_TM, //并
	GUAN_TM, //关
	GUAI_TM, //拐
	JIANCHONG_TM, //肩冲
	JIANDING_TM, //尖顶
	DIANFANG_TM, //点方
	YUXINGSANJIAO_TM, //愚形三角
	SANZIZHENGZHONG_TM, //三子正中
	CHUANXIANGYAN_TM, //穿象眼

	XIAOFEI_TM, //小飞
	DAFEI_TM, //大飞
	CHAODAFEI_TM, //超大飞
	XIANGFEI_TM, //象步飞
	GUA_TM, //挂
	DI_TM, //缔
	LIERCAISAN_TM, //立二拆三
	ERJIANCHAI_TM, //二间拆
	ERJIANTIAO_TM, //二间跳
	TIAO_TM, //跳
};

enum SHAPELIBTERM{
	PANJIAOQUSI_TM, //盘角曲四
	DAERHUANYI_TM, //打二还一
	JINJIDULI_TM, //金鸡独立
	DATOUGUI_TM, //大头鬼
	DAOTUOXUE_TM, //倒脱靴
	ZHANGSINIU_TM, //胀死牛
}; 

enum LIBTERM{
	DA3_TM, //打
	FANDA_TM, //反打
	SHUANGDA_TM, //双打
	HUANZHENG_TM, //缓征
	//JIEBUGUI_TM, //接不归
	TI_TM, //提
	ZIJINQI_TM, //自紧气
}; 

class TERM {
	FSM<ULL>	shape; 
	FSM<ULL>	liberty; 
public:
	TERM(); 
	template <class T>
		void insert(string patstr, vector<T>){
		}
}; 

/* _______________________________ 测试区 _______________________________ */

enum	TT_FUN	{
	TT_TRANSPTABLE = FTT* FUN_MAX, TT_FUN_SIZE}; 

class TEST_TT {
public:
	void assertall(); 
	void timefunc(string which = ""); 
	void aimtree(); 
	void active(){
		aimtree(); 
	} 
}; 

#endif
