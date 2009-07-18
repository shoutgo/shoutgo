#include "inout.h" 
#include "stdio.h"
#include <unistd.h>	// GetStdHandle() SetConsoleTextAttribute()	// VS2008
#include <sys/stat.h>
#include <sys/types.h>

#if		IO_MACRO == 0
#undef	___TIME
#define ___TIME(code)	{}
#endif

// ┠┷┯┨┿┼╋╬┳┻┫┣
// ●○⊙■□▲△▼▽◆◇★☆◎☉
// ⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽⑾⑿⒀⒁⒂⒃⒄⒅⒆⒇
// ⒈⒉⒊⒋⒌⒍⒎⒏⒐⒑⒒⒓⒔⒕⒖⒗⒘⒙⒚⒛
// ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ
// ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ
// ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdefghijklmnoprrstuvwxyz
// ①②③④⑤⑥⑦⑧⑨⑩

// 上为已采用下为备用
// ０１２３４５６７８９
// ??????????????????????????
// ??????????
// ??????????
// ??????????
// ①②③④⑤⑥⑦⑧⑨⑩??????????
// ??????????????????????????
// ??????????????????????????

INOUT::INOUT(bool b):axisshow(b){
}

string     INOUT::logo_() {	
	static string s = ""; 
		s += " -+    -+    ++ \n"; 
		s += "   ●┯┯┯○   \n"; 
		s += "   ┠③②①┨   \n"; 
		s += " --┠⑥⑤④┨++ \n"; 
		s += "   ┠⑨⑧⑦┨   \n"; 
		s += "   ○┷┷┷●   \n"; 
		s += " --    +-    +- \n"; 
	return s; 
}

string   INOUT::grid(const POS& pos) {	
	int		 r = pos.first; 
	ROW		 p = pos.second; 
	if (r == 0 && p == LEFTEST)		{return "┏"; }
	if (r == 0 && p == 1)			{return "┓"; }
	if (r == BS-1 && p == 1)		{return "┛"; }
	if (r == BS-1 && p == LEFTEST)	{return "┗"; }
	if (r == 0)						{return "┯"; }
	if (p == 1)						{return "┨"; }
	if (r == (BS-1))				{return "┷"; }
	if (p == LEFTEST)				{return "┠"; }
	if ((BS >= 13) &&
		((r == 3) || (r == MIDBS) || (r == (BS-1-3)) ) &&
		((p == (1<<3)) || (p == (CENTEREST)) || (p == (1<<(BS-1-3)))))
									{return "╋"; }
	if (r == MIDBS && p == CENTEREST )
									{return "╋"; }
	else 							{return "┼"; }
}

POS		 INOUT::in2pos(const POS& pos, string in) {	

	POS p = ai2pos(in); 
	if (p != NULL_POS)
		return p; //-// 2007.2.11

	string op; 
	if (isdigit(in[0])&&(in.size() == 1))   
		in += "++00"; 
	if ((in.find("++") == in.npos) && 
		(in.find("-+") == in.npos) && 
		(in.find("--") == in.npos) && 
		(in.find("+-") == in.npos)) 
		return NULL_POS; 
	if (!isdigit(in[0]))       
		in.insert(0, "0"); 
	op = in.substr(1, 2); 
	switch (in.size()) {
		case 5:
			if (!isdigit(in[3]) || !isdigit(in[4]))			
				return NULL_POS; 
			break; 
		case 3:
			in += "11"; 
			break; 
		case 4:
			if (!isdigit(in[3]))    	
				return NULL_POS; 
			if (op == "++")	in += "0"; 
			if (op == "-+")	in.insert(3, "0"); 
			if (op == "--")	in += "0"; 
			if (op == "+-")	in.insert(3, "0"); 
			break; 
	}
	if (in.size()>5)    
		return NULL_POS; 
	if ((in[0]-'0') == 0) 
		return jj2pos(in, pos); 
	else 
		return jj2pos(in); 
}

POS		 INOUT::jj2pos(string in, const POS& pp){	
	string op; 
	int    n0, n3, n4; 
	n0 = in[0]-'0'; n3 = in[3]-'0'; n4 = in[4]-'0'; 
	op = in.substr(1, 2); 
	POS pos; 
	if (n0 == 0) pos = pp; else pos = star(n0); 
	if      (op == "++")	{pos.first -= n4; pos.second >>= n3; } 
	else if (op == "-+")	{pos.first -= n4; pos.second <<= n3; }
	else if (op == "--")	{pos.first += n4; pos.second <<= n3; } 
	else if (op == "+-")	{pos.first += n4; pos.second >>= n3; } 
    else 			  
		return NULL_POS; 
	if (outside(pos))  
		return NULL_POS; 
	return pos; 
}

