#ifndef _DEFINE
#define _DEFINE

//_______ 头文件包含关系 ________
//
// define.h 
//   utility.h 
//     bitboard.h 
//       itr.h 
//       board.h 
//   transptable.h
//         infoboard.h 　
//   search.h *
//           targetsearch.h
//   nn.h *
//           learn.h 
//             brain.h
//		     tsumego.h 
//		     fir.h 
//               inout.h 
//               test.h 
//               go.h
//
//______有 * 者仅被紧邻的上级包含

#include <iostream>		// VS2008
//#include <tchar.h>		// VS2008

using namespace std; 

#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>
#include <map>
#include <hash_map>
#include <set>
#include <bitset>
#include <queue>

#include <ctime>
#include <cassert>
#include <cmath>
#include <climits>

#include <iomanip>
#include <fstream>
#include <sstream>

#include <typeinfo>

/*
enum	MODE_PRINT			{BOARD_P, BLOCK_P, BITB_P, ONEAREA_P}; 
enum	MODE_PRINTLABEL		{L_CLUSTER, L_AREA1, L_AREA2}; 
*/
enum	MODE_PROMPT			{BEGINGAME, PREMOVE_MAN, AFTERMOVE_MAN, 
  							PREMOVE_PC, AFTERMOVE_PC, ENDGAME, BITSET}; 
enum	MODE_PLAY			{MAN_PC, PC_PC, MAN_MAN}; 

enum	MODE_RANDOMEYE		{LESS_LESS, BIGGER_LESS, 
  							LESS_LESS_CENTER, EQ_EQ}; 
enum	MODE_RANDOMBOARD	{LEGALIZE, ILLEGALIZE}; 
enum	DIRECTION			{EAST = 0, SOUTH = 1, WEST = 2, NORTH = 3, 
  							ES, EN, WS, WN, CENTER, DIRECTION_SIZE}; 
enum	COLOR				{BLACK, WHITE, EMPTY, BLACK_WHITE, 
  							HOTKO, NULL_CLR, COLOR_SIZE}; 
enum	ACTION				{AFTERDELETE, KILL, RESCUE, ACTION_SIZE}; 
enum	EYEKIND				{ENEMY_ALIVE, FALSEEYE, ONEEYE, CO_ONEEYE, 
  							CO_SOMEEYE, TWOEYE, SOMEEYE, MANYEYE, 
  							UNKOWN_WHENPASS}; 
enum  ROUTE				{R3 = 0, R4, R5, R4UP, R4DOWN, R4DOWN_EAST, 
  							R4DOWN_SOUTH, R4DOWN_WEST, R4DOWN_NORTH}; 
enum	STATE				{ALIVE, DEAD, UNCLEAR}; 
const string eyekindname[]    = {"ENEMY_ALIVE", "FALSEEYE", "ONEEYE", 
								"CO_ONEEYE", "CO_SOMEEYE", "TWOEYE", 
								"SOMEEYE", "MANYEYE", "UNKOWN_WHENPASS"}; 

class	BITB; 
class	GO; 
class	LEARN; 

typedef		string				RGB;

typedef		int						VALUE; 
typedef		unsigned				ROW, KEY, LIKELIHOOD; 
typedef		unsigned long			UL; 
typedef		unsigned long long		ULL; 
typedef		float					ParaTYPE; //-//
typedef		BITB					MASK, BLOCK; //, CHAIN, MASS, CLUSTER; 

typedef		pair<int, ROW>			POS; 
typedef		pair<int, int>			PII; 
typedef		pair<unsigned, unsigned>	PUU; 
typedef		pair<int, POS>			PIP; 
typedef		pair<POS, COLOR>			MOVE; 
typedef		pair<BITB, BITB>			PBB; 
typedef		pair<BITB, COLOR>		PBC; 
typedef		pair<GO, MASK>			PAT; 

typedef		vector<int>				VI; 
typedef		vector<unsigned>		VU; 
typedef		vector<float>			VF; 
typedef		vector<VI>				VVI; 
typedef		vector<VF >				VVF; 
typedef		vector<VVF >			VVVF; 
typedef		vector<POS>				VP; 
typedef		vector<VP>				VVP; 
typedef		vector<BITB>			VB; 
typedef		vector<MOVE>			VM; 
typedef		vector<GO>				VG; 
typedef		vector<COLOR>			VC; 
typedef		vector<string>			VS; 
typedef		vector<KEY>				VK; 
typedef		vector<VK>				VVK; 

typedef		map<POS, int>			MPI; 

/*
const       RGB			_I = 0x0080; 
const       RGB			_B = 0x0010|_I; 
const       RGB			_G = 0x0020|_I; 
const       RGB			_R = 0x0040|_I; 
const       RGB			_W = _R|_G|_B; 
*/
/*
const       RGB			I = 0x0008; 
const       RGB			B = 0x0001|I; 
const       RGB			G = 0x0002|I; 
const       RGB			R = 0x0004|I; 
const       RGB			W = (R|G|B)^I; 
*/

