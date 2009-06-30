#include "stdafx.h"
#include "inout.h" 
#include "io.h"
#include "windows.h"	// GetStdHandle() SetConsoleTextAttribute()	// VS2008
#include "direct.h"		// _mkdir()									// VS2008

#if		IO_MACRO == 0
#undef	___FUNCOUNT
#define ___FUNCOUNT(fun) {}
#undef	___TIME
#define ___TIME(code)	{}
#endif

// �ĩ۩ө̩���p�שߩϩ�
// ���ѡ������������������
// �ŢƢǢȢɢʢˢ̢͢΢ϢТѢҢӢԢբ֢ע�
// �����������������������������������¢â�
// ���£ãģţƣǣȣɣʣˣ̣ͣΣϣУѣңӣԣգ֣ףأ٣�
// ���������������������������������
// ABCDEFGHIJKLMNOPQRSTUVWXYZ
// abcdefghijklmnoprrstuvwxyz
// �٢ڢۢܢݢޢߢ���

// ��Ϊ�Ѳ�����Ϊ����
// ��������������������
// ??????????????????????????
// ??????????
// ??????????
// ??????????
// �٢ڢۢܢݢޢߢ���??????????
// ??????????????????????????
// ??????????????????????????

INOUT::INOUT(bool b):axisshow(b){
}

string     INOUT::logo_() {	
	___FUNCOUNT( IO_LOGO); 
	static string s = ""; 
		s += " -+    -+    ++ \n"; 
		s += "   ��өөӡ�   \n"; 
		s += "   �Ģۢڢ٩�   \n"; 
		s += " --�Ģޢݢܩ�++ \n"; 
		s += "   �Ģ��ߩ�   \n"; 
		s += "   ��۩۩ۡ�   \n"; 
		s += " --    +-    +- \n"; 
	return s; 
}

string   INOUT::grid(const POS& pos) {	
	___FUNCOUNT( IO_GRID); 
	int		 r = pos.first; 
	ROW		 p = pos.second; 
	if (r == 0 && p == LEFTEST)		{return "��"; }
	if (r == 0 && p == 1)			{return "��"; }
	if (r == BS-1 && p == 1)		{return "��"; }
	if (r == BS-1 && p == LEFTEST)	{return "��"; }
	if (r == 0)						{return "��"; }
	if (p == 1)						{return "��"; }
	if (r == (BS-1))				{return "��"; }
	if (p == LEFTEST)				{return "��"; }
	if ((BS >= 13) &&
		((r == 3) || (r == MIDBS) || (r == (BS-1-3)) ) &&
		((p == (1<<3)) || (p == (CENTEREST)) || (p == (1<<(BS-1-3)))))
									{return "��"; }
	if (r == MIDBS && p == CENTEREST )
									{return "��"; }
	else 							{return "��"; }
}