string INOUT::axis(const POS& pos) const {
	if (!axisshow)
		return "  "; 
	//static string h = "ＺＹＸＷＶＵＴＳＲＱＰＯＮＭＬＫＪＩＨＧＦＥＤＣＢＡ";
	static string h = " Z Y X W V U T S R Q P O N M L K J I H G F E D C B A"; 
	static string v = " 1 2 3 4 5 6 7 8 91011121314151617181920212223242526";
	static string blanks = "";
	if (pos == corner(WN))
		return blanks + "  " + h.substr ( h.size()-BS*2,BS*2) + "  "+ "\n" 
			 + blanks + v.substr (pos.first*2, 2); 
	else if (pos == corner(ES))
		return v.substr (pos.first*2, 2) + "\n" 
			 + blanks + "  " + h.substr ( h.size()-BS*2, BS*2) + "  "; 
	else if (pos.second & LEFTEST)
		return blanks + v.substr (pos.first*2, 2); 
	else if (pos.second & 1)
		return v.substr (pos.first*2, 2); 
}

void	INOUT::print(const GO& go, const BITB& mask) {	
	string s; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		if (pos.second & LEFTEST && mask.r[pos.first])
			cout << axis(pos); 
		if ( mask[pos] ){ 
			switch (go[pos]) {
				case BLACK:				
					s = "●"; break; 
				case WHITE:				
					s = "○"; break; 
				case BLACK_WHITE:	   	
					s = "⊙"; break; 
				case HOTKO:			    
					s = grid(pos); break; 
				case EMPTY:			    
					s = grid(pos); break; 
			}
			if (pos == go.getlastpos())
				cout << setx(R) << s << setx(W); 
			else if (pos == go.gethotko ())
				cout << setx(B) << s << setx(W); 
			else if (go.getkill ()[pos])
				cout << setx(G) << s << setx(W); 
			else
				cout << s; 
		}
		if (pos.second == 1 && mask.r[pos.first])             
			cout << axis(pos) << endl; 
	}
}