const string R = "\033[1;33;44m";
const string G = "\033[1;32;43m";
const string B = "\033[1;32;44m";
const string W = "\033[0m";

const		int			BS = 19; // max: 25
const		float		KOMI = 7.5; 
const		int			MIDBS = (BS-1)/2; 
const       ROW			LEFTEST = 1<<(BS-1); 
const		ROW			CENTEREST = 1<<(BS-1)/2; 
const		ROW			ROWMASK = (1<<BS)-1; 
const		POS			NULL_POS = make_pair(0, 0); 
const		POS			PASS_POS = make_pair(BS, 0); 
const		int			NULL_TAG = BS*BS; 
const		MOVE		NULL_MOVE = make_pair(NULL_POS, NULL_CLR); 
const		VP			NULL_VP; 
const		int			NOINIT = INT_MIN; 
/*
const		string		GOPATH = "g:\\vc\\go\\"; 
const		string		SGFPATH = "g:\\vc\\go\\sgf\\"; 
const		string		LIBPATH = "g:\\vc\\go\\lib\\"; 
static		char *		CMDPOS = "g:\\vc\\cmd.exe"; 
static		char *		FONTPOS = "g:\\vc\\font\\font.exe"; 
*/
const		string		GOPATH = ""; 
const		string		SGFPATH = "sgf/"; 
const		string		LIBPATH = "lib/"; 
//2009//static		char *		CMDPOS = "cmd.exe"; 
//2009//static		char *		FONTPOS = "font.exe"; 
const		string		CMDPOS = "cmd.exe"; 
const		string		FONTPOS = "font.exe"; 

class setx {
private:
	RGB c; 
public:
	setx(RGB x):c(x){}
	friend ostream& operator<< (ostream& os, setx s); 
}; 

/* _______________________________ 测试区 _______________________________ */

#define MACRO_ALL			1

#define MACRO_INIT			0

#define MACRO_ASSERT		1
#define MACRO_PARASSERT		0
#define MACRO_REPORT		1
#define MACRO_COUT			1
#define MACRO_TIME			1
#define MACRO_CONTROL		1

#define U_MACRO		1
#define IO_MACRO	1
#define ITR_MACRO	1
#define BB_MACRO	1
#define GO_MACRO	1
#define INFO_MACRO	1
#define L_MACRO		1
#define NN_MACRO	1
#define MACRO_T		1
#define TS_MACRO	1
#define TT_MACRO	1
#define BR_MACRO	1
#define F_MACRO		1
#define TG_MACRO	1

/* 验证宏：参数验证和正确性验证 */

void ___assert(bool exp, string s, string _f_ = "", long _l_ = 0); 
void ___parassert(bool exp, string s); 

#if MACRO_ALL && MACRO_ASSERT  
#define	___ASSERT(exp)		___assert(exp, #exp); 
#define	___ASSERT2(exp, code)	___assert(exp, #exp); if (!(exp)) code 
#else
#define ___ASSERT(exp)			{}
#define ___ASSERT2(exp, code)	{}
#endif

#if MACRO_ALL && MACRO_PARASSERT
#define	___PARASSERT(exp)		___parassert(exp, #exp); 
#else
#define ___PARASSERT(exp)		{}
#endif

/* 报告宏：打印宏替换后的执行代码和执行结果 */

#if MACRO_REPORT
#define ___PRINT(code)					{	\
	string s = #code; \
	for (int p = 0; p<s.size (); ++p){			\
		if (s.substr(p, 1) == "; ")				\
			s.replace(p, 1, "; \n"); \
	}										\
	if (s[s.size()-1] == '\n')				\
		s.erase(s.end()-1); \
	cout<<" "<< s << endl; \
}

		// code 之后的语句有时不会到达，所以 code 内不应含 return
		// 宏参数按逗号分割，所以 code 内也不能含非括号内的逗号
#define ___REPORT(code)												\
	cout<<"_________________________________________code:"<< endl; \
	___PRINT(code)													\
	cout<<"_________________________________________"<< endl; \
	code															\
	cout<<"_________________________________________end"<< endl; 
#else
#define ___REPORT(code)	code
#endif

/* 输出宏：输出变量，报告函数内部状态 */

#if MACRO_ALL && MACRO_COUT
		// 宏和模板只是替代，其内出现的类不必声明在前; 
		// 多了参数可以，少了不行
		// 注意宏名括号之前不能有空格
template <class T>
	void ___cout(T t, string s) {
		if (typeid(T) == typeid(int) || 
			typeid(T) == typeid(unsigned)	|| 
			typeid(T) == typeid(float) || 
			typeid(T) == typeid(double) || 
			typeid(T) == typeid(ULL) || 
			typeid(T) == typeid(POS) ||
			typeid(T) == typeid(PUU) ||
			typeid(T) == typeid(PII))
			cout<<std::setw(20)<<std::left<< setx(G) << ( s + ": ")
				<<setx(W)<< t << endl; 
		else
			cout<<setx(G)<< s <<": "<< endl
				<<setx(W)<< t << endl; 
	}

