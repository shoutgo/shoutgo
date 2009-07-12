#ifndef GO_
#define GO_

#include "brain.h"	
#include "inout.h"	

/* global function */

void prompt(const GO& go, MODE_PROMPT mode); 

void command(VS in); 
void command_test(VS in); 
void command_state(const INFOGO& info, VS in); 

template <class T>	void play( MODE_PLAY mode = MAN_PC); 
template <class T>	void review(); 
template <class T>	void sgftree(); 

/* class COMMAND */

enum {
	HELP_C, VERSION_C, CMD_C, THESIS_C, FONT_C, SAVECODE_C, 
	PLAY_C, FIR_C, REVIEW_C, SGFTREE_C, INPUTPAT_C, 
	ACTIVE_C, TRACK_C, COUNTERSET_C, COUNTER_C, TIME_C, SIZE_C, 
	//NEWLINE_C, 
	EXIT_C, 
	PASS_C, END_C, 
	HISTORY_C, SHOWBOARD_C, 
	DELTAMASSMAPS_C, 
	MASSMAPS_C, CLUSTER_C, POTENTIAL_C, 
	PUBLICAREA_C, ALIVE_C, DEAD_C, UNCLEAR_C, 
	LAND_C, SEA_C, AIR_C, ZOBRIST_C, PEMIS_C, 
	SCAN_C, TRYMOVE_C, 
	SAVE_C, SAVE2SGF_C, BACK_SAVE2TXT_C, 
	CMD_SIZE }; 

class COMMAND {
	static const int SETW = 20; 
private:
	struct CMD {
		VS			names; 
		string		format; 
		string		english; 
		string		chinese; 
		CMD(){}
		CMD(VS n, string f = "", string e = "", string c = "")
			:names(n), format(f), english(e), chinese(c){}
	}; 
	CMD	cmd[CMD_SIZE]; 
public:
	COMMAND(); 
	void help(int i = CMD_SIZE); 
	int which(string s); 
	bool iscommand_(VS vs); 
	bool iscommand_state_(VS vs); 
}; 

/* play(), review(), sgftree() */

template <class T>
void play( MODE_PLAY mode){
	POS	pos; 
	VS	in; 
	T	t; 
	t.init(); 
	prompt(t, BEGINGAME); 
	for(; ; ){
		for(; ; ){
			if (mode == PC_PC)
				break; 
			prompt(t, PREMOVE_MAN); 
			in = getline(); 
			if ( in[0] == "pass" || in[0] == "end") 
				break; 
			pos = INOUT().in2pos(t.getlastpos(), in[0]); 
			if (t.move(pos, t.turn())){
				t.update(); 
				prompt(t, AFTERMOVE_MAN); 
				break; 
			}
			else {
				command_test(in); 
				command_state(t, in); 
			}
		}
		if (in[0] == "end" || t.endgame()){
			prompt(t, ENDGAME); 
			break; 
		}
		for(; ; ){
			___ESC;
			if (mode == MAN_MAN)     
				break; 
			pos = t.genmove(); 
			prompt(t, PREMOVE_PC); 
			if (t.move(pos, t.turn())){
				t.update(); 
				prompt(t, AFTERMOVE_PC); 
				break; 
			}
		}
		if (t.endgame()){
			prompt(t, ENDGAME); 
			break; 
		}
	}
}