void	INOUT::print(const GO& go, const GO& markgo, RGB xxc, RGB ooc, RGB xoc) {	
	string s; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		if (pos.second & LEFTEST)
			cout << axis(pos); 
		switch (go[pos]) {
			case BLACK:				
				s = "●"; break; 
			case WHITE:				
				s = "○"; break; 
			case BLACK_WHITE:	   	
				s = "⊙"; break; 
			case HOTKO:			    
				s = grid(pos); break; 
			case EMPTY:			    
				s = grid(pos); break; 
		}
		if (pos == go.getlastpos())
			cout << setx(R) << s << setx(W); 
		else if (pos == go.gethotko ())
			cout << setx(B) << s << setx(W); 
		else if (go.getkill ()[pos])
			cout << setx(G) << s << setx(W); 
		else if (markgo[pos] == BLACK)
			cout << setx(xxc) << s << setx(W); 
		else if (markgo[pos] == WHITE)
			cout << setx(ooc) << s << setx(W); 
		else if (markgo[pos] == BLACK_WHITE)
			cout << setx(xoc) << s << setx(W); 
		else
			cout << s; 

		if (pos.second == 1)             
			cout << axis(pos) << endl; 
	}
}
/*
string	INOUT::axis (string board) {
	___ASSERT(board.size () == (2*BS*BS+BS)); 
	string v = " 1 2 3 4 5 6 7 8 91011121314151617181920212223242526"; 
	string h = " A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"; 
	board = v.substr (0, 2) + board; 
	for (int p = 0, i = 0; p<board.size (); ++p)
		if (board.substr (p, 1) == "\n"){
			board.replace(p, 1, v.substr(i, 2)+"\n"+v.substr(i+2, 2)); 
			i += 2; 
			p += 3; 
		}
	board.replace (p-2, 2, ""); 
	board = "  " + h.substr (0, BS*2) +"  \n" 
			+ board + "  " + h.substr (0, BS*2) +"  \n"; 
	return board; 
}

string   INOUT::print__(const GO& go, const BITB& bb, 
					  const POS& pp, MODE_PRINT mode) {	
	for (int i = 0; i<BS; ++i) 
		___ASSERT((go.xx.r [i]| go.oo.r [i]) <= ROWMASK); 
	string s = ""; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		if (bb[pos]){
			if (mode == BITB_P)				
				s += "☉"; 
			if (mode == ONEAREA_P && (go[pos] == EMPTY)) 
				s += "☉"; 
			if (mode == BLOCK_P||mode == BOARD_P||mode == ONEAREA_P) {		
				if(go[pos] == BLACK)			 
					s += "◆"; 
			    if(go[pos] == WHITE)          
					s += "◇"; 
			    if(go[pos] == BLACK_WHITE)     
					s += "⊙"; 
			    if(go[pos] == HOTKO)             
					s += "╬"; 
			}
			if (pos.second == 1)
				s += "\n"; 
			continue; 
		}
		switch (go[pos]) {
			case BLACK:	
				if (pos == pp) {
					s += "◆"; 
					break; 
				}
				if (pos == go.getlastpos() ) 
					s += "■"; 
				else                 
					s += "●"; 
				break; 
			case WHITE:	
				if (pos == pp) 	{
					s += "◇"; 
					break; 
				}
				if (pos == go.getlastpos() ) 
					s += "□"; 
				else		         
					s += "○"; 
				break; 
			case BLACK_WHITE:	    
				s += "⊙"; 
				break; 
			case HOTKO:		
				s += "╬"; 
				break; 
			case EMPTY:				 
				s += grid(pos); 
				break; 
		}
		if (pos.second == 1)         
			s += "\n"; 
	}
	//cout<<(s += "\n"); 
	return s; 
}

string   INOUT::printbitboard(const BITB& bb) {	
	string s; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		switch (bb[pos]) {
			case 1:	            
				s += "◎"; 
				break; 
			case 0: 			
				s += grid(pos); 
				break; 
		}
		if (pos.second == 1)     
			s += "\n"; 
	}
	cout<<(s += "\n"); 
	return s; 
}
*/
/*
string	 INOUT::printlabel ( const GO& go, 
							const pair<VB, VB >& pvv, MODE_PRINTLABEL mode) {	
	string black = "ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ"; 
	string white = "ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ"; 
	string blk = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	string wht = "abcdefghijklmnopqrstuvwxyz"; 
	string s = ""; 
	VB vx = pvv.first, vo = pvv.second; 
	BITB xlabel, olabel, publiclabel; 
	for (int i = 0; i<vx.size (); ++i)	xlabel |= vx[i]; 
	for (int i = 0; i<vo.size (); ++i)	olabel |= vo[i]; 
	publiclabel = xlabel & olabel; 
	publiclabel ^= publiclabel & (go.xx | go.oo); 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		bool labelfound = 0; 
		// 不加 break 以观察各点的归属是否唯一
		for (int i = 0; i<vx.size (); ++i)
			if ((mode == L_CLUSTER && vx[i][pos]) ||
				(mode == L_AREA1 && vx[i][pos] 
				&& go[pos] != BLACK && go[pos] != WHITE)){
					if (publiclabel[pos])   
						s += blk[i]; 
					else	            	
						s += black.substr(2*i, 2); 
				labelfound = 1; 
				// break; 
			}
			else if (mode == L_AREA2 && vx[i][pos]&&go[pos] != WHITE){
				if (go[pos] == BLACK) s += black.substr(2*i, 2); 
				else 
					if (publiclabel[pos]) 
						s += blk[i]; 
					else 
						s = s+blk[i]+"."; 
				labelfound = 1; 
				// break; 
			}
		for (int i = 0; i<vo.size (); ++i)
			if ((mode == L_CLUSTER && vo[i][pos]) 
				||								   
				(mode == L_AREA1 && vo[i][pos]
				&& go[pos] != BLACK && go[pos] != WHITE)){ 
					if (publiclabel[pos])   
						s += wht[i]; 
					else	            	
						s += white.substr(2*i, 2); 
				labelfound = 1; 
				// break; 
			}
			else if (mode == L_AREA2 && vo[i][pos]&&go[pos] != BLACK){
				if (go[pos] == WHITE) 
					s += white.substr(2*i, 2); 
				else 
					if (publiclabel[pos]) 
						s += wht[i]; 
					else 
						s = s+"."+wht[i]; 
				labelfound = 1; 
				// break; 
			}	
		if (!labelfound)
			switch (go[pos]) {
				case BLACK:			s += "●"; break; 
				case WHITE:			s += "○"; break; 
				case BLACK_WHITE:	s += "⊙"; break; 
				case HOTKO:			s += "╬"; break; 
				case EMPTY:			s += grid(pos); break; 
			}														
		if (pos.second == 1)			
			s += "\n"; 
	}
	cout<<(s += "\n"); 
	return s; 
}

string	 INOUT::printcluster ( const GO& go, const pair<VB, VB >& pvv) {	
	return printlabel(go, pvv, L_CLUSTER); 
}

string	 INOUT::printarea ( const GO& go, const pair<VB, VB >& pvv) {	
	return printlabel(go, pvv, L_AREA1); // mode = L_AREA1 or L_AREA2
}

string	 INOUT::printonearea ( const GO& go, const BITB& bb) {	
	return print__(go, bb, NULL_POS, ONEAREA_P); 
}

string	 INOUT::printpublicarea ( const GO& go, const BITB& bb) {	
	return print__(go, bb, NULL_POS, BITB_P); 
}

string   INOUT::printblock(const GO& go, const BITB& blk, const POS& pos) {	
	return	print__(go, blk, pos, BLOCK_P); 
}

string   INOUT::printlibertysite(const GO& go, 
								 const BITB& libsite, const POS& pos) {	
	return	print__(go, libsite, pos, BITB_P); 
}

string   INOUT::printpartition(const VG& vg){	
	string black = "●⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽⑾⑿⒀⒁⒂⒃⒄⒅⒆⒇"; 
	string white = "○⒈⒉⒊⒋⒌⒍⒎⒏⒐⒑⒒⒓⒔⒕⒖⒗⒘⒙⒚⒛"; 
	string s = ""; 
	int find; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		find = 0; 
		for (int i = 0; i<(int)(vg.size ()); ++i){
			switch (vg[i][pos]) {
				case BLACK:	 
					{++find; 
					s += (black.substr(2*i, 2)); 
					break; }
				case WHITE:	          
					{++find; 
					s += (white.substr(2*i, 2)); 
					break; }
				case BLACK_WHITE:	  
					{++find; 
					s += "⊙"; 
					break; }
				case HOTKO:			  
					{++find; 
					s += "╬"; 
					break; }
			}															  
		if (find>0)  
			break; 
		}
		if (find == 0)                     
			s += grid(pos); 
		if (pos.second == 1)                
			s += "\n"; 
	}
	cout<<s; 
	return s; 
}

string   INOUT::printliberty(const GO& go, const VI& vec){		
	
	//-// 20个字符不够用, 可能导致运行时错误, 待改进
	string black = "◎⑴⑵⑶⑷⑸⑹⑺⑻⑼⑽⑾⑿⒀⒁⒂⒃⒄⒅⒆⒇"; 
	string white = "☉⒈⒉⒊⒋⒌⒍⒎⒏⒐⒑⒒⒓⒔⒕⒖⒗⒘⒙⒚⒛"; 
	string s = ""; 
	int i; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		i = pos2i(pos); 
		if (vec[i] == 0) {
			s += grid(pos); 
			if (i%BS == 0)   
				s += "\n"; 
			continue; 
		}											
		if (go[pos] == BLACK)
			s += black.substr (2*vec[i], 2); 
		else 
			s += white.substr (2*vec[i], 2); 
		if (i%BS == 0)      
			s += "\n"; 
	}															
	cout<<(s += "\n"); 
	return s; 
}
*/
/*
// 此 mask 可以呈不规则形状
string	INOUT::printpat(const GO& old, const BITB& mask){	
	string s = ""; 
	GO go = GO(old.xx & mask, old.oo & mask); 
	BITB squarem = mask.project (); 
	___COUT1(squarem); 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		if (squarem[pos]){
			switch (go[pos]) {
				case BLACK:				
					//if (mask[pos]) 
						s += "●"; 
					//else 
					//	s += "  "; 
					break; 
				case WHITE:				
					//if (mask[pos])
						s += "○"; 
					//else 
					//	s += "  "; 
					break; 
				case BLACK_WHITE:	   	
					//if (mask[pos])
						s += "⊙"; 
					//else 
					//	s += "  "; 
					break; 
				case HOTKO:			    
					//if (mask[pos]) 
						s += grid(pos); 
					//else 
					//	s += "  "; 
					break; 
				case EMPTY:			    
					//if (mask[pos]) 
						s += grid(pos); 
					//else 
					//	s += "  "; 
					break; 
			}
		}
		if (pos.second == 1 && squarem.r[pos.first])             
			s += "\n"; 
	}
	for (int pp = s.find ("╋"); pp != s.npos; pp = s.find ("╋"))
        	s.replace(pp, 2, "┼"); 
	cout<<(s += "\n"); 
	return s; 
}

// 此 mask 可以呈不规则形状
string	INOUT::printpat(const BITB& old, const BITB& mask) {	
	string s; 
	BITB bb = old&mask; 
	BITB squarem = mask.project (); 
	___COUT1(squarem); 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		if (squarem[pos]){
			switch (bb[pos]) {
				case 1:	           
					//if (mask[pos]) 
						s += "◎"; 
					//else 
					//	s += "  "; 
					break; 
				case 0: 			
					//if (mask[pos])
						s += grid(pos); 
					//else 
					//	s += "  "; 
					break; 
			}
		}
		if (pos.second == 1 && squarem.r[pos.first])   
			s += "\n"; 
	}
	for (int pp = s.find ("╋"); pp != s.npos; pp = s.find ("╋"))
        	s.replace(pp, 2, "┼"); 
	cout<<(s += "\n"); 
	return s; 
}
*/