#define ___COUT1(var)		___cout(var, #var); 
#define ___COUT2(a, b)		{___COUT1(a)		___COUT1(b)}
#define ___COUT3(a, b, c)		{___COUT2(a, b)		___COUT1(c)}
#define ___COUT4(a, b, c, d)	{___COUT3(a, b, c)	___COUT1(d)}
#define ___COUT5(a, b, c, d, e)	{___COUT4(a, b, c, d)	___COUT1(e)}
#define ___COUT6(a, b, c, d, e, f)			{___COUT5(a, b, c, d, e)		\
										___COUT1(f)}
#define ___COUT7(a, b, c, d, e, f, g)			{___COUT5(a, b, c, d, e)		\
										___COUT2(f, g)}
#define ___COUT8(a, b, c, d, e, f, g, h)		{___COUT5(a, b, c, d, e)		\
										___COUT3(f, g, h)}
#define ___COUT9(a, b, c, d, e, f, g, h, i)		{___COUT5(a, b, c, d, e)		\
										___COUT4(f, g, h, i)}
#define ___COUT10(a, b, c, d, e, f, g, h, i, j)	{___COUT5(a, b, c, d, e)		\
										___COUT5(f, g, h, i, j)}
#else
#define ___COUT1(var)					{}
#define ___COUT2(a, b)					{}
#define ___COUT3(a, b, c)					{}
#define ___COUT4(a, b, c, d)				{}
#define ___COUT5(a, b, c, d, e)				{}
#define ___COUT6(a, b, c, d, e, f)			{}
#define ___COUT7(a, b, c, d, e, f, g)			{}
#define ___COUT8(a, b, c, d, e, f, g, h)		{}
#define ___COUT9(a, b, c, d, e, f, g, h, i)		{}
#define ___COUT10(a, b, c, d, e, f, g, h, i, j)	{}
#endif

/* 计时宏 */

#if MACRO_ALL && MACRO_TIME
#define ___TIME(id, code)	{}
#else
#define ___TIME(id, code)	{}
#endif

/* 控制宏 */

#if MACRO_ALL && MACRO_CONTROL
//#include "conio.h"	// _kbhit() _getch()						// VS2008
//#define ___ESC	if ( _kbhit() && ( _getch() == 0x1b ) )	break; 
#define ___ESC	{}
#else 
#define ___ESC	{}
#endif

/* 时间类 */

class TIMER{							
private:
	unsigned	ticat; 
	int			dif; 
public:
	TIMER():ticat(clock()), dif(0){ }	
	void	tic(string s = "", int n = 1); 
	void	toc(); 
	void	set_(); 
	void	silenttic_(); 
	void	silenttoc_(); 
}; 

/* 随机器类 */

class RANDER {
public:
	VI			vi; 
	VU			vu; 
	VP			vp; 
	VS			vs; 
	vector<DIRECTION>	vd; 
	vector<COLOR>		vc; 
	vector<ACTION>		va; 
	VB			vb; 
	VG			vg; 
public:
	RANDER(int n = 6, int nb = 4, int ng = 1); 
}; 

/* _______________________________ 非测试区 _______________________________ */


/* 初始化类*/

class INIT {
private:
	VP			i2p; 
	VVP			i2near4; 
	MPI			p2i; 
	map<POS, VP> p2near4; 

	VVI			id8; 
	VVI			q8; 
	VB			josekimask; 
	VU			xrand; 
	VU			orand; 
	map<KEY, MOVE> key2rand; 
	VB			route; 

public:
    bool		inited; 

public:
	INIT(); 

	void	seti2p(); 
	void	seti2near4(); 
	void	setp2i(); 
	void	setp2near4(); 

	POS		getp(int i) const; 
	VP		getnear4(int i)const; 
	int		geti(const POS& p); 
	VP&		getnear4(const POS& p); 

	void	setzobristmask(int width = MIDBS-1); 
	void	set_xorand_key2rand(); 
	void	set_id8_q8(); 

	void	swap_id8_q8(int i, int j); 
	void	swap_xorand(); 

	BITB	getjosekimask(int i) const; 
	KEY		getrand(const POS& pos, COLOR clr); 
	MOVE&	getmove(KEY key); 
	int		getq8(int i) const; 

	void	setroute(); 
	BITB	getroute(ROUTE r); 

	bool	load_xorand_key2rand(string filename); 
	bool	save_xorand(string filename); 
}; 

/* 组合类 */

#include "stdarg.h"		// va_list va_start() va_arg() va_end()		// VS2008


template <typename T, typename S>
class BIND {
public:
	vector<T> vt; 
	vector<T>& operator()(int numb, ...){
		va_list ptr; 
		va_start(ptr, numb); 
		while(numb--){
			S t = va_arg(ptr, S); 
			vt.push_back(static_cast<T>(t)); 
		}
		va_end(ptr); 
		return vt; 
	}
}; 

/* 全局变量 */


extern TIMER		___, gametimer; 
extern INIT			g_init; 
extern LEARN		g_learn; 

#endif


