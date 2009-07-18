#ifndef _UTILITY
#define _UTILITY

#include "define.h"

// rowexpand()也可以用查表法：
// 一个二维4位量上下接触表（返回4位），
// 一个二维4位量左右接触表（返回8位）！
// 针对 BS 位前为0，可以减少某些步骤。
// 慢？！还是要以4位慢慢膨胀！
/*extern*/ /*inline*/ unsigned	rowexpand(unsigned background, unsigned seedrow); 
/*extern*/ /*inline*/ unsigned	connectingone(unsigned x, int n); 
/*extern*/ /*inline*/ unsigned	reverse(unsigned x); 
/*extern*/ /*inline*/ int		popu(unsigned x); 
/*extern*/ /*inline*/ int		nleadingzero(unsigned x); 
/*extern*/ /*inline*/ int		ntailzero(unsigned x); 
/*extern*/ /*inline*/ unsigned	mulhigh32(unsigned u, unsigned v); 
/*extern*/ /*inline*/ bool		muloverflow(unsigned x, unsigned y); 

/*extern*/ /*inline*/ bool		apos_(const POS&); 
/*extern*/ /*inline*/ POS		_xy2pos(int x, int y); 
/*extern*/ /*inline*/ PII		_pos2xy(const POS&); 
/*extern*/ /*inline*/ int		xy2i_(int x, int y); 
/*extern*/ /*inline*/ PII		i2xy_(int); 
/*extern*/ /*inline*/ POS		i2pos(int); 
/*extern*/ /*inline*/ int		pos2i(const POS&); 
/*extern*/ /*inline*/ POS		aa2pos(string); 
/*extern*/ /*inline*/ string	pos2aa(const POS&);
/*extern*/ /*inline*/ POS		ai2pos(string); 

/*extern*/ /*inline*/ POS		corner(DIRECTION d); 
/*extern*/ /*inline*/ POS		star(DIRECTION d); 
/*extern*/ /*inline*/ POS		star(int); 
/*extern*/ /*inline*/ VP		near4(const POS&); 
/*extern*/ /*inline*/ VP		near8(const POS&); 

/*extern*/ /*inline*/ bool		outside(const POS&); 
/*extern*/ /*inline*/ bool		onboarder(const POS&); 
/*extern*/ /*inline*/ POS		rot(int angle, const POS&); //clockwise
/*extern*/ /*inline*/ POS		transp(const POS&); 
/*extern*/ /*inline*/ int		interval_(const POS&, const POS&); 
/*extern*/ /*inline*/ VVI		index8(int a, int b); 

/*extern*/ /*inline*/ BITB		cross(const POS& center, int radius); 
/*extern*/ /*inline*/ BITB		square(const POS& center, int radius); 
/*extern*/ /*inline*/ BITB		frame(const POS& center, int radius); 
/*extern*/ /*inline*/ BITB		strip(const POS& onecorner, const POS& othercorner); 
/*extern*/ /*inline*/ BITB		strip(const POS& pos, 
							  int horizontal, int vertical, DIRECTION d = CENTER); 
/*extern*/ /*inline*/ BITB		strip(const POS& pos, 
							  int up, int left, int down, int right); 
/*extern*/ /*inline*/ BITB		diamond(const POS& center, int radius); 
/*extern*/ /*inline*/ BITB		pie(const POS& center, int radius); 
/*extern*/ /*inline*/ BITB		circle(const POS& center, int radius); 
/*extern*/ /*inline*/ BITB		line(POS, POS); 

// /*extern*/ /*inline*/ bool		issamepat(string a, string b); 
// /*extern*/ /*inline*/ string	asciipat(string patstr); 
// /*extern*/ /*inline*/ string	asciigrid(const POS& pos); 
// /*extern*/ /*inline*/ string	pat2ascii(const BITB& bb, const BITB& mask); 
// /*extern*/ /*inline*/ string	pat2ascii(const BITB& xbb, const BITB& obb, const BITB& mask); 

/*extern*/ /*inline*/ bool		prob(int small, int large); 
/*extern*/ /*inline*/ int		randsqrt(int m); 
/*extern*/ /*inline*/ int		randlog_(int m); 
/*extern*/ /*inline*/ unsigned	randu(int m); 
/*extern*/ /*inline*/ float		randf(); 
/*extern*/ /*inline*/ VF		randvf(int m); 
/*extern*/ /*inline*/ VVF		randvvf(int m, int n); 
/*extern*/ /*inline*/ VVVF		randvvvf_(int m, int n, int l); 

UL		rand32(UL& Seed, UL Multiplier = 16807); 
UL		longmulmod(UL Multiplier, UL Multiplicand, UL Divisor); 

COLOR	operator ~ (const COLOR c); 

/*extern*/ /*inline*/ int	str2i(string s, int minv, int maxv); 
VS	 getline(); 
/*extern*/ /*inline*/ VI	indexsort(const VI& vi);

#endif