// 与pat2ascii()并非对应关系.模式"xx\n-x\n"中的-是没有意义的, 
// 但它可以由pat2ascii()生成.
// 如果in2pat()同时给出一个对应 mask 的 string 则可以解决.
// 输入是否合法在inputpat()里检查, 默认长宽不超过(BS-1)/2
// 可加入 X O 表示边上的子, 待完成
PAT	INOUT::in2pat(string s){	
	int w; 
	while ((w = s.find_first_of ("\t ")) != s.npos )
		s.replace(w, 1, ""); 
	bool up = 0, left = 0, down = 0, right = 0; 
	string smn[BS][BS]; 
	int  m = 0, n = 0, p; 
	for (p = 0; p<s.size(); ++p){
		if (s.substr (p, 1) != "\n") 			
			smn[m][n++] = s[p]; 
		else {
			n = 0; 
			++m; 
		}
	}
	n = p/m - 1; //-// 取模？			
	for (p = 1; p<m-1; ++p){
		if (smn[p][0] == "-")
			left = 1; 
		if (smn[p][n-1] == "-")
			right = 1; 
	}
	if (smn[0][0] == "-" && smn[m-1][0] == "-" && m != 1)
		left = 1; 
	if (smn[0][n-1] == "-" && smn[m-1][n-1] == "-" && m != 1)
		right = 1; 
	for (p = 1; p<n-1; ++p){
		if (smn[0][p] == "-")
			up = 1; 
		if (smn[m-1][p] == "-")
			down = 1; 
	}	
	if (smn[0][0] == "-" && smn[0][n-1] == "-" && n != 1)
		up = 1; 
	if (smn[m-1][0] == "-" && smn[m-1][n-1] == "-" && n != 1)
		down = 1; 
	int i0 = MIDBS, j0 = MIDBS; 

	// + 优先  - 次之 . 更次之  
	// 不在考虑位置的 + - 与 . 无异
	if		((smn[0][0] == "+")	|| (up && left))	{ 
		i0 = 0; 
		j0 = BS-1; 
	}
	else if ((smn[m-1][0] == "+")	|| (down && left))	{ 
		i0 = BS-m; 
		j0 = BS-1; 
	}
	else if ((smn[m-1][n-1] == "+")||(down && right))	{ 
		i0 = BS-m; 
		j0 = n-1; 
	}
	else if ((smn[0][n-1] == "+")	|| (up && right))	{ 
		i0 = 0; 
		j0 = n-1; 
	}
	else if (up)								
		i0 = 0; 
	else if (left)											
		j0 = BS-1; 
	else if (down)									
		i0 = BS-m; 
	else if (right)										
		j0 = n-1; 
	GO go; 
	BITB mask = strip(make_pair(i0, 1<<j0), n-1, m-1, ES); 
	for (int i = 0; i<m; ++i)
		//不认识的符号一律作空白点
		for (int j = 0; j<n; ++j){			
			if		(smn[i][j] == "x")		{	
				go.xx.r[i+i0] |= (1<<(j0-j)); 
			}
			else if (smn[i][j] == "o")		{	
				go.oo.r[i+i0] |= (1<<(j0-j)); 
			}		
		}

	return make_pair(go, mask); //未经 project()
}

