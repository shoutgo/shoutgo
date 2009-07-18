#include "bitboard.h"
#include "itr.h"
#include "inout.h"

#if		BB_MACRO == 0
#undef  ___ASSERT
#define ___ASSERT(var) {}
#undef  ___PARASSERT
#define ___PARASSERT(var) {}
#undef	___FUNCOUNT
#define ___FUNCOUNT(fun) {}
#undef  ___LINECOUNT
#define ___LINECOUNT(var) {}
#undef  ___REPORT
#define ___REPORT(code) {}
//#undef  ___COUT1 ... 10
//#define ___COUT1 ... 10 {}
#undef  ___ESC
#define ___ESC {}
#undef	___TIME
#define ___TIME(code)	{}
#endif

BITB::BITB(ROW x){   
	////___FUNCOUNT( BB_BITB1); 
	for (int i = 0; i<BS; ++i)
		r[i] = x; 
}

BITB::BITB(ROW a[]) {  
	___FUNCOUNT( BB_BITB2); 
	for (int i = 0; i<BS; ++i) 
		r[i] = a[i]; 
}

BITB::BITB(const POS& p)	{  
	___FUNCOUNT( BB_BITB3); 
	for (int i = 0; i<BS; ++i) 
		r[i] = 0; 
	r[p.first ] = p.second; 
}

void BITB::random(){  
	___FUNCOUNT( BB_RANDOM); 
	srand((unsigned)clock()); // 测试时可用time()   2006.9.23
	for (int i = 0; i<BS; ++i) 
		r[i] = ((rand()<<15)+rand()) & ROWMASK; 
	//-//
}

BLOCK BITB::blockat(const POS& pos) const {  
	//-// 应该放入blockon(), 再调用
	___FUNCOUNT( BB_BLOCKAT); 
	___PARASSERT( !outside (pos) || pos == NULL_POS ); 

	if (( r[pos.first ] & pos.second ) == 0) 
	    return NULL_BB; 
	BLOCK     B, Bsnap; 
	B.r[pos.first] = rowexpand(r[pos.first], pos.second); 
	int  id = pos.first; 
	ROW  p; 
	int  d = ((id == 0)? 1:-1); 
	for (; ; ) {						// 此处如多次重复检测可提前转向, 待优化. 
		id += d; 
		p = B.r[id-d] & r[id]; 
		B.r[id] = rowexpand(r[id], p|B.r[id]); 
		if ((B.r[id] == 0)||(id == 0)&&(d == -1)||(id == BS-1)&&(d == 1)){
			if (Bsnap == B) return B; 
			Bsnap = B; 
			d *= -1; 
		}				
	}
}

BITB BITB::blockon_ (const BITB& t) const {  
	//-// 有待改进，仿blockat(POS)或者dialate()
	___FUNCOUNT( BB_BLOCKON); 
	BITB bb = *this; 
	ITR itr(bb); 
	for (BITB b = itr.blockbegin(); !itr.blockend(); b = itr.blocknext())
		if ((b&t) == NULL_BB)  
			bb ^= b; 
	return bb; 
}

BITB BITB::blockon (const BITB& t) const {  
	//-// 有待改进，仿blockat(POS)或者dialate()
	BITB snapbb, bb = *this & t; 
	for (; ; ){
		snapbb = bb; 
		bb = bb.dilate (1) & *this; 
		if (snapbb == bb)
			return bb; 
	}
}

bool BITB::only (const POS& pos) const { 
	___FUNCOUNT( BB_ONLY); 
	//-//
	for (int i = 0; i<BS; ++i) {
		if (i != pos.first) {
			if (r[i]>0)	
				return 0; 
		}
		else if ((r[i]^pos.second)>0)	
			return 0; 
	}
	return 1; 
}

int	BITB::count () const {  
	___FUNCOUNT( BB_COUNT); 
	int n = 0; 
	for (int i = 0; i<BS; ++i)   
		n += popu(r[i]); 
	return n; 
}

BITB BITB::transpose() const {  
	___FUNCOUNT( BB_TRANSPOSE); 
	BITB T; 
	for (int i = 0; i<BS; ++i) {
		for (int j = 0; j<BS; ++j) { 
			T.r[BS-i-1] = T.r[BS-i-1] | ((r[j]&(1<<i))>>i)<<(BS-j-1); 
		}
	}
	return T; 
}

