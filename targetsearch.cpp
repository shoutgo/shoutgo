#include "targetsearch.h"  
#include "inout.h"
#include "go.h"
/* class AIMGRAPH */

AIMGRAPH::AIMGRAPH(){
	for (int i = 0; i<AIM_SIZE; ++i)
		p[i] = P_MAX/2; 
	father[ROOT] = ROOT; 
	setup(BIND<AIM, int>()( 10, ROOT, 
									HIS_CLUSTER, HIS_MASS, HIS_POTENTIAL, PUBLICAREA, 
									MY_CLUSTER, MY_MASS, MY_POTENTIAL, 
									COMPLICATE, SIMPLIFY		)); 
	setup(BIND<AIM, int>()( 5, HIS_CLUSTER, 
									GE, 
									FENG, 
									GAN, 
									SHA			)); 
	setup(BIND<AIM, int>()( 3, SHA, 
										POYAN, 
										SUOYANWEI		)); 
	setup(BIND<AIM, int>()( 3, HIS_MASS, 
									_ZHENGXING, 
									CHIZI			)); 
	setup(BIND<AIM, int>()( 8, CHIZI, 
										DA, 
										JIA, 
										GUN, 
										JIEBUGUI, 
										DAOPU, 
										ZHENGZI, 
										DAOTUOXUE	)); 
	setup(BIND<AIM, int>()( 6, HIS_POTENTIAL, 
									QINGXIAO, 
									BI, 
									SHENGTUI, 
									QINSHI, 
									DARU		)); 
	setup(BIND<AIM, int>()( 2, PUBLICAREA, 
									ZHANDACHANG	)); 
	setup(BIND<AIM, int>()( 5, MY_CLUSTER, 
									_GE, 
									TUWEI, 
									SHENGGEN, 
									ZUOHUO		)); 
	setup(BIND<AIM, int>()( 3, ZUOHUO, 
										ZUOYAN, 
										KUOYANWEI		)); 
	setup(BIND<AIM, int>()( 3, MY_MASS, 
									ZHENGXING, 
									JIUZI		)); 
	setup(BIND<AIM, int>()( 8, JIUZI, 
										_DA, 
										_JIA, 
										_GUN, 
										_JIEBUGUI, 
										_DAOPU, 
										_ZHENGZI, 
										_DAOTUOXUE	)); 
	setup(BIND<AIM, int>()( 6, MY_POTENTIAL, 
									KUOZHANG, 
									CHAI, 
									_SHENGTUI, 
									_QINSHI, 
									_DARU		)); 
	setup(BIND<AIM, int>()( 1, 
									ZHIGAODIAN	)); 
	setup(BIND<AIM, int>()( 9, COMPLICATE, 
									TENNUO, 
									JIAO, 
									YUWEI, 
									SHIYINGSHOU, 
									ZUOJIE, 
									XUNJIE, 
									CHANGSHENG, 
									DUISHA		)); 
	setup(BIND<AIM, int>()( 5, SIMPLIFY, 
									DINGXING, 
									LIYONG, 
									ZHENGXIAN, 
									DAJIANG		)); 

}

AIM AIMGRAPH::forward(AIM a){
	if (sons[a].empty ())
		return a; 
	else
		for (int i = 0; i<sons[a].size (); ++i)
			if (prob(p[a], 100))
				return sons[a][i]; 
	return forward(a); 
}

AIM AIMGRAPH::backward(AIM a){
	return father[a]; 
}

AIM AIMGRAPH::anti(AIM a){
	if (a == ROOT || a >= COMPLICATE)
		return a; 
	else if (a >= MY_CLUSTER)
		return static_cast<AIM>(a-MY_CLUSTER+1); 
	else 
		return static_cast<AIM>(a+MY_CLUSTER-1); 
}

void AIMGRAPH::setup(vector<AIM> va){
	//___ASSERT2(va.size() >= 2,___COUT1(va.size());); 
	//___COUT1(va.size ()); 
	for (int i = 1; i<va.size (); ++i){
		sons[va[0]].push_back (va[i]); 
		father[va[i]] = va[0]; 
	}
}

/* class TARGET */

