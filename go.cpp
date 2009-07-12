#include "go.h"
#include "inout.h"

/* class COMMAND */

// �򻯹���˫�ָ�ȡǰһ��ĸ������ȡǰ����ĸ��
COMMAND::COMMAND() {
	cmd[HELP_C] = CMD ( BIND<string, char*>()(2, "help", "he"), 
		"[command]", 
		"...", 
		"��ʾȫ����ĳ������İ�����Ϣ"); 
	cmd[VERSION_C] = CMD ( BIND<string, char*>()(2, "version", "ve" ), 
		"[ ]", 
		"...", 
		"��ʾ�汾��Ϣ"); 
	cmd[CMD_C] = CMD ( BIND<string, char*>()(2, "cmd", "cm"), 
		"[ ]", 
		"...", 
		"ִ�� CMD.EXE ����"); 
	cmd[THESIS_C] = CMD ( BIND<string, char*>()(2, "thesis", "th"), 
		"[ ]", 
		"...", 
		"ִ��һϵ�ж�������"); 
	cmd[FONT_C] = CMD ( BIND<string, char*>()(2, "font", "fo"), 
		"[ ]", 
		"...", 
		"ִ�� FONT.EXE ����"); 
	cmd[SAVECODE_C] = CMD ( BIND<string, char*>()(2, "savecode", "sc"), 
		"[ ]", 
		"...", 
		"���汾����Դ����"); 
	cmd[PLAY_C] = CMD ( BIND<string, char*>()(2, "play", "pl"), 
		"[MAN_PC, PC_PC, MAN_MAN]", 
		"...", 
		"��ָ����ʽ����"); 
	cmd[FIR_C] = CMD ( BIND<string, char*>()(2, "fir", "fi"), 
		"[MAN_PC, PC_PC, MAN_MAN]", 
		"...", 
		"��ָ����ʽ��������"); 
	cmd[REVIEW_C] = CMD ( BIND<string, char*>()(2, "review", "re"), 
		"[ ]", 
		"...", 
		"���� SGF �ļ�����ʾ"); 
	cmd[SGFTREE_C] = CMD ( BIND<string, char*>()(2, "sgftree", "st"), 
		"[ ]", 
		"...", 
		"������ʽ���� SGF �ļ�����ʾ"); 
	cmd[INPUTPAT_C] = CMD ( BIND<string, char*>()(2, "inputpat", "ip"), 
		"[ ]", 
		"...", 
		"����ģʽ"); 
	cmd[ACTIVE_C] = CMD ( BIND<string, char*>()(2, "active", "ac"), 
		"[ ]", 
		"...", 
		"�������в��Ժ���"); 
	cmd[TRACK_C] = CMD ( BIND<string, char*>()(2, "track", "tr"), 
		"[numb]", 
		"...", 
		"�����쳣�����������ִ�е� numb ��������"); 
	cmd[COUNTERSET_C] = CMD ( BIND<string, char*>()(2, "counterset", "cs"), 
		"[numb]", 
		"...", 
		"���ü�����״̬"); 
	cmd[COUNTER_C] = CMD ( BIND<string, char*>()(2, "counter", "co"), 
		"[ ]", 
		"...", 
		"��ʾ������״̬"); 
	cmd[TIME_C] = CMD ( BIND<string, char*>()(2, "time", "ti"), 
		"[function( eg: \"\", \"u\", \"U_RANDU\" )]", 
		"...", 
		"����ȫ����������������ض�����"); 
	cmd[SIZE_C] = CMD ( BIND<string, char*>()(2, "size", "si"), 
		"[ ]", 
		"...", 
		"��ʾ����Ĵ�С"); 
	//cmd[NEWLINE_C] = CMD ( BIND<string, char*>()(3, "newline", "nl", ";"), 
	//	"[ ]", 
	//	"...", 
	//	"����"); 
	cmd[EXIT_C] = CMD ( BIND<string, char*>()(2, "exit", "ex"), 
		"[ ]", 
		"...", 
		"�˳�������"); 

	cmd[PASS_C] = CMD ( BIND<string, char*>()(1, "pass"), 
		"[ ]", 
		"...", 
		"��Ȩһ��"); 
	cmd[END_C] = CMD ( BIND<string, char*>()(1, "end"), 
		"[ ]", 
		"...", 
		"������ǰ��������"); 
	cmd[HISTORY_C] = CMD ( BIND<string, char*>()(2, "history", "hi"), 
		"[ ]", 
		"...", 
		"��ʾ��ʷֵ"); 
	cmd[SHOWBOARD_C] = CMD ( BIND<string, char*>()(2, "showboard", "sb"), 
		"[ ]", 
		"...", 
		"��ʾ����"); 
	cmd[DELTAMASSMAPS_C] = CMD ( BIND<string, char*>()(2, "deltamassmaps", "dm"), 
		"[numb]", 
		"...", 
		"��ʾ numb �����ŵı仯����"); 
	cmd[MASSMAPS_C] = CMD ( BIND<string, char*>()(2, "massmaps", "mm"), 
		"[numb]", 
		"...", 
		"��ʾ numb ������"); 
	cmd[CLUSTER_C] = CMD ( BIND<string, char*>()(2, "cluster", "cl"), 
		"[ ]", 
		"...", 
		"��ʾ��Ⱥ"); 
	cmd[POTENTIAL_C] = CMD ( BIND<string, char*>()(2, "potential", "po"), 
		"[ ]", 
		"...", 
		"��ʾ��"); 
	cmd[PUBLICAREA_C] = CMD ( BIND<string, char*>()(2, "publicarea", "pa"), 
		"[ ]", 
		"...", 
		"��ʾ������"); 
	cmd[ALIVE_C] = CMD ( BIND<string, char*>()(2, "alive", "al"), 
		"[ ]", 
		"...", 
		"��ʾ����Ⱥ"); 
	cmd[DEAD_C] = CMD ( BIND<string, char*>()(2, "dead", "de"), 
		"[ ]", 
		"...", 
		"��ʾ����Ⱥ"); 
	cmd[UNCLEAR_C] = CMD ( BIND<string, char*>()(2, "unclear", "un"), 
		"[ ]", 
		"...", 
		"��ʾ���������Ⱥ"); 
	cmd[LAND_C] = CMD ( BIND<string, char*>()(2, "land", "la"), 
		"[ ]", 
		"...", 
		"��ʾ��Ⱥ�ġ�½�ء�"); 
	cmd[SEA_C] = CMD ( BIND<string, char*>()(2, "sea", "se"), 
		"[ ]", 
		"...", 
		"��ʾ��Ⱥ�ġ�����"); 
	cmd[AIR_C] = CMD ( BIND<string, char*>()(2, "air", "ai"), 
		"[ ]", 
		"...", 
		"��ʾ��Ⱥ�ġ���ա�"); 
	cmd[SCAN_C] = CMD ( BIND<string, char*>()(2, "scan", "sc"), 
		"[ ]", 
		"...", 
		"ɨ�赱ǰ��ֵ���Ϣ"); 
	cmd[TRYMOVE_C] = CMD ( BIND<string, char*>()(2, "trymove", "tm"), 
		"[pos/numb][command]", 
		"...", 
		"����һ������ numb ����ִ��һ������"); 
	cmd[ZOBRIST_C] = CMD ( BIND<string, char*>()(2, "zobrist", "zo"), 
		"[numb]", 
		"...", 
		"���յ� numb �� ZOBRIST ����ʾ�����߲�"); 
	cmd[PEMIS_C] = CMD ( BIND<string, char*>()(2, "pemis", "pe"), 
		"[numb]", 
		"...", 
		"���յ� numb �� PEMIS ����ʾ�����߲�"); 
	cmd[SAVE_C] = CMD ( BIND<string, char*>()(2, "save", "sa"), 
		"[ ]", 
		"...", 
		"���浱ǰ���Ϊ���ļ�"); 
	cmd[SAVE2SGF_C] = CMD ( BIND<string, char*>()(2, "save2sgf", "ss"), 
		"[ ]", 
		"...", 
		"���浱ǰ���Ϊ SGF �ļ�"); 
	cmd[BACK_SAVE2TXT_C] = CMD ( BIND<string, char*>()(3, "-save2txt", "-s", "-st"), 
		"[ ]", 
		"...", 
		"����������浱ǰ������"); 
}