BITB BITB::rotate(int angle) const {  
	___FUNCOUNT( BB_ROTATE); 
	___PARASSERT(angle == 0 || angle == 90 || angle == 180 || angle == 270); 
	BITB T; 
	switch (angle){
		case 90:
			for (int i = 0; i<BS; ++i) 
				for (int j = 0; j<BS; ++j) { 
					T.r[BS-i-1] = T.r[BS-i-1] | ((r[j]&(1<<i))>>i)<<j; 
				}
			break; 
		case 180:
			for (int i = 0; i<BS; ++i) 		
				T.r[i] = reverse( r[BS-1-i] )>>(32-BS); 
			break; 
		case 270:
			for (int i = 0; i<BS; ++i)
				for (int j = 0; j<BS; ++j) { 
					T.r[i] = T.r[i] | ((r[j]&(1<<i))>>i)<<(BS-j-1); 
				}
			break; 
		default:
			T = *this; 
	}
	return T; 
}

int	BITB::blockcount() const { 
	___FUNCOUNT( BB_BLOCKCOUNT); 
	ITR itr = ITR(*this); 
	int n = 0; 
	for (itr.blockbegin(); !itr.blockend(); itr.blocknext(), ++n){}; 
	return n; 
}

//输出为水平和垂直方向的长度
PII	BITB::range() const {					
	___FUNCOUNT( BB_RANGE); 
	if (*this == NULL_BB) 
		return 	make_pair(0, 0); 
	ROW rr = 0; //必须初始化为0
	int up = -1, down = -1; 
	for (int i = 0; i<BS; ++i){ 
		rr |= r[i]; 
		if ((up == -1) && r[i]>0 ) 
			up = i; 
		if ((down == -1) && r[BS-1-i]>0 ) 
			down = BS-1-i; 
	}
	return make_pair(down-up, 32-nleadingzero(rr)-ntailzero(rr)-1); 
}

BITB BITB::rangemask() const {  
	___FUNCOUNT( BB_RANGEMASK); 
	if (*this == NULL_BB) 
		return NULL_BB; 
	ROW rr = 0; 
	int up = -1, down = -1; 
	for (int i = 0; i<BS; ++i){ 
		rr |= r[i]; 
		if ((up == -1) && r[i]>0 ) 
			up = i; 
		if ((down == -1) && r[BS-1-i]>0 ) 
			down = BS-1-i; 
	}
    BITB bb; 
	int n = ntailzero(rr); 
	rr = (((1<<(32-nleadingzero(rr)-ntailzero(rr)))-1)<<ntailzero(rr)); 
	for (int i = up; i <= down; ++i)
		bb.r[i] = rr; 
	return bb; 
}

BITB BITB::dilate(int times) const {  
	___FUNCOUNT( BB_DILATE); 
	BITB ex, bb = *this; 
	for (int t = 0; t<times; ++t){
		ex.r[0] = (bb.r[0]|bb.r[0]<<1|bb.r[0]>>1|bb.r[1]); 
		for (int i = 1; i<BS-1; ++i)
			ex.r[i] = ( bb.r[i] | bb.r[i]>>1 | bb.r[i]<<1 | bb.r[i-1] | bb.r[i+1] ); 
		ex.r[BS-1] = ( bb.r[BS-1] | bb.r[BS-1]<<1 | bb.r[BS-1]>>1 | bb.r[BS-2] ); 
		ex &= ROWMASK; 
		swap(ex, bb); 
	}
	return bb; 
}

BITB BITB::dry(const BITB& bone, int times) const {  
	___FUNCOUNT( BB_DRY); 
	BITB ex, bb = *this; 
	for (int t = 0; t<times; ++t){
		ex.r[0] = bone.r[0]; 
		for (int i = 1; i<BS-1; ++i) 
			ex.r[i] = ( bone.r[i] |( bb.r[i]>>1 & bb.r[i]<<1 & bb.r[i-1] & bb.r[i+1]) ); 
		ex.r[BS-1] = bone.r[BS-1]; 
		swap(ex, bb); 
	}
	return bb; 
}

BITB BITB::adsorb(const BITB& bone, int times) const {  
	___FUNCOUNT( BB_ADSORB); 
	BITB ex, bb = *this; 
	for (int t = 0; t<times; ++t){
		ex = bb.dry(bone, 1); 
		for (int i = 1; i<BS-1; ++i) 
			ex.r[i] |= ((ex.r [i]<<1)&LEFTEST|(( ex.r [i]>>1)&1)); 
		ex.r[0] |= ex.r[1]; 
		ex.r[BS-1] |= ex.r[BS-2]; 
		swap(ex, bb); 
	}
	return bb; 
}