//ע��unique()ֻ�������ڵ���ͬԪ�أ�
//����������������ɾ���������ͬ�ߣ�
//����ÿ��ֻ����һ����Ԫ�ء������ơ�
TARGET::TARGET(AIM a, int ma, const PBC& ob, const PBC& en, 
			   int pr, bool se, int pa )
			   :aim(a), maxdepth(ma), obj(ob), env(en), 
			   probvalue(pr), sentewish(se), passtimes(pa){ 
}

TARGET	TARGET::operator~ ()	{
	TARGET tmp = (*this); 
	tmp.aim = AIMGRAPH().anti(tmp.aim); 
	return tmp; 
}

bool	TARGET::operator< (const TARGET& t) const { 
	return probvalue<t.probvalue; 
}				

bool	TARGET::operator == (const TARGET& t) const { 
	return (aim == t.aim && 
		obj == t.obj && 
		env == t.env && 
		probvalue == t.probvalue && 
		sentewish == t.sentewish && 
		passtimes == t.passtimes); 
}

bool	TARGET::operator != (const TARGET& t) const {
	return (aim != t.aim && 
		obj != t.obj && 
		env != t.env && 
		probvalue != t.probvalue && 
		sentewish != t.sentewish && 
		passtimes != t.passtimes); 
}

/* class TARGETS */

TARGETS::TARGETS(){
}

TARGETS::TARGETS(const VVT& a):nut(a) {
}

TARGETS::TARGETS(const TARGET& t) {
	VT vt = VT(1, t); 
	nut.push_back(vt); 
}

/* global operator */

bool		operator == (const VT& vta, const VT& vtb) {
	ST sta(vta.begin (), vta.end ()); 
	ST stb(vtb.begin (), vtb.end ()); 
	return (sta == stb); 
}

bool		operator != (const VT& vta, const VT& vtb) {
	ST sta(vta.begin (), vta.end ()); 
	ST stb(vtb.begin (), vtb.end ()); 
	return (sta != stb); 
}

TARGETS		operator | (const TARGET& a, const TARGET& b){
	VT vta(1, a); 
	VVT vvt(1, vta); 
	if (a != b){
		VT vtb(1, b); 
		vvt.push_back (vtb); 
	}
	return TARGETS(vvt); 
}

TARGETS		operator & (const TARGET& a, const TARGET& b){
	VT vta(1, a); 
	VVT vvt(1, vta); 
	if (a != b){
		vvt[0].push_back (b); 
	}
	return TARGETS(vvt); 
}
////
TARGETS		operator ~ (TARGETS a)	{
	vector<TARGETS>	 vts(a.nut.size ()); 
	VT vt; 
	for (int i = 0; i<a.nut.size (); ++i)						//�� a.nut ���У���ṹ��ÿ��Ԫ��ȡ~��
		for (int j = 0; j<a.nut[i].size (); ++j){				//���ţ���ṹ���γ�һ��targets, ��a.nut.size()��
			vt.clear (); 
			vt.push_back (~a.nut[i][j]); 
			vts[i].nut.push_back (vt); 
		}
	TARGETS tmp = vts[0]; 
	for (int i = 1; i<vts.size (); ++i)							//����Щtargets��&
        tmp = tmp & vts[i]; 
	for (int i = 0; i<tmp.nut.size (); ++i)						//����ÿ�У���ṹ��
		unique(tmp.nut[i].begin (), tmp.nut[i].end ()); 
	unique(tmp.nut.begin (), tmp.nut.end ()); //���������У���ṹ��
	return tmp; 
}

bool		operator == (const TARGETS& a, const TARGETS& b) {
	SVT svta(a.nut.begin (), a.nut.end ()); 
	SVT svtb(b.nut.begin (), b.nut.end ()); 
	return (svta == svtb); 
}

bool		operator != (const TARGETS& a, const TARGETS& b) {
	SVT svta(a.nut.begin (), a.nut.end ()); 
	SVT svtb(b.nut.begin (), b.nut.end ()); 
	return (svta != svtb); 
}