//直接用缓冲区操作比frommatlab()中那种按字读入的方式快得多。
string	INOUT::readbuf(string filename){	
	ifstream file(filename.c_str()); 
	if (!file) {
		cerr<< filename << " : open fail !"<< endl; 
		return ""; 
	}
	ostringstream ostr; 
	ostr<<file.rdbuf (); 
	string s = ostr.str (); 
	return s; 
}

/*
VP	INOUT::sgf2pos(string filename) {	
	vector<PII >  vecpii = sgf2xy(filename); 
    VP  vech; 
	POS pos; 
	for (int i = 0; i<vecpii.size (); ++i){
		pos = _xy2pos(vecpii[i].first, vecpii[i].second ); 
		if (outside(pos)){
			cerr << filename <<" is a bad sgf file!"<< endl; 
			return NULL_VP; 
		}
		vech.push_back (pos); 
	}
	return vech; 
}

vector<PII >	INOUT::sgf2xy(string filename) {	
	string s = readbuf(filename); 
	vector<PII >  vechistory; 
	int p = 0; 
	while((p = s.find ("; B[", p)) != s.npos) {
		vechistory.push_back (make_pair(s[p+3]-'a', (s[p+4]-'a'))); 
		if((p = s.find ("; W[", p)) != s.npos){
			vechistory.push_back (make_pair(s[p+3]-'a', (s[p+4]-'a'))); 
		}
	}
	return vechistory; 
}

VVP			INOUT::sgfs2pos(const VS& vecf){	
	VVP  vech; 
	VP vp; 
	for (int i = 0; i<vecf.size (); ++i){
		vp = sgf2pos(vecf[i]); 
		if (vp.empty()){
			cerr << vecf[i] << " is a bad sgf file ! continue ..." << endl; 
			continue; 
		}
		vech.push_back (vp); 
	}
	return vech; 
}

vector<vector<PII> > INOUT::sgfs2xy(const VS& vecf){	
	vector<vector<PII > >  vech; 
	for (int i = 0; i<vecf.size (); ++i)
		vech.push_back (sgf2xy(vecf[i])); 
	return vech; 
}
*/