BITB BITB::inner(int times) const {  
	___FUNCOUNT( BB_INNER); 
	BITB bone = *this; // 2006.10.30
	BITB bb; 
	ITR itr(dilate (times)^bone); 
	for (BITB blk = itr.blockbegin(); !itr.blockend(); blk = itr.blocknext())
		if (((blk.dilate(1)^blk)|bone) == bone)
			bb |= blk; 
	return bb; 
}

bool BITB::notbigger (int x, int y) const {  
	___FUNCOUNT( BB_NOTBIGGER); 
	pair <int, int> rg = range(); 
	return ((rg.first <= x && rg.second <= y) 
			|| (rg.first <= y && rg.second <= x)); 
}

BITB BITB::ruboff (int x, int y) const {  
	___FUNCOUNT( BB_RUBOFF1); 
	BITB b = *this; 
	ITR itr = ITR(b); 
	for(BITB bb = itr.blockbegin(); !itr.blockend(); bb = itr.blocknext())
		if (bb.notbigger (x, y))  b ^= bb; 
	return b; 
}

BITB BITB::ruboff(const BITB& b) const {  
	___FUNCOUNT( BB_RUBOFF2); 
	return *this^(*this&b); 
}

BITB BITB::close (int t) const {  
	___FUNCOUNT( BB_CLOSE); 
	BITB bb = *this; 
	for(int i = 0; i<t; ++i)
		bb = bb.dilate (1).erode(1); 
	return bb; 
}

BITB BITB::open (int t) const {  
	___FUNCOUNT( BB_OPEN); 
	BITB bb = *this; 
	for(int i = 0; i<t; ++i)
		bb = bb.erode(1).dilate (1); 
	return bb; 
}

BITB BITB::erode(int times) const {  
	___FUNCOUNT( BB_ERODE); 
	BITB ex, bb = *this; 
	for (int t = 0; t<times; ++t){
		ex.r[0] = 0; 
		for (int i = 1; i<BS-1; ++i) 
			ex.r[i] = ( bb.r[i]>>1 & bb.r[i]<<1 & bb.r[i-1] & bb.r[i+1] ); 
		ex.r[BS-1] = 0; 
		swap(ex, bb); 
	}
	return bb; 
}

bool BITB::touch_(const BITB& bb) const {
	return (((*this) & bb) == NULL_BB)
			&& (((*this) & bb.dilate (1)) != NULL_BB); 
}

bool BITB::diagonal_touch_(const BITB& bb) const {
    return (((*this) & bb) == NULL_BB)
			&& (((*this) & bb.diagonal()) != NULL_BB); 
}

BITB BITB::diagonal() const {  
	BITB ex; 
	ex.r[0] = r[1]<<1 | r[1]>>1; 
	for (int i = 1; i<BS-1; ++i)
		ex.r[i] = r[i-1]>>1 | r[i-1]<<1 | r[i+1]>>1 | r[i+1]<<1; 
	ex.r[BS-1] = r[BS-2]<<1 | r[BS-2]>>1; 
	ex &= ROWMASK; 
	return ex ^ ( ex & dilate(1) ); 
}

BITB BITB::_fly (int a, int b) const {
	BITB ex; 
	int m; 
	for (int i = 0; i<BS; ++i){
		m = i-a; 
		if (m >= 0 && m<BS)
            ex.r[i] = r[m]>>b | r[m]<<b; 
		m = i+a; 
		if (m >= 0 && m<BS)
            ex.r[i] |= r[m]>>b | r[m]<<b; 
		m = i-b; 
		if (m >= 0 && m<BS)
            ex.r[i] |= r[m]>>a | r[m]<<a; 
		m = i+b; 
		if (m >= 0 && m<BS)
            ex.r[i] |= r[m]>>a | r[m]<<a; 
	}
	ex &= ROWMASK; 
	return ex ^ (ex & dilate(a+b-1)); 
}

BITB BITB::fly (int a, int b) const {
	BITB tmp = _fly(a, b); 
	if (a-b >= 2)
		return tmp^(tmp&_fly(a-1, b+1)); 
	else if (b-a >= 2)
		return tmp^(tmp&_fly(a+1, b-1)); 
	else
		return tmp; 
}

BITB BITB::diagonal_blockon_(const BITB& mask) const{
	return blockon(mask.diagonal())^blockon(mask); 
}

BITB BITB::touch_blockon_(const BITB& mask) const{
	return blockon(mask.dilate (1))^blockon(mask); 
}

// 注意与include()并不完全相应，ismassof() 更要以mass层次包含。
bool BITB::ismassof(const BITB& big) const {
	return big.blockon (*this) == (*this); 
}