TARGETS		operator | (const TARGETS& a, const TARGETS& b)	{
	TARGETS tmp = a; 
	copy(b.nut.begin (), b.nut.end (), tmp.nut.end ()); 
	unique(tmp.nut.begin (), tmp.nut.end ()); 
	return tmp; 
}

TARGETS		operator & (const TARGETS& a, const TARGETS& b)	{
	TARGETS tmp; 
	for (int i = 0; i<b.nut.size (); ++i){
		copy(a.nut.begin (), a.nut.end (), tmp.nut.end ()); 
		for (int j = 0; j<a.nut.size (); ++j){
			int m = a.nut.size()*i + j; 
			copy(b.nut[j].begin (), b.nut[j].end (), tmp.nut[m].end ()); 
			unique(tmp.nut[m].begin (), tmp.nut[m].end ()); 
		}
	}
	unique(tmp.nut.begin (), tmp.nut.end ()); 
	return tmp; 
}
//// 
TARGETS		operator | (const TARGETS& a, const TARGET& b){
	TARGETS tmp = a; 
	VT vt(1, b); 
	tmp.nut.push_back (vt); 
	unique(tmp.nut.begin (), tmp.nut.end ()); 
	return tmp; 
}
	
TARGETS		operator & (const TARGETS& a, const TARGET& b){
	TARGETS tmp = a; 
	for (int i = 0; i<a.nut.size (); ++i){
		tmp.nut[i].push_back (b); 
		unique(tmp.nut[i].begin (), tmp.nut[i].end ()); 
	}
	return tmp; 
}

TARGETS		operator |= (const TARGETS& a, const TARGET& b){
	return a|b; 
}

TARGETS		operator &= (const TARGETS& a, const TARGET& b){
	return a&b; 
}

/* class RESULT */

bool	RESULT::operator  <  (const RESULT& r) const { 
	return value<r.value; 
}	

bool	RESULT::operator == (const RESULT& r) const { 
	return value == r.value; 
}	

RESULT	RESULT::operator += (const RESULT& r) const {
	return RESULT(value+r.value); 
}	

/* class TARGETSEARCH */