// ֻ�鿴 sgftree ������֧
template <class T>
void review(){ 
	string f = INOUT().selectsgf( SGFPATH ); 
	if (f.empty ()){
		cerr << "wrong when selecting sgf file !" << endl; 
		return; 
	}
	VM rcd = INOUT().sgf2move(f); 
	if (rcd.empty()){
		cerr << f << " is a bad sgf file!"<< endl; 
		return; 
	}
	vector<T>	vect; 
	vect.reserve (BS*BS); 
	T	t; 
	for (int i = 0; i<rcd.size (); ++i){ 
		t.move(rcd[i].first, rcd[i].second); 
		t.update(); 
		vect.push_back(t); 
		//___ESC; 
		//command_state(t, BIND<string, char*>()(1, "scan")); 
	}
	//for thesis
	GO goo,go;
	INFOGO inff,inf;
	___.tic("go.move()");
	for (int t=0;t<100;++t){
		for (int i=0;i<rcd.size();++i)
			goo.move(rcd[i].first,rcd[i].second);
		goo = go;
	}
	___.toc();
	___.tic("info.movebyinfo()");
	for (int t=0;t<100;++t){
		for (int i=0;i<rcd.size();++i)
			inff.move(rcd[i].first,rcd[i].second);
		inff = inf;
	}
	___.toc();
	___COUT1(rcd.size());
	//for thesis
	cout << vect.back ();
	cout << endl;
	t = vect[0]; 
	cout << t; 
	for (int i = 0; 0 <= i && i < rcd.size (); ){
		cout << endl << ">  "; 
		VS in = getline(); 
		if ( isdigit( in[0][0] ) || in[0][0] == '-') {
			i += str2i(in[0], -i, rcd.size()-1-i); 
			t = vect[i]; 
			cout << t ; 
		}
		else
			if (in[0] == "end")
				break; 
			else {
				command_test(in); 
				command_state(t, in); 
			}
	}
}

template <class T>
void sgftree(){
	// ��ʼ����
	SGFROOT *root = INOUT().sgf2tree(INOUT().selectsgf()); 
	SGFNODE *cnode = new SGFNODE; 
	T* t_ptr = new T; 
	if (!root->intro.empty ())
		cout << root->intro << endl; 
	root->fuseki(t_ptr); 
	// α��
	cnode->sons = root->sons; 
	for (int i = 0; i<cnode->sons .size (); ++i)
		cnode->sons [i]->father = cnode; 
	// ��β鿴		
	for(; ; ){
		// ��ʾ����ʾ ( Ҳ������ʾ����� bigson ����� )
		BITB bb; 
		for (int i = 0; i<cnode->sons .size (); ++i)
			bb |= cnode->sons[i]->pos; 
		cout << (*t_ptr)(bb); 
		// �������Ҷ���
		if (cnode->sons.empty ()){
			for (; ; ) {		
				cout<< endl << ">  ";
				cout<< "the leaf node now. end ? [y/n] "; 
				string tmp = getline()[0]; 
				if ( tmp == "y" ) {
					// �ƺ���Ҫɾ������Ǹ� cnode
					delete root; 
					t_ptr->delfromroot(); 
					return; 
				}
				else if (tmp == "n")
					break;
			}
		}
		// ѡ����ʷ�֧
		int m; 
		for(; ; ){
			cout<< endl << ">  "; 
			VS in = getline(); 
			// �򻯲���
			if (in[0] == "end")
				return; 
			if (in[0] == "y" && !cnode->sons.empty() ){
				cnode = cnode->sons[0]; 
				break; 
			}
			// ����һ���Ϸ�λ��
			POS pos = INOUT().in2pos(t_ptr->getlastpos(), in[0]); 
			if (bb[pos]){
				for (int i = 0; i<cnode->sons.size (); ++i)
					if (pos == cnode->sons[i]->pos )
						cnode = cnode->sons [i]; 
				break; 
			}				
			// ���� (������������ʽ����Ϊ���ܺ��������뷨��ͻ)
			else if (in[0][0] == '-'){
				m = atoi(in[0].c_str ()); 
				for (; m<0 && (t_ptr->father); ++m ){
					t_ptr = t_ptr->father; 
					cnode = cnode->father; 
				}
				for (int i = 0; i<t_ptr->sons .size (); ++i)
					t_ptr->sons[i]->delfromhere(); 
				t_ptr->sons .clear (); 
				break; 
			}
			// ���������
			command_test(in); 
			command_state(*t_ptr, in); 
		}
		// ����
		t_ptr = t_ptr->move_tree(cnode->pos, cnode->color ); 
		t_ptr->update(); 
	}
}

#endif