//-// 注意增量算法利用到 tag 位置值最小
POS  BITB::tag() const{	
	POS p; 
	for(int i = 0; i<BS; ++i) 
		if (r[i]>0){
			p.first = i, 
			p.second = (((r[i]^(r[i]-1))+1)>>1); 
			return p; 
		}
	return NULL_POS; 
}

bool BITB::touchboarder() const {  
	___FUNCOUNT( BB_TOUCHBOARDER); 
	return ( *this & frame(star(CENTER), MIDBS) ) != NULL_BB; 
}

bool BITB::contain (const BITB& c) const {  
	___FUNCOUNT( BB_CONTAIN); 
	return (*this&c) == c; 
}

VP BITB::keypos_() const {  
	___FUNCOUNT( BB_KEYPOS); 
	return NULL_VP; 
}

VP BITB::jointpos() const {  
	___FUNCOUNT( BB_JOINTPOS); 
	BLOCK blk = *this; 
	ITR itr(blk); 
	VP vecpos; 
	for (POS p = itr.stonebegin (); !itr.stoneend (); p = itr.stonenext())
		if ((blk^p).blockcount()>1) vecpos.push_back(p); 
	return vecpos; 
}

// 子数小于等于7的BLOCK的模式特征码，
// 为各位置可扩展点数相乘结果，小于 5^7。
UL BITB::encode32() const {  
	___FUNCOUNT( BB_ENCODE32); 
	BLOCK blk = *this; 
	ITR itr(blk); 
	int n = 1; 
	for (POS p = itr.stonebegin (); !itr.stoneend (); p = itr.stonenext())
		n *= (BITB(p).dilate(1)&blk).count(); 
	return n; 
}
/*
BITB BITB::pemismask() const {
	BITB b = NULL_BB; 
	if ( ((*this) &g_init.getroute(R4DOWN_EAST)) != NULL_BB)
		b |= corner(EAST); 
	if ( ((*this) &g_init.getroute(R4DOWN_SOUTH)) != NULL_BB)
		b |= corner(SOUTH); 
	if ( ((*this) &g_init.getroute(R4DOWN_WEST)) != NULL_BB)
		b |= corner(WEST); 
	if ( ((*this) &g_init.getroute(R4DOWN_NORTH)) != NULL_BB)
		b |= corner(NORTH); 
	return project(b)
		|((*this).transpose ().project(b.transpose ()).transpose ()); 
}

// direction 的作用仅在于判断是否和四边接触，以便投射到相应边，
// 所以左上四分之一区与左上角点完全等价
BITB	BITB::project(const BITB& direction) const { 
	___FUNCOUNT( U_COMPACTMASK); 
	ROW s = 0, m = 0; 
	int up = -1, down = -1; 
	for (int i = 0; i<BS; ++i){
		if ((up == -1) && r[i]>0 ) 
			up = i; 
		if ((down == -1) && r[BS-1-i]>0 ) 
			down = BS-1-i; 
		s |= r[i]; 
		m |= direction.r[i]; 
	}
	BITB bb; 
	
	//if (direction.r[0])						//direction 接触上边界 
	//	for (int i = 0; i<up; ++i)
	//		bb.r[i] = s; 
	//if (direction.r[BS-1])					//direction 接触下边界 
	//	for (int i = down+1; i<BS; ++i)
	//		bb.r[i] = s; 
	
	if (m&LEFTEST)							//direction 接触左边界
		for (int i = 0; i<BS; ++i)
			if (r[i])
				bb.r[i] = ROWMASK & (r[i]|reverse(reverse(r[i])-1)); 
	if (m&1)								//direction 接触右边界
		for (int i = 0; i<BS; ++i)
			if (r[i])		
				bb.r[i] |= (r[i]-1); 
	return bb; 
}

//注意 NULL_BB 结果为 1
PUU BITB::pemis3232_(const BITB& patmask) const {			
	___FUNCOUNT( BB_PEMIS3232); 
	vector<short> vecs; 
	BITB patbb = blockon(patmask); 
	BITB mask = patbb.pemismask(); 
	//___COUT4(*this, patmask, patbb, mask); 
	ITR itr(patbb); 
	for (POS p = itr.stonebegin (); !itr.stoneend (); p = itr.stonenext())
		vecs.push_back((BITB(p).dilate(1)&patbb).count() + onboarder(p)*BS); //
	//___COUT1(vecs); 
	short a, c; 
	for(int i = 0; i<BS; ++i){
		if(patbb.r[i]){
			a = nleadingzero(patbb.r[i])+BS-32; 
			c = ntailzero(patbb.r[i]); 
			vecs.push_back (a*( (mask.r[i]&LEFTEST)>0 )+(BS-a-c)+c*((mask.r[i]&1)>0)+BS); //
		}
	}
	//___COUT1(vecs); 
	patbb = patbb.transpose (); 
	mask = mask.transpose (); 
	for(int i = 0; i<BS; ++i){
		if(patbb.r[i]){
			a = nleadingzero(patbb.r[i])+BS-32; 
			c = ntailzero(patbb.r[i]); 
			vecs.push_back (a*( (mask.r[i]&LEFTEST)>0 )+(BS-a-c)+c*((mask.r[i]&1)>0)+BS); //
		}
	}
	//___COUT1(vecs); 
	unsigned high32 = 0, low32 = 1; 
	for(int i = 0; i<vecs.size (); ++i) {							
		high32 = high32*vecs[i] + mulhigh32(low32, vecs[i]); 
		low32 *= vecs[i]; 
	}															
	unsigned y = patbb.rangemask().count(); //这样才能区别小飞与尖等简单棋形 
	if ( ~low32 < y ) 
		high32 += 1; 
	low32 += y; 
	return make_pair(high32, low32); 
}

//算法应与 project() 有关!
ULL	BITB::pemis64_(const BITB& patmask) const {				
	___FUNCOUNT( BB_PEMIS64); 
	ULL tmp = 1; 
	BITB patbb = blockon(patmask); 
	BITB mask = patbb.pemismask(); 
	//___COUT4(*this, patmask, patbb, mask); 
	ITR itr(patbb); 
	for (POS p = itr.stonebegin (); !itr.stoneend (); p = itr.stonenext()){
		tmp *= (ULL)((BITB(p).dilate(1)&patbb).count() + onboarder(p)*BS); //	
		//___COUT2(1, tmp); 
	}																				
	int a, c; 
	for(int i = 0; i<BS; ++i){
		if(patbb.r[i]){
			a = nleadingzero(patbb.r[i])+BS-32; 
			c = ntailzero(patbb.r[i]); 
			tmp *= (ULL)(a*( (mask.r[i]&LEFTEST)>0 )+(BS-a-c)+c*((mask.r[i]&1)>0)+BS); //
		//___COUT2(2, tmp); 
		}
	}																					
	patbb = patbb.transpose (); 
	mask = mask.transpose (); 
	//___COUT2(patbb, mask); 
	for(int i = 0; i<BS; ++i){
		if(patbb.r[i]){
			a = nleadingzero(patbb.r[i])+BS-32; 
			c = ntailzero(patbb.r[i]); 
			tmp *= (ULL)(a*((mask.r[i]&LEFTEST)>0)+(BS-a-c)+c*((mask.r[i]&1)>0)+BS); //
		//___COUT2(3, tmp); 
		}
	}																					
	return tmp + (ULL)(patbb.rangemask().count()); 
	//return tmp * (ULL)(patbb.rangemask().count()+(ULL)BS*BS); 
}
*/