// 小括号（）之内是一个全局分支，可嵌套
// 中括号［］之内是实际内容，不会有嵌套
// 此处的结点不是全局分支，仅仅是一个走步
SGFROOT* INOUT::sgf2tree(string filename){	
	string s = readbuf(filename.c_str ()); 
	int	unmatch = 0; // 未匹配的左小括号（
	int	m = 0; // 未匹配的左中括号［
	int	expand[1024] = {0}; // 从相应的未匹配的（处展开的节点层数，用以回溯
	int	p; // 在 s 上的工作位置

	/* 生成根结点 */

	SGFROOT* root = new SGFROOT; 
	//后面的算法正确运行需先替换掉空格换行符等
	while ((p = s.find_first_of ("\t\n\r ")) != s.npos )
		s.replace(p, 1, ""); 
	root->intro = s.substr (0, 0); 
	// 排局
	if ((p = s.find("AB[")) != s.npos)
		for(p += 2; s.substr(p, 1) == "["; p += 4) 
				//root->xfuseki.push_back(_xy2pos(s[p+1]-'a', (s[p+2]-'a'))); 
				root->xfuseki.push_back(aa2pos(s.substr(p+1,2))); 
	if ((p = s.find("AW[")) != s.npos)
		for(p += 2; s.substr(p, 1) == "["; p += 4) 
			if (s.substr(p, 1) == "[") 
				//root->ofuseki.push_back(_xy2pos(s[p+1]-'a', (s[p+2]-'a'))); 
				root->ofuseki.push_back(aa2pos(s.substr(p+1,2))); 
	// 下一手该谁走
	if ((p = s.find("PL[")) != s.npos)
		root->turn = ((s.substr (p+3, 1) == "B")?BLACK:WHITE); 
	else root->turn = BLACK; 

	/* 首次为伪根结点，将来会被 root 淹没；作用为工作结点 */

	// 当前结点 cnode ，赋初值，为根结点的本色
	SGFNODE* cnode = new SGFNODE; 
	cnode->color = ~(root->turn); 
	// 按字符推进
	for (p = s.find("("); p<s.size (); ++p){
		// 普通字符均跳过
		// 遇到［ 时，到下一个］之间的内容一定已处理，跳过
		if (s.substr(p, 1) == "[")
			++m; 
		else if (s.substr(p, 1) == "]")
			--m; 
		if (m != 0) 
			continue; 
		// 一个分支开始，记录分支开始时的分支层数
		if (s.substr(p, 1) == "("){										
			++unmatch; 
			expand[unmatch] = 0; 
		}
		// 遇到；号，向前看3个字符
		else if (s.substr(p, 3) == ";B[" || s.substr(p, 3) == ";W["){						
			++expand[unmatch]; 
		}
		// 遇到 B 
		else if (s.substr(p-1, 3) == ";B["){		
			// 走子方有跳跃，加入PASS
			if (cnode->color == BLACK){
				SGFNODE* newnode = new SGFNODE; 
				cnode->sons.push_back(newnode); 
				newnode->father = cnode; 
				newnode->pos = PASS_POS; 
				newnode->color = WHITE; 
				cnode = newnode; 
				++expand[unmatch]; 
			}
			// 正常生成一个结点
			SGFNODE* newnode = new SGFNODE; 
			cnode->sons.push_back(newnode); 
			newnode->father = cnode; 
			//newnode->pos = _xy2pos(s[p+2]-'a', (s[p+3]-'a') ); 
			newnode->pos = aa2pos(s.substr(p+2,2)); 
			newnode->color = BLACK; 
			cnode = newnode; 
		}
		// 遇到 W
		else if (s.substr(p-1, 3) == ";W["){										
			// 走子方有跳跃，加入PASS
			if (cnode->color == WHITE){
				SGFNODE* newnode = new SGFNODE; 
				cnode->sons.push_back(newnode); 
				newnode->father = cnode; 
				newnode->pos = PASS_POS; 
				newnode->color = BLACK; 
				cnode = newnode; 
				++expand[unmatch]; 
			}
			// 正常生成一个结点
			SGFNODE* newnode = new SGFNODE; 
			cnode->sons.push_back(newnode); 
			newnode->father = cnode; 
			//newnode->pos = _xy2pos(s[p+2]-'a', (s[p+3]-'a') ); 
			newnode->pos = aa2pos(s.substr(p+2,2)); 
			newnode->color = WHITE; 
			cnode = newnode; 
		}
		// 一个分支结束，退回到合适层数
		else if (s.substr(p, 1) == ")"){							
			for(int i = 0; i<expand[unmatch]; ++i)
				cnode = cnode->father; 
			--unmatch; 
		}															
	}														

	/* 真实的根结点 */

	root->sons = cnode->sons; 
	for(int i = 0; i< root->sons.size(); ++i)
		root->sons[i]->father = 0; 
	
	/* 验证 */

	___ASSERT(unmatch == 0); 
	if (root->sons .size ()){
		___ASSERT(root->turn == root->sons[0]->color); 
	}

	return root; 
}

VM INOUT::sgf2move(string filename) {
	SGFROOT* root = sgf2tree(filename); 
	VM vecm; 
	for (int i = 0; i<root->xfuseki.size (); ++i)
		vecm.push_back (make_pair(root->xfuseki [i], BLACK)); 
	for (int i = 0; i<root->ofuseki.size (); ++i)
		vecm.push_back (make_pair(root->ofuseki [i], WHITE)); 
	if (root->sons.empty() )
		return vecm; 
	SGFNODE* node = root->sons [0]; 
	vecm.push_back (make_pair(node->pos, node->color )); 
	while (node->sons .size ()){
		node = node->sons [0]; 
		vecm.push_back (make_pair(node->pos, node->color )); 
	}
	return vecm; 
}

