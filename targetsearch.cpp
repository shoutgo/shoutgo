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

//注意unique()只整合相邻的相同元素，
//所以最好先排序才能删除间隔但相同者；
//除非每次只插入一个新元素。待完善。
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
	for (int i = 0; i<a.nut.size (); ++i)						//将 a.nut 按行（与结构）每个元素取~后
		for (int j = 0; j<a.nut[i].size (); ++j){				//竖排（或结构）形成一个targets, 共a.nut.size()个
			vt.clear (); 
			vt.push_back (~a.nut[i][j]); 
			vts[i].nut.push_back (vt); 
		}
	TARGETS tmp = vts[0]; 
	for (int i = 1; i<vts.size (); ++i)							//将这些targets连&
        tmp = tmp & vts[i]; 
	for (int i = 0; i<tmp.nut.size (); ++i)						//归整每行（与结构）
		unique(tmp.nut[i].begin (), tmp.nut[i].end ()); 
	unique(tmp.nut.begin (), tmp.nut.end ()); //归整所有行（或结构）
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
				//get:公海
				//optimize:最空旷处或邻敌最弱处
				break; 
			case ZHIGAODIAN:
				//env, obj
				//get:双方领空交接处or公空
				break; 
		case HIS_CLUSTER: break; 
			case GE:
				//env, obj, obj
				//___ASSERT:两obj同属一群
				//get:三者领空领海交接处
				//select:与env相连者
				break; 
			case FENG:
				//env, env, obj
				//get:三者领空交接处
				//select:与两env相连者
				break; 
			case GAN:
				//env, env, obj
				//get:obj的领海与env之一相连者
				//optimize:某env更需要者or新增目更大者
				break; 
			case SHA:break; 
				case POYAN:
					//env, obj
					//get:obj领海中眼形最丰富处
					//optimize:与env联络性更大者
					break; 
				case SUOYANWEI:
					//env, obj
					//get:obj领海中且与env相连者
					break; 
		case HIS_MASS:break; 
			case _ZHENGXING:
				//env, obj
				//get:obj邻域内与env相连者
				//select:使obj断点暴露或增多者
				break; 
			case CHIZI:break; 
				case DA:
					//env, obj
					//使obj向小空间逃
					break; 
				case JIA:
					//env, obj
					//get:新增点形成obj包围圈
					break; 
				case GUN:
					//env, obj
					//get:
					break; 
				case JIEBUGUI:
					//env, obj
					//get:接上之后obj只有一口气
					break; 
				case ZHENGZI:
					//env, obj
					//搜索
					break; 
				case DAOTUOXUE:
					//env, obj
					//在被提块内紧obj的气
					break; 
				case DAOPU:
					//env, obj
					//送死后使obj触边或触我方安全块并不能形成真眼
					break; 
		case HIS_POTENTIAL:break; 
			case QINGXIAO:
				//cluster
				//___ASSERT:非中央型巨空
				//get:cluster中起桥梁作用的孤子上方(肩冲, 镇, 马步飞, 碰等)		
				break; 
			case BI:
				//cluster
				//get:cluster所在方形区域左右适当距离(间二或间三)
				break; 
			case SHENGTUI:
				//env, cluster
				//get:与env相连且突入cluster边空
				break; 
			case QINSHI:
				//cluster
				//get:cluster防线之上最薄弱处
				break; 
			case DARU:
				//cluster
				//get:cluster最疏松最多利用之处	
				break; 
		case MY_CLUSTER:break; 
			case _GE:
				//env, env
				//get:两env交接处
				break; 
			case TUWEI:
				//env, cluster, cluster
				//get:三者交接处
				break; 
			case SHENGGEN:
				//env
				//get:扩大领海
				break; 
			case ZUOHUO:break; 
				case ZUOYAN:
					//get:眼形最丰富处
					break; 
				case KUOYANWEI:
					//get:扩大眼域, 不一定是领海
					break; 
		case MY_MASS				: break; 
			case ZHENGXING		:
				//env, obj
				//get:obj邻域内与env相连者
				//select:使obj断点减少者
				break; 
			case JIUZI			: break; 
				case _DA:
					//env, obj
					//接
					break; 
				case _JIA:
					//env, obj
					//get:从包围圈突围
					break; 
				case _GUN:
					//env, obj
					//get:
					break; 
				case _JIEBUGUI:
					//env, obj
					//get:自接
					break; 
				case _ZHENGZI:
					//env, obj
					//搜索
					break; 
				case _DAOTUOXUE:
					//env, obj
					//防备在被提块内紧obj的气
					break; 
				case _DAOPU:
					//env, obj
					//自接
					break; 
		case MY_POTENTIAL				: break; 
			case KUOZHANG		: 
				//cluster
				//___ASSERT:非中央型巨空
				//get:cluster中起桥梁作用的孤子上方(肩冲, 镇, 马步飞, 碰等)	
				break; 
			case CHAI			: 
				//cluster
				//get:cluster所在方形区域左右适当距离(间二或间三)
				break; 
			case _SHENGTUI			: 
				//env, cluster
				//get:与env相连且突入cluster边空
				break; 
			case _QINSHI			: 
				//cluster
				//get:cluster防线之上最薄弱处
				break; 
			case _DARU		: 
				//cluster
				//get:cluster最疏松最多利用之处	
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

// targets 至少有一行，每行必有元素是前提
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
	//-// 此处利用定式库的走步频率来排序
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
	//-// 此处利用模式库的走步频率来排序
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

// 为防止脱节，应在leafvalue(INFOGO*, TARGET)中
// 调用isleaf(INFOGO*, TARGET)
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
// 粗粒度搜索
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

/* _______________________________ 测试区 _______________________________ */

void TEST_TS::timefunc (string which) {
	RANDER  r; 
}