// 先将其方形化，再看是否与四海接触，投射成一更大的方形。
BITB	BITB::project( ) const { 
	BITB tmp = rangemask(); 
	ROW s = 0, m = 0; 
	int up = -1, down = -1; 
	for (int i = 0; i<BS; ++i){
		if ((up == -1) && tmp.r[i]>0 ) 
			up = i; 
		if ((down == -1) && tmp.r[BS-1-i]>0 ) 
			down = BS-1-i; 
		s |= tmp.r[i]; 
	}
	BITB bb = tmp; 
	if (!(tmp &g_init.getroute(R4DOWN_NORTH)).empty())// 接触上海 
		for (int i = 0; i<up; ++i)
			bb.r[i] = s; 
	if (!(tmp &g_init.getroute(R4DOWN_SOUTH)).empty())// 接触下海 
		for (int i = down+1; i<BS; ++i)
			bb.r[i] = s; 
	// 上下操作完再操作左右，用更新后的 bb，对每行
	if (!(tmp &g_init.getroute(R4DOWN_WEST)).empty())// 接触左海
		for (int i = 0; i<BS; ++i)
			if (bb.r[i])
				bb.r[i] = ROWMASK & (bb.r[i]|reverse(reverse(bb.r[i])-1)); 
	if (!(tmp &g_init.getroute(R4DOWN_EAST)).empty())// 接触右海
		for (int i = 0; i<BS; ++i)
			if (bb.r[i])		
				bb.r[i] |= (bb.r[i]-1); 
	return bb; 
}

