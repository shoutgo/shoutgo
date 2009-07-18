#include "go.h"
#include "inout.h"

/* class COMMAND */

// 简化规则：双字各取前一字母，单字取前两字母。
COMMAND::COMMAND() {
	cmd[HELP_C] = CMD ( BIND<string, char*>()(2, "help", "he"), 
		"[command]", 
		"...", 
		"显示全部或某个命令的帮助信息"); 
	cmd[VERSION_C] = CMD ( BIND<string, char*>()(2, "version", "ve" ), 
		"[ ]", 
		"...", 
		"显示版本信息"); 
	cmd[CMD_C] = CMD ( BIND<string, char*>()(2, "cmd", "cm"), 
		"[ ]", 
		"...", 
		"执行 CMD.EXE 程序"); 
	cmd[THESIS_C] = CMD ( BIND<string, char*>()(2, "thesis", "th"), 
		"[ ]", 
		"...", 
		"执行一系列定制命令"); 
	cmd[FONT_C] = CMD ( BIND<string, char*>()(2, "font", "fo"), 
		"[ ]", 
		"...", 
		"执行 FONT.EXE 程序"); 
	cmd[SAVECODE_C] = CMD ( BIND<string, char*>()(2, "savecode", "sc"), 
		"[ ]", 
		"...", 
		"保存本程序源代码"); 
	cmd[PLAY_C] = CMD ( BIND<string, char*>()(2, "play", "pl"), 
		"[MAN_PC, PC_PC, MAN_MAN]", 
		"...", 
		"按指定方式下棋"); 
	cmd[FIR_C] = CMD ( BIND<string, char*>()(2, "fir", "fi"), 
		"[MAN_PC, PC_PC, MAN_MAN]", 
		"...", 
		"按指定方式下五子棋"); 
	cmd[REVIEW_C] = CMD ( BIND<string, char*>()(2, "review", "re"), 
		"[ ]", 
		"...", 
		"读入 SGF 文件并演示"); 
	cmd[SGFTREE_C] = CMD ( BIND<string, char*>()(2, "sgftree", "st"), 
		"[ ]", 
		"...", 
		"以树方式读入 SGF 文件并演示"); 
	cmd[INPUTPAT_C] = CMD ( BIND<string, char*>()(2, "inputpat", "ip"), 
		"[ ]", 
		"...", 
		"输入模式"); 
	cmd[SIZE_C] = CMD ( BIND<string, char*>()(2, "size", "si"), 
		"[ ]", 
		"...", 
		"显示各类的大小"); 
	//cmd[NEWLINE_C] = CMD ( BIND<string, char*>()(3, "newline", "nl", ";"), 
	//	"[ ]", 
	//	"...", 
	//	"换行"); 
	cmd[EXIT_C] = CMD ( BIND<string, char*>()(2, "exit", "ex"), 
		"[ ]", 
		"...", 
		"退出本程序"); 

	cmd[PASS_C] = CMD ( BIND<string, char*>()(1, "pass"), 
		"[ ]", 
		"...", 
		"弃权一手"); 
	cmd[END_C] = CMD ( BIND<string, char*>()(1, "end"), 
		"[ ]", 
		"...", 
		"结束当前命令或棋局"); 
	cmd[HISTORY_C] = CMD ( BIND<string, char*>()(2, "history", "hi"), 
		"[ ]", 
		"...", 
		"显示历史值"); 
	cmd[SHOWBOARD_C] = CMD ( BIND<string, char*>()(2, "showboard", "sb"), 
		"[ ]", 
		"...", 
		"显示棋盘"); 
	cmd[DELTAMASSMAPS_C] = CMD ( BIND<string, char*>()(2, "deltamassmaps", "dm"), 
		"[numb]", 
		"...", 
		"显示 numb 级棋团的变化区域"); 
	cmd[MASSMAPS_C] = CMD ( BIND<string, char*>()(2, "massmaps", "mm"), 
		"[numb]", 
		"...", 
		"显示 numb 级棋团"); 
	cmd[CLUSTER_C] = CMD ( BIND<string, char*>()(2, "cluster", "cl"), 
		"[ ]", 
		"...", 
		"显示棋群"); 
	cmd[POTENTIAL_C] = CMD ( BIND<string, char*>()(2, "potential", "po"), 
		"[ ]", 
		"...", 
		"显示势"); 
	cmd[PUBLICAREA_C] = CMD ( BIND<string, char*>()(2, "publicarea", "pa"), 
		"[ ]", 
		"...", 
		"显示公共区"); 
	cmd[ALIVE_C] = CMD ( BIND<string, char*>()(2, "alive", "al"), 
		"[ ]", 
		"...", 
		"显示活棋群"); 
	cmd[DEAD_C] = CMD ( BIND<string, char*>()(2, "dead", "de"), 
		"[ ]", 
		"...", 
		"显示死棋群"); 
	cmd[UNCLEAR_C] = CMD ( BIND<string, char*>()(2, "unclear", "un"), 
		"[ ]", 
		"...", 
		"显示死活不明的棋群"); 
	cmd[LAND_C] = CMD ( BIND<string, char*>()(2, "land", "la"), 
		"[ ]", 
		"...", 
		"显示棋群的“陆地”"); 
	cmd[SEA_C] = CMD ( BIND<string, char*>()(2, "sea", "se"), 
		"[ ]", 
		"...", 
		"显示棋群的“海洋”"); 
	cmd[AIR_C] = CMD ( BIND<string, char*>()(2, "air", "ai"), 
		"[ ]", 
		"...", 
		"显示棋群的“领空”"); 
	cmd[SCAN_C] = CMD ( BIND<string, char*>()(2, "scan", "sc"), 
		"[ ]", 
		"...", 
		"扫描当前棋局的信息"); 
	cmd[TRYMOVE_C] = CMD ( BIND<string, char*>()(2, "trymove", "tm"), 
		"[pos/numb][command]", 
		"...", 
		"试下一步或倒退 numb 步并执行一个命令"); 
	cmd[ZOBRIST_C] = CMD ( BIND<string, char*>()(2, "zobrist", "zo"), 
		"[numb]", 
		"...", 
		"参照第 numb 个 ZOBRIST 库显示可能走步"); 
	cmd[PEMIS_C] = CMD ( BIND<string, char*>()(2, "pemis", "pe"), 
		"[numb]", 
		"...", 
		"参照第 numb 个 PEMIS 库显示可能走步"); 
	cmd[SAVE_C] = CMD ( BIND<string, char*>()(2, "save", "sa"), 
		"[ ]", 
		"...", 
		"保存当前棋局为类文件"); 
	cmd[SAVE2SGF_C] = CMD ( BIND<string, char*>()(2, "save2sgf", "ss"), 
		"[ ]", 
		"...", 
		"保存当前棋局为 SGF 文件"); 
	cmd[BACK_SAVE2TXT_C] = CMD ( BIND<string, char*>()(3, "-save2txt", "-s", "-st"), 
		"[ ]", 
		"...", 
		"后置命令，保存当前输出结果"); 
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
				<<(go.turn() == BLACK ? "●":"○")
				<<" History: "<<go .gethistory() << endl
				<<">  "; 
			break; 
		case PREMOVE_PC:
			cout<<endl
				<<(go.turn() == BLACK ? "●":"○")
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