int COMMAND::which(string s){
	for (int i = 0; i<CMD_SIZE; ++i)
		for (int j = 0; j<cmd[i].names.size(); ++j){
			if (cmd[i].names[j] == s)
				return i; 
		}
	return CMD_SIZE; 
}

bool COMMAND::iscommand_(VS vs) {
	return which(vs[0]) <= EXIT_C; 
}

bool COMMAND::iscommand_state_(VS vs) {
	return which(vs[0]) > EXIT_C && which(vs[0]) < CMD_SIZE; 
}

void COMMAND::help(int i){
	string s; 
	if (i < CMD_SIZE) {
		for (int j = 0; j<cmd[i].names.size (); ++j){
			s += cmd[i].names[j] + " "; 
			if (j != cmd[i].names.size()-1)
				s += "/"; 
		}
		cout<< setw(SETW) << std::left << s; 
		if (cmd[i].format != "[ ]"){
			cout<< cmd[i].format << endl; 
			cout<< setw(SETW) <<"" << cmd[i].chinese << endl; 
		}
		else
			cout<< cmd[i].chinese << endl; 
	}
	else 
		for (int n = 0; n<CMD_SIZE; ++n)
			help(n); 
}

/* prompt() */

void prompt(const GO& go, MODE_PROMPT mode){
	GO g; 
	switch (mode){
		case BEGINGAME:
			prompt(GO(), BITSET); 
			cout<<endl;
			cout<<go; 
			break; 
		case PREMOVE_MAN:
			cout<< endl
				<<(go.turn() == BLACK ? "��":"��")
				<<" History: "<<go .gethistory() << endl
				<<">  "; 
			break; 
		case PREMOVE_PC:
			cout<<endl
				<<(go.turn() == BLACK ? "��":"��")
				<<" History: "<<go .gethistory() << endl 
				<<">  " <<endl; 
			break; 
		case AFTERMOVE_MAN:
			cout
				//<< "By human: " <<go.getlastpos() << endl 
				<<go; 
			break; 
		case AFTERMOVE_PC:
			cout
				//<< "By computer: " <<go.getlastpos() << endl
				<<go; 
			break; 
		case ENDGAME:
			cout<< "End game !"<< endl; 
			break; 
		case BITSET:
			if (BS != 19) {
				cout<<"BS != 19, can't prompt by BITSET mode !"<< endl; 
				return; 
			}
			g.xx.r [ 0] = (bitset<32>(string("0000000000000110000"))).to_ulong (); 
			g.xx.r [ 1] = (bitset<32>(string("0000000000001110000"))).to_ulong (); 
			g.xx.r [ 2] = (bitset<32>(string("0000000000000110000"))).to_ulong (); 
			g.xx.r [ 3] = (bitset<32>(string("0000110000010110000"))).to_ulong (); 
			g.xx.r [ 4] = (bitset<32>(string("0000111000010111100"))).to_ulong (); 
			g.xx.r [ 5] = (bitset<32>(string("0000011000011111000"))).to_ulong (); 
			g.xx.r [ 6] = (bitset<32>(string("0000011000111110000"))).to_ulong (); 
			g.xx.r [ 7] = (bitset<32>(string("0000111101111110000"))).to_ulong (); 
			g.xx.r [ 8] = (bitset<32>(string("0001111001110110000"))).to_ulong (); 
			g.xx.r [ 9] = (bitset<32>(string("0001111100011110000"))).to_ulong (); 
			g.xx.r [10] = (bitset<32>(string("0000011000011110000"))).to_ulong (); 
			g.xx.r [11] = (bitset<32>(string("0000111000010110110"))).to_ulong (); 
			g.xx.r [12] = (bitset<32>(string("0001111000001111000"))).to_ulong (); 
			g.xx.r [13] = (bitset<32>(string("0011011000111100000"))).to_ulong (); 
			g.xx.r [14] = (bitset<32>(string("0110011001100000100"))).to_ulong (); 
			g.xx.r [15] = (bitset<32>(string("0000011111000001110"))).to_ulong (); 
			g.xx.r [16] = (bitset<32>(string("0000010000010000110"))).to_ulong (); 
			g.xx.r [17] = (bitset<32>(string("0000010001100001100"))).to_ulong (); 
			g.xx.r [18] = (bitset<32>(string("0000000000000000000"))).to_ulong (); 
			cout << g ; 
			break; 
		default:; 
	}
}