vector<short> BITB::pospemis() const {
	vector<short> vs; 
	ITR itr = ITR(*this); 
	for (POS p = itr.stonebegin (); !itr.stoneend (); p = itr.stonenext())
		vs.push_back((BITB(p).dilate(1)&(*this)).count() + onboarder(p)*BS); 
	return vs; 
}

vector<short> BITB::rowpemis() const {
	vector<short> vs; 
	bool leftproject = !(((*this)&g_init.getroute(R4DOWN_WEST)).empty ()); 
	bool rightproject = !(((*this)&g_init.getroute(R4DOWN_EAST)).empty ()); 
	short a, c; 
	for (int i = 0; i<BS; ++i)
		if( r[i] ){
			a = nleadingzero( r[i])+BS-32; 
			c = ntailzero( r[i]); 
			vs.push_back ( a*leftproject + (BS-a-c) + c*rightproject); // + BS
		}
	return vs; 
}

ULL BITB::pemis64() const {
	vector<short> vs = pospemis(); 
	vector<short> v1 = rowpemis(); 
	vector<short> v2 = transpose ().rowpemis(); 
	copy(v1.begin (), v1.end (), back_inserter(vs)); 
	copy(v2.begin (), v2.end (), back_inserter(vs)); 
	ULL tmp = 1; 
	for (int i = 0; i<vs.size (); ++i)
		tmp *= (ULL) vs[i]; 
	return tmp + (ULL)(rangemask().count()); 
}

PUU	 BITB::pemis3232() const{
	vector<short> vs = pospemis(); 
	vector<short> v1 = rowpemis(); 
	vector<short> v2 = transpose ().rowpemis(); 
	copy(v1.begin (), v1.end (), back_inserter(vs)); 
	copy(v2.begin (), v2.end (), back_inserter(vs)); 
	unsigned high = 0, low = 1; 
	for(int i = 0; i<vs.size (); ++i) {							
		high = high*vs[i] + mulhigh32(low, vs[i]); 
		low *= vs[i]; 
	}															
	unsigned y = rangemask().count(); //这样才能区别小飞与尖等简单棋形 
	if ( ~low < y ) 
		high += 1; 
	low += y; 
	return make_pair(high, low); 
}

// 可利用encode32()和jointp()直接给出结果.
// 如 encode32 不重复, 可不用count().
pair<EYEKIND, VP > BITB::eyekind(ACTION action) const { 
	___FUNCOUNT( BB_EYEKIND); 
	if (blockcount()>1 || count()>7)
		return make_pair(SOMEEYE, NULL_VP); 
	if ((action == AFTERDELETE)||(action == KILL)){
		VP jointp = jointpos(); 
		switch (jointp.size()){
			case 0:
				if (count() == 6)	
					return make_pair(SOMEEYE, NULL_VP); 
				else					
					return make_pair(ONEEYE, NULL_VP); 
			case 1:
				if (count() == 7)
					if (encode32() == 3645) 
						return make_pair(CO_ONEEYE, jointp); 
					else				
						return make_pair(SOMEEYE, NULL_VP); 
				else					
					return make_pair(ONEEYE, jointp); 
			default:
				return make_pair(SOMEEYE, NULL_VP); 
		}
	}
	if (action == RESCUE){
		pair<EYEKIND, VP >  result = eyekind(AFTERDELETE); 
		switch (result.first) {
			case ONEEYE:
				if (!result.second.empty()) 
					return make_pair(SOMEEYE, result.second ); 
				else						
					return make_pair(ONEEYE, NULL_VP); 
			case SOMEEYE:					
				return make_pair(SOMEEYE, NULL_VP); 
			case CO_ONEEYE:					
				return make_pair(SOMEEYE, result.second ); 
		}
	}
}

bool BITB::empty() const{
	return (*this) == NULL_BB; 
}

BITB	BITB::wrapper_(int m) const {  
	BITB bb; 
	if (m>0){
		bb = dilate (m-1); 
		return bb^bb.dilate (1); 
	}
	else {
		bb = erode (-1*m); 
		return bb^bb.erode (1); 
	}
}

// 可负可正，仅在边界上相交时为 0
// 只膨胀一方，结果会不会对称？应该是对称的。
int  BITB::dist(const BITB& bb) const{
	BITB tmp = bb; 
	if (!((*this)&tmp).empty())
		for (int i = 0; ; --i){
			tmp = tmp.erode (1); 
			if (((*this)&tmp).empty ())
				return i; 
		}
	else
		for (int i = 0; ; ++i){
			tmp = tmp.dilate (1); 
			if  (!((*this)&tmp).empty ())
				return i+1; 
		}
}

