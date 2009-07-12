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
	//-// ���ظ�����
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
		//-// û���жϲ�����Ч��
		int id = fathertag(fa); 
		return g_init.getmove (sons[id][i]); 
	}
	int getfreq(T fa, int i){
		return freq[fathertag(fa)][i];
	}
	int getfreq(T fa, T son){
		// ����ֵ make_pair(0, 0); 
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
	CHONG_TM, //��
	JIA2_TM, //��
	YA_TM, //ѹ
	BAN_TM, //��
	NIU_TM, //Ť
	DING_TM, //��
	CI_TM, //��
	WA_TM, //��
	DUAN_TM, //��
	ZHEN_TM, //��
	DA1_TM, //��
	KUA_TM, //��
	PENG_TM, //��
	KAO_TM, //��
	QU_TM, //��
	TUO_TM, //��
	LIAN_TM, //��
	HU_TM, //��
	TING_TM, //ͦ
	DANG_TM, //��	
	TIE_TM, //��
	GONG_TM, //��
	JIE_TM, //��
	ZHAN_TM, //ճ
	PU_TM, //��
	ZHENG_TM, //��
	JIA1_TM, //��
	JI_TM, //��
	GUN_TM, //��
	JU_TM, //��
	BIDING_TM, //�Ƕ�
	FANJIA_TM, //����
	FEIYA_TM, //��ѹ
	LIANBAN_TM, //����
	PA_TM, //��
	TUI_TM, //��
	CHANG_TM, //��
	SHUANG_TM, //˫
	LI_TM, //��
	JIAN_TM, //��
	BING_TM, //��
	GUAN_TM, //��
	GUAI_TM, //��
	JIANCHONG_TM, //���
	JIANDING_TM, //�ⶥ
	DIANFANG_TM, //�㷽
	YUXINGSANJIAO_TM, //��������
	SANZIZHENGZHONG_TM, //��������
	CHUANXIANGYAN_TM, //������

	XIAOFEI_TM, //С��
	DAFEI_TM, //���
	CHAODAFEI_TM, //�����
	XIANGFEI_TM, //�󲽷�
	GUA_TM, //��
	DI_TM, //��
	LIERCAISAN_TM, //��������
	ERJIANCHAI_TM, //�����
	ERJIANTIAO_TM, //������
	TIAO_TM, //��
};

enum SHAPELIBTERM{
	PANJIAOQUSI_TM, //�̽�����
	DAERHUANYI_TM, //�����һ
	JINJIDULI_TM, //�𼦶���
	DATOUGUI_TM, //��ͷ��
	DAOTUOXUE_TM, //����ѥ
	ZHANGSINIU_TM, //����ţ
}; 

enum LIBTERM{
	DA3_TM, //��
	FANDA_TM, //����
	SHUANGDA_TM, //˫��
	HUANZHENG_TM, //����
	//JIEBUGUI_TM, //�Ӳ���
	TI_TM, //��
	ZIJINQI_TM, //�Խ���
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

/* _______________________________ ������ _______________________________ */

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