BITB		TARGETSEARCH::scope ( INFOGO* infop, TARGET tg){
	BITB tmp;
	switch (tg.aim) {
		case ROOT:
			return ~(infop->xx | infop->oo);
		case COMPLICATE			: break; 
			case TENNUO			: break; 
			case JIAO			: break; 
			case YUWEI			: break; 
			case SHIYINGSHOU	: break; 
			case ZUOJIE			: break; 
			case XUNJIE			: break; 
			case CHANGSHENG		: break; 
			case DUISHA			: break; 
		case SIMPLIFY			: break; 
			case DINGXING		: break; 
			case LIYONG			: break; 
			case ZHENGXIAN		: break; 
			case DAJIANG		: break; 
		case PUBLICAREA: break; 
			case ZHANDACHANG:
				//get:����
				//optimize:��տ������ڵ�������
				break; 
			case ZHIGAODIAN:
				//env, obj
				//get:˫����ս��Ӵ�or����
				break; 
		case HIS_CLUSTER: break; 
			case GE:
				//env, obj, obj
				//___ASSERT:��objͬ��һȺ
				//get:��������캣���Ӵ�
				//select:��env������
				break; 
			case FENG:
				//env, env, obj
				//get:������ս��Ӵ�
				//select:����env������
				break; 
			case GAN:
				//env, env, obj
				//get:obj���캣��env֮һ������
				//optimize:ĳenv����Ҫ��or����Ŀ������
				break; 
			case SHA:break; 
				case POYAN:
					//env, obj
					//get:obj�캣��������ḻ��
					//optimize:��env�����Ը�����
					break; 
				case SUOYANWEI:
					//env, obj
					//get:obj�캣������env������
					break; 
		case HIS_MASS:break; 
			case _ZHENGXING:
				//env, obj
				//get:obj��������env������
				//select:ʹobj�ϵ㱩¶��������
				break; 
			case CHIZI:break; 
				case DA:
					//env, obj
					//ʹobj��С�ռ���
					break; 
				case JIA:
					//env, obj
					//get:�������γ�obj��ΧȦ
					break; 
				case GUN:
					//env, obj
					//get:
					break; 
				case JIEBUGUI:
					//env, obj
					//get:����֮��objֻ��һ����
					break; 
				case ZHENGZI:
					//env, obj
					//����
					break; 
				case DAOTUOXUE:
					//env, obj
					//�ڱ�����ڽ�obj����
					break; 
				case DAOPU:
					//env, obj
					//������ʹobj���߻��ҷ���ȫ�鲢�����γ�����
					break; 
		case HIS_POTENTIAL:break; 
			case QINGXIAO:
				//cluster
				//___ASSERT:�������;޿�
				//get:cluster�����������õĹ����Ϸ�(���, ��, ����, ����)		
				break; 
			case BI:
				//cluster
				//get:cluster���ڷ������������ʵ�����(��������)
				break; 
			case SHENGTUI:
				//env, cluster
				//get:��env������ͻ��cluster�߿�
				break; 
			case QINSHI:
				//cluster
				//get:cluster����֮�������
				break; 
			case DARU:
				//cluster
				//get:cluster�������������֮��	
				break; 
		case MY_CLUSTER:break; 
			case _GE:
				//env, env
				//get:��env���Ӵ�
				break; 
			case TUWEI:
				//env, cluster, cluster
				//get:���߽��Ӵ�
				break; 
			case SHENGGEN:
				//env
				//get:�����캣
				break; 
			case ZUOHUO:break; 
				case ZUOYAN:
					//get:������ḻ��
					break; 
				case KUOYANWEI:
					//get:��������, ��һ�����캣
					break; 
		case MY_MASS				: break; 
			case ZHENGXING		:
				//env, obj
				//get:obj��������env������
				//select:ʹobj�ϵ������
				break; 
			case JIUZI			: break; 
				case _DA:
					//env, obj
					//��
					break; 
				case _JIA:
					//env, obj
					//get:�Ӱ�ΧȦͻΧ
					break; 
				case _GUN:
					//env, obj
					//get:
					break; 
				case _JIEBUGUI:
					//env, obj
					//get:�Խ�
					break; 
				case _ZHENGZI:
					//env, obj
					//����
					break; 
				case _DAOTUOXUE:
					//env, obj
					//�����ڱ�����ڽ�obj����
					break; 
				case _DAOPU:
					//env, obj
					//�Խ�
					break; 
		case MY_POTENTIAL				: break; 
			case KUOZHANG		: 
				//cluster
				//___ASSERT:�������;޿�
				//get:cluster�����������õĹ����Ϸ�(���, ��, ����, ����)	
				break; 
			case CHAI			: 
				//cluster
				//get:cluster���ڷ������������ʵ�����(��������)
				break; 
			case _SHENGTUI			: 
				//env, cluster
				//get:��env������ͻ��cluster�߿�
				break; 
			case _QINSHI			: 
				//cluster
				//get:cluster����֮�������
				break; 
			case _DARU		: 
				//cluster
				//get:cluster�������������֮��	
				break; 

		default	: 	;
	}
	return tmp; 
}

bool		TARGETSEARCH::isleaf ( INFOGO* infop, TARGET tg){
	switch (tg.aim){
		case ROOT:
			if (tg.maxdepth <= 0)
				return 1;
			else
				return 0;
		default:
			return 0;
	}
}

RESULT		TARGETSEARCH::leafvalue ( INFOGO* infop, TARGET tg){
	RESULT rs;
	switch (tg.aim){
	case ROOT:
		//command_state(*infop, BIND<string, char*>()(1, "mm"));
		rs.move = make_pair(infop->getlastpos(), infop->getlastclr());
		rs.value = infop->area.massmaps.back().delta();
		//___COUT3(rs.move.first, rs.move.second, rs.value);
		return rs;
	default:
        return rs;
	}
}