BITB BITB::land(int times) const {
	BITB tmp = (*this).dilate(times); 
	return tmp.dry(*this, times); 
}

BITB BITB::sea(int times) const {
	BITB tmp = (*this).dilate(times); 
	return tmp.adsorb(*this, times); 
}

pair<BITB, PII> BITB::relation_(const BITB& bb) const{
	int m = dist (bb); 
	if (m <= 0)
		return make_pair((*this)&bb, make_pair(m, 0)); 
	else{
		int x = m/2, y = m - x; 
		BITB onthis, onbb; 
		for (int i = 0; x >= 0; x -= i, y += i){
			onbb = fly(x, y) & bb; 
			if (!onbb.empty ())
				onthis = bb.fly (x, y) &(*this); 
			return make_pair( onthis|onbb, make_pair(x, y) ); 
		}
	}
}

///////////

BITB BITB::operator~   ()	 const { 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = (~r[i])&ROWMASK; 
	return tmp; 
}
BITB BITB::operator&   (const BITB& bb) const { 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]&bb.r[i]; 
	return tmp; 
}
BITB BITB::operator|   (const BITB& bb) const { 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]|bb.r[i]; 
	return tmp; 
}
BITB BITB::operator^   (const BITB& bb) const { 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]^bb.r[i]; 
	return tmp; 
}
/*
BITB BITB::operator>>  (int n)  const { 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]>>n; 
	return tmp; }
BITB BITB::operator<<  (int n)  const {
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = (r[i]<<n)&ROWMASK; 
	return tmp; 
}
void	 BITB::operator> >= (int n)	{ 
	for (int i = 0; i<BS; ++i)  
		r[i]> >= n; 
}
void	 BITB::operator< <= (int n)	{ 
	for (int i = 0; i<BS; ++i)  
		r[i]< <= n, r[i] &= ROWMASK; 
}			
*/
void	 BITB::operator &= (const BITB& bb)		{ 
	for (int i = 0; i<BS; ++i)  
		r[i] &= bb.r[i]; 
}
void	 BITB::operator |= (const BITB& bb)	    { 
	for (int i = 0; i<BS; ++i)  
		r[i] |= bb.r[i]; 
}
void	 BITB::operator ^= (const BITB& bb)	    {
	for (int i = 0; i<BS; ++i)  
		r[i] ^= bb.r[i]; 
}
bool	 BITB::operator<  (const BITB& bb) const {  
	for (int i = 0; i<BS; ++i) 
		if (r[i]<bb.r[i]) 
			return 1; 
		else if (r[i]>bb.r[i]) 
			return 0; 
	return 0; 
}
/*
bool	 BITB::operator <= (const BITB& bb) const { 
	for (int i = 0; i<BS; ++i) 
		if (r[i]<bb.r[i]) 
			return 1; 
		else if (r[i]>bb.r[i]) 
			return 0; 
	return 1; 
}
bool	 BITB::operator>   (const BITB& bb) const { 
	for (int i = 0; i<BS; ++i) 
		if (r[i]<bb.r[i]) 
			return 0; 
		else if (r[i]>bb.r[i]) 
			return 1; 
	return 0; 
}
bool	 BITB::operator >= (const BITB& bb) const { 
	for (int i = 0; i<BS; ++i) 
		if (r[i]<bb.r[i]) 
			return 0; 
		else if (r[i]>bb.r[i]) 
			return 1; 
	return 1; 
}
*/
bool	 BITB::operator == (const BITB& bb) const { 
	for (int i = 0; i<BS; ++i) 
		if (r[i]<bb.r[i]||bb.r[i]<r[i]) 
			return 0; 
	return 1; 
}									//这两句可以用 != .待改。2006.9.9
bool	 BITB::operator != (const BITB& bb) const { 
	for (int i = 0; i<BS; ++i) 
		if (r[i]<bb.r[i]||bb.r[i]<r[i]) 
			return 1; 
	return 0; 
}									
/*
BITB BITB::operator&	 (const ROW rr)	  const { 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]&rr; 
	return tmp; 
}
BITB BITB::operator|   (const ROW rr)	  const { 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]|rr; 
	return tmp; 
}
BITB BITB::operator^   (const ROW rr)	  const { 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]^rr; 
	return tmp; 
}
*/
/*
BITB BITB::operator& (const POS& pos)	const	{ 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]; 
	tmp.r[pos.first] = r[pos.first]&pos.second; 
	return tmp; 
}
*/
BITB BITB::operator|   (const POS& pos)const	{ 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]; 
	tmp.r[pos.first] = r[pos.first]|pos.second; 
	return tmp; 
}
BITB BITB::operator^   (const POS& pos)const	{ 
	BITB tmp; 
	for (int i = 0; i<BS; ++i) 
		tmp.r[i] = r[i]; 
	tmp.r[pos.first] = r[pos.first]^pos.second; 
	return tmp; 
}
/*
bool	 BITB::operator&&  (const POS& pos)const	{ 
	return (r[pos.first]&pos.second) > 0; 
} 
bool	 BITB::operator||  (const POS& pos)const	{ 
	return (r[pos.first]|pos.second) > 0; 
}
bool	 BITB::operator!   ( )	 const	{ 
}
void	 BITB::operator &= (const POS& pos)		{ 
	r[pos.first] &= pos.second; 
}
*/
void	 BITB::operator |= (const POS& pos)		{ 
	r[pos.first] |= pos.second; 
}
void	 BITB::operator ^= (const POS& pos)		{ 
	r[pos.first] ^= pos.second; 
}
bool	 BITB::operator[]  (const POS& pos)  const	{ 
	return (r[pos.first]&pos.second) >0; 
}