POS		 INOUT::in2pos(const POS& pos, string in) {	
	___FUNCOUNT( IO_IN2POS); 

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
	___FUNCOUNT( IO_JJ2POS); 
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
	___FUNCOUNT( IO_AXIS); 
	if (!axisshow)
		return "  "; 
	//static string h = "�ڣ٣أף֣գԣӣңѣУϣΣ̣ͣˣʣɣȣǣƣţģã£�";
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
	___FUNCOUNT( IO_PRINT1); 
	string s; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		if (pos.second & LEFTEST && mask.r[pos.first])
			cout << axis(pos); 
		if ( mask[pos] ){ 
			switch (go[pos]) {
				case BLACK:				
					s = "��"; break; 
				case WHITE:				
					s = "��"; break; 
				case BLACK_WHITE:	   	
					s = "��"; break; 
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
	___FUNCOUNT( IO_PRINT2); 
	string s; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		if (pos.second & LEFTEST)
			cout << axis(pos); 
		switch (go[pos]) {
			case BLACK:				
				s = "��"; break; 
			case WHITE:				
				s = "��"; break; 
			case BLACK_WHITE:	   	
				s = "��"; break; 
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
	___FUNCOUNT( IO_PRINT); 
	for (int i = 0; i<BS; ++i) 
		___ASSERT((go.xx.r [i]| go.oo.r [i]) <= ROWMASK); 
	string s = ""; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		if (bb[pos]){
			if (mode == BITB_P)				
				s += "��"; 
			if (mode == ONEAREA_P && (go[pos] == EMPTY)) 
				s += "��"; 
			if (mode == BLOCK_P||mode == BOARD_P||mode == ONEAREA_P) {		
				if(go[pos] == BLACK)			 
					s += "��"; 
			    if(go[pos] == WHITE)          
					s += "��"; 
			    if(go[pos] == BLACK_WHITE)     
					s += "��"; 
			    if(go[pos] == HOTKO)             
					s += "�p"; 
			}
			if (pos.second == 1)
				s += "\n"; 
			continue; 
		}
		switch (go[pos]) {
			case BLACK:	
				if (pos == pp) {
					s += "��"; 
					break; 
				}
				if (pos == go.getlastpos() ) 
					s += "��"; 
				else                 
					s += "��"; 
				break; 
			case WHITE:	
				if (pos == pp) 	{
					s += "��"; 
					break; 
				}
				if (pos == go.getlastpos() ) 
					s += "��"; 
				else		         
					s += "��"; 
				break; 
			case BLACK_WHITE:	    
				s += "��"; 
				break; 
			case HOTKO:		
				s += "�p"; 
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
	___FUNCOUNT( IO_PRINTBITBOARD); 
	string s; 
	ITR itr; 
	for (POS pos = itr.ioposbegin(); !itr.ioposend(); pos = itr.ioposnext()) {
		switch (bb[pos]) {
			case 1:	            
				s += "��"; 
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
	___FUNCOUNT( IO_PRINTLABEL); 
	string black = "���£ãģţƣǣȣɣʣˣ̣ͣΣϣУѣңӣԣգ֣ףأ٣�"; 
	string white = "���������������������������������"; 
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
		// ���� break �Թ۲����Ĺ����Ƿ�Ψһ
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
				case BLACK:			s += "��"; break; 
				case WHITE:			s += "��"; break; 
				case BLACK_WHITE:	s += "��"; break; 
				case HOTKO:			s += "�p"; break; 
				case EMPTY:			s += grid(pos); break; 
			}														
		if (pos.second == 1)			
			s += "\n"; 
	}
	cout<<(s += "\n"); 
	return s; 
}

string	 INOUT::printcluster ( const GO& go, const pair<VB, VB >& pvv) {	
	___FUNCOUNT( IO_PRINTCLUSTER); 
	return printlabel(go, pvv, L_CLUSTER); 
}

string	 INOUT::printarea ( const GO& go, const pair<VB, VB >& pvv) {	
	___FUNCOUNT( IO_PRINTAREA); 
	return printlabel(go, pvv, L_AREA1); // mode = L_AREA1 or L_AREA2
}

string	 INOUT::printonearea ( const GO& go, const BITB& bb) {	
	___FUNCOUNT( IO_PRINTONEAREA); 
	return print__(go, bb, NULL_POS, ONEAREA_P); 
}

string	 INOUT::printpublicarea ( const GO& go, const BITB& bb) {	
	___FUNCOUNT( IO_PRINTPUBLICAREA); 
	return print__(go, bb, NULL_POS, BITB_P); 
}

string   INOUT::printblock(const GO& go, const BITB& blk, const POS& pos) {	
	___FUNCOUNT( IO_PRINTBLOCK); 
	return	print__(go, blk, pos, BLOCK_P); 
}

string   INOUT::printlibertysite(const GO& go, 
								 const BITB& libsite, const POS& pos) {	
	___FUNCOUNT( IO_PRINTLIBERTYSITE); 
	return	print__(go, libsite, pos, BITB_P); 
}

string   INOUT::printpartition(const VG& vg){	
	___FUNCOUNT( IO_PRINTPARTITION); 
	string black = "��ŢƢǢȢɢʢˢ̢͢΢ϢТѢҢӢԢբ֢ע�"; 
	string white = "�𢱢��������������������������������¢â�"; 
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
					s += "��"; 
					break; }
				case HOTKO:			  
					{++find; 
					s += "�p"; 
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
	___FUNCOUNT( IO_PRINTLIBERTY); 
	
	//-// 20���ַ�������, ���ܵ�������ʱ����, ���Ľ�
	string black = "��ŢƢǢȢɢʢˢ̢͢΢ϢТѢҢӢԢբ֢ע�"; 
	string white = "�������������������������������������¢â�"; 
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
// �� mask ���Գʲ�������״
string	INOUT::printpat(const GO& old, const BITB& mask){	
	___FUNCOUNT( IO_PRINTPAT1); 
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
						s += "��"; 
					//else 
					//	s += "  "; 
					break; 
				case WHITE:				
					//if (mask[pos])
						s += "��"; 
					//else 
					//	s += "  "; 
					break; 
				case BLACK_WHITE:	   	
					//if (mask[pos])
						s += "��"; 
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
	for (int pp = s.find ("��"); pp != s.npos; pp = s.find ("��"))
        	s.replace(pp, 2, "��"); 
	cout<<(s += "\n"); 
	return s; 
}

// �� mask ���Գʲ�������״
string	INOUT::printpat(const BITB& old, const BITB& mask) {	
	___FUNCOUNT( IO_PRINTPAT2); 
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
						s += "��"; 
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
	for (int pp = s.find ("��"); pp != s.npos; pp = s.find ("��"))
        	s.replace(pp, 2, "��"); 
	cout<<(s += "\n"); 
	return s; 
}
*/

// ��pat2ascii()���Ƕ�Ӧ��ϵ.ģʽ"xx\n-x\n"�е�-��û�������, 
// ����������pat2ascii()����.
// ���in2pat()ͬʱ����һ����Ӧ mask �� string ����Խ��.
// �����Ƿ�Ϸ���inputpat()����, Ĭ�ϳ�������(BS-1)/2
// �ɼ��� X O ��ʾ���ϵ���, �����
PAT	INOUT::in2pat(string s){	
	___FUNCOUNT( IO_IN2PAT); 
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
	n = p/m - 1; //-// ȡģ��			
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

	// + ����  - ��֮ . ����֮  
	// ���ڿ���λ�õ� + - �� . ����
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
		//����ʶ�ķ���һ�����հ׵�
		for (int j = 0; j<n; ++j){			
			if		(smn[i][j] == "x")		{	
				go.xx.r[i+i0] |= (1<<(j0-j)); 
			}
			else if (smn[i][j] == "o")		{	
				go.oo.r[i+i0] |= (1<<(j0-j)); 
			}		
		}

	return make_pair(go, mask); //δ�� project()
}

//ֱ���û�����������frommatlab()�����ְ��ֶ���ķ�ʽ��öࡣ
string	INOUT::readbuf(string filename){	
	___FUNCOUNT( IO_READBUF); 
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

void	INOUT::savecode(string path){	
	___FUNCOUNT( IO_SAVECODE); 
	LPSTR s = new TCHAR[256]; 
	wsprintf(s, "%u", (unsigned)time(0)); 
	::_mkdir(s); 
	VS	h = getfilename (path, "*.h"); 
	VS	cpp = getfilename (path, "*.cpp"); 
	VS  txt = getfilename (path, "*.txt"); 
	VS  all(h); 
	copy(cpp.begin(), cpp.end(), back_inserter(all)); 
	copy(txt.begin (), txt.end (), back_inserter(all)); 
	___COUT4( h.size(), cpp.size(), txt.size (), all.size () ); 
	___ASSERT( all.size() == h.size() + cpp.size() + txt.size () ); 
	for (int i = 0; i<all.size (); ++i){
		string tmp = readbuf(all[i]); 
		all[i].replace(all[i].begin(), all[i].begin()+path.length(), ""); 
		save2txt ( tmp, path + string(s) + "\\" + all[i]); 
	}
}


/*
VP	INOUT::sgf2pos(string filename) {	
	___FUNCOUNT( IO_SGF2POS); 
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
	___FUNCOUNT( IO_SGF2XY); 
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
	___FUNCOUNT( IO_SGFS2POS); 
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
	___FUNCOUNT( IO_SGFS2XY); 
	vector<vector<PII > >  vech; 
	for (int i = 0; i<vecf.size (); ++i)
		vech.push_back (sgf2xy(vecf[i])); 
	return vech; 
}
*/

// С���ţ���֮����һ��ȫ�ַ�֧����Ƕ��
// �����ţۣ�֮����ʵ�����ݣ�������Ƕ��
// �˴��Ľ�㲻��ȫ�ַ�֧��������һ���߲�
SGFROOT* INOUT::sgf2tree(string filename){	
	___FUNCOUNT( IO_SGF2TREE); 
	string s = readbuf(filename.c_str ()); 
	int	unmatch = 0; // δƥ�����С���ţ�
	int	m = 0; // δƥ����������ţ�
	int	expand[1024] = {0}; // ����Ӧ��δƥ��ģ���չ���Ľڵ���������Ի���
	int	p; // �� s �ϵĹ���λ��

	/* ���ɸ���� */

	SGFROOT* root = new SGFROOT; 
	//������㷨��ȷ���������滻���ո��з���
	while ((p = s.find_first_of ("\t\n\r ")) != s.npos )
		s.replace(p, 1, ""); 
	root->intro = s.substr (0, 0); 
	// �ž�
	if ((p = s.find("AB[")) != s.npos)
		for(p += 2; s.substr(p, 1) == "["; p += 4) 
				//root->xfuseki.push_back(_xy2pos(s[p+1]-'a', (s[p+2]-'a'))); 
				root->xfuseki.push_back(aa2pos(s.substr(p+1,2))); 
	if ((p = s.find("AW[")) != s.npos)
		for(p += 2; s.substr(p, 1) == "["; p += 4) 
			if (s.substr(p, 1) == "[") 
				//root->ofuseki.push_back(_xy2pos(s[p+1]-'a', (s[p+2]-'a'))); 
				root->ofuseki.push_back(aa2pos(s.substr(p+1,2))); 
	// ��һ�ָ�˭��
	if ((p = s.find("PL[")) != s.npos)
		root->turn = ((s.substr (p+3, 1) == "B")?BLACK:WHITE); 
	else root->turn = BLACK; 

	/* �״�Ϊα����㣬�����ᱻ root ��û������Ϊ������� */

	// ��ǰ��� cnode ������ֵ��Ϊ�����ı�ɫ
	SGFNODE* cnode = new SGFNODE; 
	cnode->color = ~(root->turn); 
	// ���ַ��ƽ�
	for (p = s.find("("); p<s.size (); ++p){
		// ��ͨ�ַ�������
		// ������ ʱ������һ����֮�������һ���Ѵ�������
		if (s.substr(p, 1) == "[")
			++m; 
		else if (s.substr(p, 1) == "]")
			--m; 
		if (m != 0) 
			continue; 
		// һ����֧��ʼ����¼��֧��ʼʱ�ķ�֧����
		if (s.substr(p, 1) == "("){										
			++unmatch; 
			expand[unmatch] = 0; 
		}
		// �������ţ���ǰ��3���ַ�
		else if (s.substr(p, 3) == ";B[" || s.substr(p, 3) == ";W["){						
			++expand[unmatch]; 
		}
		// ���� B 
		else if (s.substr(p-1, 3) == ";B["){		
			// ���ӷ�����Ծ������PASS
			if (cnode->color == BLACK){
				SGFNODE* newnode = new SGFNODE; 
				cnode->sons.push_back(newnode); 
				newnode->father = cnode; 
				newnode->pos = PASS_POS; 
				newnode->color = WHITE; 
				cnode = newnode; 
				++expand[unmatch]; 
			}
			// ��������һ�����
			SGFNODE* newnode = new SGFNODE; 
			cnode->sons.push_back(newnode); 
			newnode->father = cnode; 
			//newnode->pos = _xy2pos(s[p+2]-'a', (s[p+3]-'a') ); 
			newnode->pos = aa2pos(s.substr(p+2,2)); 
			newnode->color = BLACK; 
			cnode = newnode; 
		}
		// ���� W
		else if (s.substr(p-1, 3) == ";W["){										
			// ���ӷ�����Ծ������PASS
			if (cnode->color == WHITE){
				SGFNODE* newnode = new SGFNODE; 
				cnode->sons.push_back(newnode); 
				newnode->father = cnode; 
				newnode->pos = PASS_POS; 
				newnode->color = BLACK; 
				cnode = newnode; 
				++expand[unmatch]; 
			}
			// ��������һ�����
			SGFNODE* newnode = new SGFNODE; 
			cnode->sons.push_back(newnode); 
			newnode->father = cnode; 
			//newnode->pos = _xy2pos(s[p+2]-'a', (s[p+3]-'a') ); 
			newnode->pos = aa2pos(s.substr(p+2,2)); 
			newnode->color = WHITE; 
			cnode = newnode; 
		}
		// һ����֧�������˻ص����ʲ���
		else if (s.substr(p, 1) == ")"){							
			for(int i = 0; i<expand[unmatch]; ++i)
				cnode = cnode->father; 
			--unmatch; 
		}															
	}														

	/* ��ʵ�ĸ���� */

	root->sons = cnode->sons; 
	for(int i = 0; i< root->sons.size(); ++i)
		root->sons[i]->father = 0; 
	
	/* ��֤ */

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
	___FUNCOUNT( IO_FROMMATLAB); 
	ifstream file(filename.c_str()); 
	if (!file) {
		cerr<< filename << " : open fail !"<< endl; 
		return GO(); 
	}
	char in; 
	string s; 
	// ֻ��С����ǰ��һ���ַ�
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

VS		INOUT::getfilename(string path, string type){	
	___FUNCOUNT( IO_GETFILENAME); 
	//-// �޴�����
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
	___FUNCOUNT( IO_SAVE1); 
	// out, out|trunc, out|app, in|out, out|app
	ofstream file(filename.c_str (), ios::out); 
	if (!file) 
		cerr<< filename << " : open fail !"<< endl; 
	file<<content; 
	file.close(); 
}

void	INOUT::save2txt( const GO& go, string filename){	
	___FUNCOUNT( IO_SAVE2); 
	ofstream file(filename.c_str (), ios::app); 
	if (!file) 
		cerr<< filename << " : open fail !"<< endl; 
	streambuf* old = cout.rdbuf (); 
	cout.rdbuf(file.rdbuf ()); 
	cout<<go; 
	cout.rdbuf (old); 
}

void   INOUT::save2txt(const VVF& vvf, string filename){	
	___FUNCOUNT( IO_SAVE3); 
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
		if (s.substr (p, 2) == "��" || s.substr (p, 2) == "��"){
			cout<<setx(R); 
			cout<<(s.substr (p, 2) == "��"?"��":"��"); 
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

ostream& operator<< (ostream & os, setx& s){
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdOut, s.c); 
	return os; 
}

ostream& operator<<(ostream& os, const PAT& pat){
	INOUT inout; 
	inout.axisshow = 0; 
	inout.print(GO(pat.first .xx & pat.second, pat.first .oo& pat.second), 
				pat.second .project ()); 
	return os; 
}

/* _______________________________ ������ _______________________________ */

void TEST_IO::pattern(){
	string ina = 
					"\
		+--------	\n\
		.........	\n\
		....xo..o	\n\
		...x.o...	\n\
		.........	\n\
		.........	\n\
		..x......	\n\
					"; 
	string inb = 
					"\
		+------		\n\
		.......		\n\
		......x		\n\
		...x...		\n\
		..x....		\n\
		..oo...		\n\
		.......		\n\
		.......		\n\
		..o....		\n\
					"; 

	PAT  pata = INOUT().in2pat(ina); 
	PAT  patb = INOUT().in2pat(inb); 
	PUU	 a3232 = pata.first .pemis3232(pata.second ); 
	PUU	 b3232 = patb.first .pemis3232(patb.second ); 
	___COUT6(
		ina, pata.first, pata.second, 
		inb, patb.first, patb.second 
		); 
	___COUT4(
		a3232, 
		b3232, 
		pata.first.pemis64(pata.second ), 
		patb.first.pemis64(patb.second )
		); 
	//___ASSERT(issamepat(ina, inb)); 
	___ASSERT(a3232 == b3232); 

	BITB paton = strip(star(8), 3, 1); 
	___COUT1(paton); 
	BITB bb; 
	bb.random(); 
	BITB patmask = bb.blockon(paton); 
	PUU bb3232 = (bb&patmask).pemis3232(); 
	ULL	bb64 = (bb&patmask).pemis64(); 
	cout << make_pair(GO(bb, bb), patmask); 
	___COUT4(
		bb, 
		patmask, 
		//INOUT().printpat(bb, patmask), 
		//asciipat(INOUT().printpat(bb, patmask)), 
		//pat2ascii(bb, patmask), 
		bb3232, 
		bb64
		); 
	___ASSERT((ULL)(bb3232.first )*((ULL)1<<32)+(ULL)(bb3232.second ) == bb64); 

	GO go; 
	go.random (); 
	patmask = go.xx.blockon (paton)|go.oo.blockon (paton); 
	PUU go3232 = go.pemis3232 (patmask); 
	ULL go64 = go.pemis64(patmask); 
	cout << make_pair(go, patmask); 
	___COUT4(
		go, 
		patmask, 
		//INOUT().printpat(go, patmask), 
		//asciipat(INOUT().printpat(go, patmask)), 
		//pat2ascii(go.xx, go.oo, patmask), 
		go3232, 
		go64
		); 
	___ASSERT((ULL)(go3232.first )*((ULL)1<<32)+(ULL)(go3232.second ) == go64); 
	
}

/*
void TEST_IO::sgftree(){
	// ��ʼ����
	SGFROOT *root = INOUT().sgf2tree(INOUT().selectsgf()); 
	SGFNODE *cnode = new SGFNODE; 
	INFOGO* infop; 
	infop = new INFOGO; 
	cout << root->intro << endl << "start:\n"; 
	root->fuseki(infop); 
	cout << *infop; 
	cnode->sons = root->sons; 
	for (int i = 0; i<cnode->sons .size (); ++i)
		cnode->sons [i]->father = cnode; 

	// ��β鿴		
	for(; ; ){
		// �������Ҷ���
		if (cnode->sons.empty ()){
			delete root; 
			infop->delfromroot(); 
			// �ƺ���Ҫɾ������Ǹ� cnode
			return; 
		}
		// ��ʾ
		if (cnode->sons.size () == 1)
			cout<<"branch: "<<"[0]"<< endl; 
		else 
			cout<<"branch: "<<"[0]-["
				<< cnode->sons .size ()-1<<"]"<< endl; 
		// ѡ����ʷ�֧
		int m; 
		for(; ; ){
			cout<< endl << ">  "; 
			string s; 
			cin >> s; 
			if (isdigit(s[0])|| s[0] == '-'){
				m = atoi(s.c_str ()); 
				if( 0 <= m && m< cnode->sons .size ()){
					cnode = cnode->sons [m]; 
					break; 
				}
				// ����
				else if ( m<0 ) {
					for (; m<0 && (infop->father); ++m ){
						infop = infop->father; 
						cnode = cnode->father; 
					}
					for (int i = 0; i<infop->sons .size (); ++i)
						infop->sons[i]->delfromhere(); 
					infop->sons .clear (); 
					break; 
				}
			}
		}
		// ��ʾ
		infop = infop->move_tree(cnode->pos, cnode->color ); 
		cout<<*infop; 
	}
}
*/

void TEST_IO::inputpat(){
	char	chars[256]; 
	string	patstr, tmp; 
	int		n; 
	PAT		pat; 
	cout<<"input the pattern: \n"; 
	cin.getline(chars, 256); 
	tmp = (string)(chars); 
	n = tmp.size (); 
	for (; ; )
		if (tmp == ""){
			if (patstr == "") break; 
			pat = INOUT().in2pat(patstr); 
			cout<<"pemis64(): "<<pat.first.pemis64(pat.second)<< endl; 
			cout<<pat; 
			//INOUT().printpat(pat.first, pat.second ); 
			break; 
		}
		else{
			if (tmp.size() != n){
				inputpat(); 
				break; 
			}
			patstr += (tmp+"\n"); 
		    cin.getline(chars, 256); 
			tmp = (string)(chars); 
		}
}

void TEST_IO::timefunc (string which) {
	RANDER  r; 
	
	INOUT	io = INOUT(); 
	pair<VB, VB> pvv = make_pair(r.vb, r.vb); 
	VI	vi; 
	vi.assign (BS*BS, 1); 
	VVF vvf = randvvf(5, 3); 

	___TIME( IO_LOGO, io.logo_ ( ); ); 
    ___TIME( IO_GRID, io.grid ( r.vp[0]); ); 
	//___TIME( IO_PRINTBITBOARD, io.printbitboard ( r.vb[0] ); ); 
	//___TIME( IO_PRINT, io.print__ ( r.vg[0], r.vb[0], r.vp[0], BOARD_P ); ); 
	/*
	___TIME( IO_PRINTBLOCK, io.printblock(r.vg[0], r.vb[0], r.vp[0] ); ); 
	___TIME( IO_PRINTLIBERTYSITE, io.printlibertysite(r.vg[0], r.vb[0], r.vp[0] ); ); 
    ___TIME( IO_PRINTLIBERTY, io.printliberty( r.vg[0], vi); ); 
    ___TIME( IO_PRINTPARTITION, io.printpartition (r.vg ); ); 
	___TIME( IO_PRINTLABEL, io.printlabel ( r.vg[0], pvv, L_CLUSTER); ); 
	___TIME( IO_PRINTCLUSTER, io.printcluster ( r.vg[0], pvv); ); 
	___TIME( IO_PRINTAREA, io.printarea ( r.vg[0], pvv); ); 
	___TIME( IO_PRINTONEAREA, io.printonearea ( r.vg[0], r.vb[0]); ); 
	___TIME( IO_PRINTPUBLICAREA, io.printpublicarea ( r.vg[0], r.vb[0]); ); 
	*/
	//___TIME( IO_PRINTPAT1, io.printpat ( r.vg[0], r.vb[0]); ); 
	//___TIME( IO_PRINTPAT2, io.printpat ( r.vb[0], r.vb[1]); ); 
	
	/*
	___TIME( IO_SGF2POS, io.sgf2pos(r.vs[0]); ); 
	___TIME( IO_SGF2XY, io.sgf2xy(r.vs[0]); ); 
	___TIME( IO_SGFS2POS, io.sgfs2pos(r.vs); ); 
	___TIME( IO_SGFS2XY, io.sgfs2xy(r.vs); ); 
	*/
	___TIME( IO_IN2PAT, io.in2pat(r.vs[0]); ); 
	___TIME( IO_IN2POS, io.in2pos( r.vp[0], r.vs[0] ); ); 
	___TIME( IO_JJ2POS, io.jj2pos( r.vs[0], r.vp[0]); ); 
	___TIME( IO_GETFILENAME, io.getfilename(r.vs[0], r.vs[1]); ); 
	___TIME( IO_SGF2TREE, io.sgf2tree(r.vs[0]); ); 
	___TIME( IO_FROMMATLAB, io.frommatlab(r.vs[0]); ); 
	/*
	___TIME( IO_SAVE1, io.save2txt(r.vs[0], r.vs[1]); ); 
	___TIME( IO_SAVE2, io.save2txt(vvf, r.vs[0]); ); 
	___TIME( IO_SAVE3, io.save2txt(r.vg[0], r.vs[0]); ); 
	*/
	___TIME( IO_SAVECODE, io.savecode(r.vs[0]); ); 
	___TIME( IO_READBUF, io.readbuf(r.vs[0]); ); 
}