// targets ������һ�У�ÿ�б���Ԫ����ǰ��
VM TARGETSEARCH::findpath(INFOGO * infop, TARGETS tgs) {
	VM vm; 
	BITB b, bb; 
	b = NULL_BB; //
	for (int i = 0; i<tgs.nut.size (); ++i){
		bb = FULL_BB; 
		for (int j = 0; j<tgs.nut[i].size (); ++j)
			bb &= scope(infop, tgs.nut[i][j]); 
		b |= bb; 
	}
	//-// �˴����ö�ʽ����߲�Ƶ��������
	VI	allvi(BS*BS, 0), vi, idsort;
	COLOR trn = tgs.nut[0][0].passtimes ? infop->getlastclr ():infop->turn ();
	for (int i = 0; i<ZOBRIST::ZOBRIST_KIND; ++i) {
		vi = infop->getf_zobrist(i, b, trn);
		transform(vi.begin(), vi.end(), allvi.begin(), allvi.begin(), plus<int>());
	}
	___ASSERT(allvi.size()==BS*BS);
	idsort = indexsort(allvi);
	for (int i = BS*BS-1; i>=0; --i)
		if (allvi[idsort[i]])
			vm.push_back(make_pair(i2pos(idsort[i]), trn));
		else 
			break;
	if (!vm.empty())
		return vm;
	//-// �˴�����ģʽ����߲�Ƶ��������
	//g_learn.setpatfreq(infop);
	int m = 3;  
	vi = infop->getf_patfreq(m, b, trn);
	idsort = indexsort(vi);
	for (int i = BS*BS-1; i>=0; --i)
		if (vi[idsort[i]]>1){
			//___COUT1(i2pos(idsort[i]));
			vm.push_back(make_pair(i2pos(idsort[i]), trn));
		}
		else 
			break;
	if (!vm.empty())
		return vm;
	else {
		clog << "no match with lib_zobrist and lib_pemis ?!" << endl;
		return vm;
	}
}

bool TARGETSEARCH::isleaf(INFOGO * infop, TARGETS tgs){
	bool b, bb; 
	b = 0; 
	for (int i = 0; i<tgs.nut.size (); ++i){
		bb = 1; 
		for (int j = 0; j<tgs.nut[i].size(); ++j){
			bb &= isleaf(infop, tgs.nut[i][j]); 
			if (!bb)
				break; 
		}
		b |= bb; 
		if (b)
			break; 
	}
	return b; 
}

// Ϊ��ֹ�ѽڣ�Ӧ��leafvalue(INFOGO*, TARGET)��
// ����isleaf(INFOGO*, TARGET)
RESULT TARGETSEARCH::leafvalue(INFOGO * infop, TARGETS tgs){
	RESULT r, rr; 
	r = upbound(); 
	for (int i = 0; i<tgs.nut.size (); ++i){
		rr = RESULT( ); 
		for (int j = 0; j<tgs.nut[i].size(); ++j)
			rr += leafvalue(infop, tgs.nut[i][j]); 
		r = min(r, rr);                                    ////////////////
	}
	return r; 
}						

INFOGO*	TARGETSEARCH::findson(INFOGO * infop, MOVE mv){
	INFOGO* sonp = infop->move_tree(mv.first, mv.second );
	if (sonp != infop)
		sonp->update();
	return sonp; 
}

RESULT TARGETSEARCH::upbound() {
	return RESULT(INT_MAX); 
}

RESULT TARGETSEARCH::downbound() {
	return RESULT(INT_MIN); 
}

bool TARGETSEARCH::ismax(INFOGO * infop, TARGETS tgs) {
	COLOR trn = tgs.nut[0][0].passtimes ? infop->getlastclr ():infop->turn ();
	return trn == BLACK; 
}

TARGETS TARGETSEARCH::control(INFOGO * infop, TARGETS tgs){
	tgs.nut[0][0].maxdepth -= 1;
	if (tgs.nut[0][0].passtimes)
		tgs.nut[0][0].passtimes -= 1;
	return tgs;
}

RESULT TARGETSEARCH::recordpath(RESULT result, MOVE move) {
	result.move = move;
	return result;
}

/*
// ����������
void	TARGETSEARCH::lookfor(){
	aim = forward(aim); 
	for (; ; ){
		ensure(); 
		get(aim); 
		select(); 
		optimize(); 
		if (stop()) 
			return; 
		aim = anti(aim); 
	}
}
*/

/* _______________________________ ������ _______________________________ */

void TEST_TS::timefunc (string which) {
	RANDER  r; 
}
