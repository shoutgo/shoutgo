#include "fir.h"
#include "inout.h"

																		//性能优化可能大大提高速度，
																		//但表现在搜索层次上可能只增加一两层，
																		//因为后一层的结点数量远远大于上一层。

																		//搜索到一定程度程序可能已经认识到必输，
																		//从而每处选点都一样；
																		//而人类认识不到，反而认为有最好点，
																		//造成程序没有搜索正确的假象。
const int	FIR::FIRNUM = 4; 
const int	FIR::FIR_MAX = 10000; 

PIP FIR::minmax(int n){								
	++nodenum; //cout<<"n = "<<n<< endl<<(*this); // fine
	if (n == 0)
		return leafvalue(); 
	//if (over())
	//	return make_pair((getlastclr() == BLACK)?FIR_MAX:-FIR_MAX, PASS_POS); 
	
	BITB rangebb = xx|oo; 
	rangebb ^= rangebb.dilate (2); 
	
	GO snapgo = snap(); 
	vector<PIP > vec; 
	ITR itr = ITR(rangebb); 
	for (POS pos = itr.stonebegin(); !itr.stoneend(); pos = itr.stonenext()){
		add(pos, turn()); 
		if (over())
			return make_pair((getlastclr() == BLACK)?FIR_MAX:-FIR_MAX, lastpos); 
		vec.push_back(make_pair(minmax( n-1).first, pos)); 
		restore(snapgo); 
	}
	return	turn() == BLACK ?
			*max_element(vec.begin (), vec.end (), ppless):
			//vec[max_element(vec.begin (), vec.end (), ppless)-vec.begin ()]:
			vec[min_element(vec.begin (), vec.end (), ppless)-vec.begin ()]; 
}

/*
PIP FIR::alphabeta(int n, int alpha, int beta){
	return make_pair(0, NULL_POS); 
}
*/

/*
PIP FIR::minmax(int n){								//速度似乎有提高，但搜索范围太局限导致结果不正确
	++nodenum; //cout<<(*this); // fine
	if (n == 0)
		return leafvalue(); 
	vector<PIP > vec; 
	GO snapgo = snap(); 
	for (int i = 0; i<vecp.size (); ++i){
		POS pos = vecp[i]; //
		swap(vecp[i], vecp.back ()); //
		vecp.pop_back (); //
																		//if (getbb(turn())[pos])
																		//	cout<<(*this)<<pos; 
		add(pos, turn()); 
		if (over())
			return make_pair((getlastclr() == BLACK)?FIR_MAX:-FIR_MAX, lastpos); 
		vec.push_back(make_pair(minmax( n-1).first, pos)); 
		vecp.push_back (pos); //
		swap(vecp[i], vecp.back ()); //
		restore(snapgo); //
	}
	return	turn() == BLACK ?
			vec[max_element(vec.begin (), vec.end (), ppless)-vec.begin ()]:
			vec[min_element(vec.begin (), vec.end (), ppless)-vec.begin ()]; 
}
*/

void FIR::scope(int a){
	BITB rangebb = xx|oo; 
	rangebb ^= rangebb.dilate (2); 
	ITR itr = ITR(rangebb); 
	vecp.clear(); 
	for (POS pos = itr.stonebegin (); !itr.stoneend (); pos = itr.stonenext())
		vecp.push_back (pos); 
}

PIP FIR::leafvalue(){									//注意这里可以调用minmax()继续搜索
	VI vi, vj; //cout<<vecp.size (); 
	vi = link4(getbb(getlastclr()), lastpos); 
	INFOGO info = snap(); //
	info.getbb(info.getlastclr()).r[info.getlastpos() .first] ^= info.getlastpos() .second; //
	info.add(info.getlastpos(), info.turn()); //
	vj = link4(info.getbb(info.getlastclr()), info.getlastpos() ); 
	int c = (getlastclr() == BLACK)?1:-1; 
	return make_pair(c*(accumulate(vi.begin(), vi.end (), 0)+accumulate(vj.begin(), vj.end (), 0)), lastpos); 
}

bool FIR::over(){
	VI  vi = link4(getbb(getlastclr()), lastpos); 
	return find(vi.begin (), vi.end (), FIRNUM) != vi.end (); 
}

POS	FIR::genmove(){
	scope (4); 
	return minmax(4).second; 
}

bool FIR::moveable(POS pos, COLOR clr){
	return !(xx|oo)[pos]; 
}

bool FIR::move(POS pos, COLOR clr){
	add(pos, clr); 
	return 1; 
}

GO	 FIR::snap() const {  
	return *this; 
}

void FIR::restore(const GO& go){ 
	// *this = go; 
}

bool ppless(const PIP & m1, const PIP & m2) {
	return m1.first < m2.first; 
}

VI link4(BITB b, POS p){								// direction: -\|/
	VI vi; 
	vi.reserve(4); // 比 vi.assign(4, 0) 性能大大提高，其它处尚未优化
	vi.push_back( popu(rowexpand(b.r[p.first], p.second))); 
	int i, j; 
	for (i = 1; p.first-i >= 0 && b[make_pair(p.first-i, p.second<<i)]; ++i); 
	for (j = 1; p.first+j<BS && b[make_pair(p.first+j, p.second>>j)]; ++j); 
	vi.push_back(i+j-1); 
	for (i = 1; p.first-i >= 0 && b[make_pair(p.first-i, p.second)]; ++i); 
	for (j = 1; p.first+j<BS && b[make_pair(p.first+j, p.second)]; ++j); 
	vi.push_back(i+j-1); 
	for (i = 1; p.first-i >= 0 && b[make_pair(p.first-i, p.second>>i)]; ++i); 
	for (j = 1; p.first+j<BS && b[make_pair(p.first+j, p.second<<j)]; ++j); 
	vi.push_back(i+j-1); 
	return vi; 
}

int link(BITB b, POS p){							
	return popu(rowexpand(b.r[p.first], p.second)); 
}