/* _______________________________ 测试区 _______________________________ */

void TEST_BB::fly(){
	BITB tmp; 
	tmp.random (); 
	ITR itr = ITR(tmp); 
	for (BITB bb = itr.blockbegin(); !itr.blockend(); bb = itr.blocknext()){
		___COUT1( GO(bb, bb.diagonal()) ); 
		___COUT8( 
			GO(bb, bb.fly (0, 1)), 
			GO(bb, bb.fly (1, 1)), 
			GO(bb, bb.fly (1, 2)), 
			GO(bb, bb.fly (2, 1)), 
			GO(bb, bb.fly (0, 2)), 
			GO(bb, bb.fly (3, 0)), 
			GO(bb, bb.fly (3, 1)), 
			GO(bb, bb.fly (3, 2))
			); 
	}
}

void TEST_BB::timefunc (string which) {
		 
	RANDER  r; 

	BITB	bb = r.vb[0]; 

	___TIME(BB_RANDOM, bb.random(); ); 
	___TIME(BB_ONLY, bb.only( r.vp[0]); ); 
	___TIME(BB_COUNT, bb.count(); ); 
	___TIME(BB_BLOCKCOUNT, bb.blockcount(); ); 
	//___TIME(BB_BLOCKAT, bb.blockat(  r.vp[0]); ); 
	___TIME(BB_TRANSPOSE, bb.transpose(); ); 
	___TIME(BB_ROTATE, bb.rotate( 90); ); 
	___TIME(BB_RANGE, bb.range(); ); 
	___TIME(BB_RANGEMASK, bb.rangemask(); ); 
	___TIME(BB_DILATE, bb.dilate(  2); ); 
	___TIME(BB_ERODE, bb.erode( r.vi[0]); ); 
	___TIME(BB_DRY, bb.dry(  r.vb[0], r.vi[0]); ); 
	___TIME(BB_ADSORB, bb.adsorb(  r.vb[0], r.vi[0]); ); 
	___TIME(BB_INNER, bb.inner(  r.vi[0]); ); 
	___TIME(BB_NOTBIGGER, bb.notbigger(  r.vi[0], r.vi[1]); ); 
	___TIME(BB_RUBOFF1, bb.ruboff(  r.vb[0]); ); 
	___TIME(BB_RUBOFF2, bb.ruboff(  r.vi[0], r.vi[1]); ); 
	___TIME(BB_CLOSE, bb.close(  r.vi[0]); ); 
	___TIME(BB_OPEN, bb.open(  r.vi[0]); ); 
	___TIME(BB_BLOCKON, bb.blockon(  r.vb[0]); ); 
	___TIME(BB_TOUCHBOARDER, bb.touchboarder(); ); 
	___TIME(BB_CONTAIN, bb.contain(  r.vb[0]); ); 
	___TIME(BB_KEYPOS, bb.keypos_(); ); 
	___TIME(BB_JOINTPOS, bb.jointpos(); ); 
	___TIME(BB_ENCODE32, bb.encode32(); ); 
	___TIME(BB_PEMIS3232, bb.pemis3232( ); ); 
	___TIME(BB_PEMIS64, bb.pemis64( ); ); 
	___TIME(BB_EYEKIND, bb.eyekind(  r.va[0]); ); 

}