void	INOUT::infop2sgf(INFOGO* infop, string filename){
	while (infop->father)
		infop = infop->father;
	string tmp = "(";
	if (!infop->xx.empty() || !infop->oo.empty()) {
		tmp = tmp + ";";
		if (!infop->xx.empty()){
			tmp = tmp + "AB";
			ITR itr(infop->xx);
			for (POS pos = itr.stonebegin(); !itr.stoneend(); pos = itr.stonenext())
				tmp = tmp + "[" + pos2aa(pos) + "]";
		}
		if (!infop->oo.empty()){
			tmp = tmp + "AW";
			ITR itr(infop->oo);
			for (POS pos = itr.stonebegin(); !itr.stoneend(); pos = itr.stonenext())
				tmp = tmp + "[" + pos2aa(pos) + "]";
		}
		if (!infop->sons.empty()){
			string s = (infop->sons[0]->getlastclr()==BLACK) ? "B":"W";
			tmp = tmp + "PL[" + s + "]";
		}
	}
	tmp = tmp + _infop2sgf(infop);
	tmp = tmp + ")\n";
	save2txt(tmp, filename);
}

string	INOUT::_infop2sgf(INFOGO* infop){
	string s = infop->getlastclr() == BLACK ? "B":"W" ;
	string tmp = "";
	if ( infop->sons.size()>1 )
		tmp = tmp + "(";
	if ( infop->father)
		tmp = tmp + ";" + s + "[" + pos2aa(infop->getlastpos()) + "]";
	for (int i=0; i<infop->sons.size(); ++i)
		tmp = tmp + _infop2sgf(infop->sons[i]);
	if ( infop->sons.size()>1 ){
		tmp = tmp + ")";
	}
	return tmp;
}

GO	INOUT::frommatlab(string filename) {	
	ifstream file(filename.c_str()); 
	if (!file) {
		cerr<< filename << " : open fail !"<< endl; 
		return GO(); 
	}
	char in; 
	string s; 
	// 只读小数点前的一个字符
	while (file.get(in)) 
		if (file.peek() == '.') 
			s += in; 
	___ASSERT(s.size() == BS*BS*2); 
	GO go; 
	for (int i = 0; i<BS; ++i){
		go.xx.r[i] = (bitset<32>(s.substr (BS*BS+BS*i, BS))).to_ulong (); 
		go.oo.r[i] = (bitset<32>(s.substr (BS*i, BS))).to_ulong (); 
	}
	file.close (); 
	return go.rotate(270); 
}

VS	 INOUT::getfilename(string path, string type){
	         string command = "ls " + path + type + " > tmp";
	         system(command.c_str());
	         string names = readbuf("tmp");
	         int p;
	         VS vecs;
	         string tmps;
	         while ((p= names.find_first_of ("\n")) != names.npos ) {
	                    tmps = names.substr (0, p),
	     		    names.replace(0,p+1,""),
	     		    vecs.push_back(tmps);
		 }
	         system("rm -f tmp");
	         return vecs;
	 }

	/*
VS		INOUT::getfilename(string path, string type){	
	//-// 无错误检测
	VS vecf; 
    struct _finddata_t c_file; 
    long hFile; 
	hFile = _findfirst( (path+type).c_str(), &c_file ); 
	vecf.push_back(path+(string)(c_file.name)); 
	while( _findnext( hFile, &c_file ) == 0 )  
		vecf.push_back(path+(string)(c_file.name)); 
    _findclose( hFile ); 
	return vecf; 
}
    */ //2009//

string INOUT::selectsgf(string path){
	VS vf = INOUT().getfilename( path, "*.sgf"); 
	if (vf.empty ()){
		cerr << "bad path !" << endl; 
		return ""; 
	}
	cout << vf << endl; 
	cout << "> "; 
	string s; 
	//cin >> s;
	s = getline()[0];
	int	m = str2i(s, 0, vf.size()); 
	clog << "Selected the ["<<m<<"] sgf file ."<< endl; 
	return vf[m]; 
}

/*
void	INOUT::save2txt( string content, string filename){	
	// out, out|trunc, out|app, in|out, out|app
	ofstream file(filename.c_str (), ios::out); 
	if (!file) 
		cerr<< filename << " : open fail !"<< endl; 
	file<<content; 
	file.close(); 
}

void	INOUT::save2txt( const GO& go, string filename){	
	ofstream file(filename.c_str (), ios::app); 
	if (!file) 
		cerr<< filename << " : open fail !"<< endl; 
	streambuf* old = cout.rdbuf (); 
	cout.rdbuf(file.rdbuf ()); 
	cout<<go; 
	cout.rdbuf (old); 
}

void   INOUT::save2txt(const VVF& vvf, string filename){	
	ofstream file(filename.c_str (), ios::ate); 
	if (!file) 
		cerr<< filename << " : open fail !"<< endl; 
	streambuf* old = cout.rdbuf (); 
	cout.rdbuf(file.rdbuf ()); 
	cout<<vvf; 
	cout.rdbuf (old); 
}
*/

/////////////////////////////////////////////////
ostream& operator<<(ostream& os, const GO& go){
	INOUT().print(go); 
	/*
	string s = INOUT().print__(go); 
	s = INOUT().axis(s); 
	for (int p = 0; p<s.size (); ){
		if (s.substr (p, 2) == "■" || s.substr (p, 2) == "□"){
			cout<<setx(R); 
			cout<<(s.substr (p, 2) == "■"?"●":"○"); 
			cout<<setx(W); 
			p += 2; 
		}
		else {
			cout<<s.substr (p, 1); 
			++p; 
		}
	}
	*/
	return os; 
}

ostream& operator<<(ostream& os, const BITB& bb){
	INOUT inout; 
	inout.axisshow = 0; 
	inout.print( GO(bb, bb) ); 
	//INOUT().printbitboard(bb); 
	return os; 
}

ostream& operator << (ostream& os, const PATFREQ& pf){
	for (int i = 0; i< PATFREQ::PEMIS_KIND; ++i)
		for (int j = 0; j<BS*BS; ++j){
			os << j<<": "<<pf.x[i][j] <<" "<< pf.o[i][j] << endl; 
		}
	return os; 
}

ostream& operator<<(ostream& os, const INFOGO& info){
	cout<<GO(info.xx, info.oo, info.getkill (), info.gethotko(), 
		info.getlastpos(), info.getlastclr (), info.gethistory() ); 
	return os; 
}

ostream& operator<<(ostream& os, const BRAIN& brain) {
	os << *brain.infop << endl; 
	return os; 
}

ostream& operator<<(ostream& os, const POS& pos){
	//return operator<<(os, _pos2xy(pos)); 
	if (!outside(pos))
		cout<<"["<<pos.first <<", "<< bitset<BS>(pos.second )<<"]"; 
	else 
		cout<<"["<<pos.first <<", "<< bitset<32>(pos.second )<<"]"; 
	return os; 
}

ostream& operator<<(ostream& os, const VS&  vecs){
	for (int i = 0; i<vecs.size (); ++i)
		cout<<"["<<i<<"] "<<vecs[i]<< endl; 
	return os; 
}
/*
ostream& operator<<(ostream& os, const VB&  vecb){
	for(int i = 0; i<vecb.size (); ++i)
		cout<<vecb[i]; 
	return os; 
}

ostream& operator<<(ostream& os, const VP&  vecp){
	for (int i = 0; i<vecp.size (); ++i){
		cout<<vecp[i]<<" "; 
	}
	return os; 
}
*/

ostream& operator<<(ostream& os, const VI&  veci){
	if (veci.size() == BS*BS){
		ITR itr; 
		for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
			cout << std::setw(4) << std::right <<veci[pos2i(pos)];
			if (pos.second == 1)   
					cout <<endl; 
		}
	}
	else 
		for (int i = 0; i<veci.size (); ++i)
			cout<<std::setw(7)<<std::right <<veci[i]<<" "; 
	
	return os; 
}

/*
ostream& operator<<(ostream& os, const VC&  vecc){
	for (int i = 0; i<vecc.size (); ++i)
		cout<<vecc[i]<<" "; 
	return os; 
}

ostream& operator<<(ostream& os, const VF& vf){
    for(int i = 0; i<vf.size (); ++i)
		cout<<vf[i]<< endl; 
	return os; 
}

ostream& operator<<(ostream& os, const VVF& vvf){
	for(int i = 0; i<vvf.size (); ++i)
		for(int j = 0; j<vvf[i].size(); ++j){
			cout<<setw(7)<<vvf[i][j]; 
			if (j == (vvf[i].size()-1))
				cout<< endl; 
		}
	return os; 
}

ostream& operator<<(ostream& os, const VVI& vvi){
	for(int i = 0; i<vvi.size (); ++i)
		for(int j = 0; j<vvi[i].size(); ++j){
			cout<<setw(7)<<vvi[i][j]; 
			if (j == (vvi[i].size()-1))
				cout<< endl; 
		}
	return os; 
}

ostream& operator<<(ostream& os, const PII& pii){
	cout<<" ["<<pii.first<<", "<<pii.second <<"] "; 
	return os; 
}

ostream& operator<<(ostream& os, const PUU&  puu){
	cout<<" ["<<puu.first<<", "<<puu.second <<"] "; 
	return os; 
}
*/
ostream& operator<<(ostream& os, const pair<EYEKIND, VP >& ts){
	cout<<eyekindname[ts.first]; 
	if (ts.second == NULL_VP) 
		cout<<"  Should pass!"<< endl; 
	else cout<<ts.second << endl; 
	return os; 
}

ostream& operator<<(ostream& os, MPI m){
	ITR itr; 
	for (POS p = itr.ioposbegin(); !itr.ioposend(); p = itr.ioposnext()){
		cout<<setw(4)<<m[p]; 
		if (p.second == 1)	cout<< endl; 
	}
	cout<< endl; 
	return os; 
}

ostream& operator<<(ostream& os, const PAT& pat){
	INOUT inout; 
	inout.axisshow = 0; 
	inout.print(GO(pat.first .xx & pat.second, pat.first .oo& pat.second), 
				pat.second .project ()); 
	return os; 
}

