#include "stdafx.h"
#include "search.h"							
#include "inout.h"

NODE* TREE::root(){
	NODE* p = new NODE; 
	nodes = 0; 
	visited = 0;
	p->numb = nodes; 
	p->ismax = prob(1, 2); 
	p->depth = 0; 
	p->isleaf = 0; 
	p->father = 0; 
    gen(p); 
	return p; 
}

		// numb �ǿ�ʼ����ʱ���䣬cout ʱ��������ϡ�
		// �߼��ṹ������������㷨һ�������� cout ˳��һ����
void TREE::gen(NODE* p){		
	if (p->depth == DEPTH_MAX-1 || nodes >= NODE_MAX-1) {
		p->isleaf = 1; 
		p->value = randu(VALUE_MAX-VALUE_MIN)+VALUE_MIN; 
		//-//cout<<*p<< endl; 
		return; 
	}
	for (int i = 0; i<SON_MAX && nodes<NODE_MAX-1; ++i){
		if (prob(1, 2))
			continue; 
		NODE* s = new NODE; 
		++nodes; 
		s->numb = nodes; 
		s->ismax = !(p->ismax); 
		s->depth = p->depth +1; 
		s->isleaf = 0; 
		s->father = p; 
		p->sons.push_back(s); 
		gen(s); 
	}
	if (p->sons.empty()) {
		p->isleaf = 1; 
		p->value = randu(VALUE_MAX-VALUE_MIN) + VALUE_MIN; 
	}
	//-//cout<<*p<< endl; 
}

ostream& operator<< (ostream& os, const NODE n){
	string s(n.depth, '\t'); 
	cout<< s
		<< "[" << n.numb << "]"
		//<< "_" << n.depth
		<< "_" << (n.ismax ? "max" : "min")
		//<< "_(" << (n.father ? (n.father->numb) : 0) <<")"
		<< " " << n.value 
		//<< " " << (n.flip ? -n.value : n.value )
		<< " " << (n.isleaf ? "leaf " : ""); 
	//if (n.numb == 0)
	//	cout << (n.ismax ? "max ":"min "); 
	return os; 
}

//_______________________________________________________________________
//
// alpha, beta �����壿
// alpha ���Ϊ�ϲ� max ���Ŀǰ���ֵ����ֵ��
// beta ���Ϊ�ϲ� min ���Ŀǰ���ֵ����ֵ��
// ��Ϊ�����ķ���ֵ������Ϊ����ʵֵ��
// ��Ϊ��Ӧ���� alpha <= beta ���Կɳ���Ϊ��ʵֵ�Ľ�ֵ��
// �� negamax() �������Ϊ�������η�ת��
// ż���η�ת���Ϸ��ֵ����ֵ��
// �������������ֵ� min ����ϸ��� alpha ��
// ���ֵ� max ����ϸ��� beta ��

// ���������壿
// ��alpha, betaΪ���֦������ֵ�󣩺�õ������������ֵ��
// alphabeta_minmax() ������֦��
// ���ҽ���֦ǰ�Ľ��ֵ�޸�Ϊ��ֵ alpha �� beta��
// ���ٻش�alpha, beta����Կ�������������ʵֵ�ˣ�
// ��Ȼ��ԭ��������αֵ��
// failsoft_minmax() �������֦��
// ����ֵ����Կ����Ǽ�֦�����������ʵֵ��
// ֻ��������ſ���Ȼ�ؽ���Ϊ���㷨�п��Եݹ���á�

// Ϊʲô����ϴ�αֵ����Ӱ��������ȷֵ��
// ��Ϊ��ͬ�㼴ʹ���ڶ��αֵ����Ҳֻ���ϴ�һ����
// �����αֵ��ʹ����ϴ���Ҳ����ΪԽ�������Ĩƽ����

// Ϊʲô��ֵ����ʱ���һ����ȷ��
// ��ֵ�����϶��´��ݣ�����Ӧ���ϴ����Ǹ�����ͬ��ĳ���һ����������
// ������ȷʵ����Ϊ�б������ѵ�ֵ�����ϲ㷢�֡�

// Ϊʲô��ֵ������ʱ��failsoft_minmax() �ܷ�ӳ��ʵֵ���Ĳࣿ
// Ŀ��ֵû�б��ϴ�һ������Ϊ����������������
// �����ڴ������Ĳ�û�й�ϵ����Ϊ����ֵӦ�ɴ�����������֦�Ľ���ͬ��
// ��������֦�Ľ��ֵһ����Խ��ֵ( ���� alpha >= beta )��
// ��Ŀ��ֵһ����Խ�磬��ΪĿ��ֵ��Ӧ���ţ�
// ��������Ҳһ���Ǵ��ڽ���ͬ�ࡣ
// �����ĸ����ĸ�С�Ͳ�һ������Ϊ���ſ����Ǹ���Ҳ�����Ǹ�С��
// �������Ŀ��ֵ�ܴ���������֦��λ�ã�
// �������ܱ���ȷ�ش������޷��϶�������������ȷֵ��

// Ϊʲô��ͬ���Ľ�ֵ����ͬ��������
// failsoft_minmax()��failsoft_negamax() ���ֲ�һ ?
// ���ͷ���Ϊ min ��㣬�����(alpha, beta)�൱��( -beta, -alpha)
// ��ͷ���Ϊ max ���ʱ��֦����Ӧ����ȫһ����
//_______________________________________________________________________

		// �߼����֮����С�����㷨���������Ϊ��׼��
VALUE TREE::minmax(NODE* p){
	++ visited;
	if (p->isleaf){
		//-//cout<< *p<< endl; 
		return (p->value); 
	}
	VALUE v, better = (p->ismax) ? INT_MIN : INT_MAX; 
	for (int i = 0; i < p->sons.size(); ++i){
		v = minmax ( p->sons[i] ); 
		better = (p->ismax) ? max(better, v)	: min(better, v); 
	}
	p->value = better; 
	//-//cout<< *p<< endl; 
	return p->value; 
}

VALUE TREE::alphabeta_minmax(NODE* p, VALUE alpha, VALUE beta){	
	++ visited;
	VALUE olda = alpha; 
	VALUE oldb = beta; 
	if (p->isleaf){
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<setx(W)<< endl; 
		return (p->value); 
	}
	if (p->ismax ){
		for (int i = 0; i < p->sons.size(); ++i){
							// alpha, beta ȷ��˳������Ϊ����ֵ
							// ������Ӧ�ϲ��ʱ��һ���ᱻ"Ĩƽ"
							// ���Կ���Ϊ��ֵ����������´���
			VALUE v = alphabeta_minmax ( p->sons[i], alpha, beta ); 
			if (v > alpha){
				alpha = v; 
				if (alpha >= beta){
					break; // ��֦֮�󷵻�Խ��� alpha ���� beta 
							// ����Ӱ������ֵ����Ϊ�ܻᱻ beta "Ĩƽ"
				}
			}
		}
		p->value = alpha; // ���ܸ���ԭ�����ʵֵ������Ӱ������ֵ��
							// ��Ϊ��Ϊ�Ǽ�֦���أ���ʵֵ�ᱻ alpha "Ĩƽ"
							// ��Ϊ��֦���أ����Խ���alphaֵ
							// �ᱻ beta "Ĩƽ"
							// �����ֵ�ɿ����Ƕ��Ӳ�һ����ȫ��
							// ����ֵ����˽�㸳��ʵֵ��
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<" "			<<setx(G)<<alpha<<", "<<beta<<setx(W)<< endl; 
		return p->value; 
	}
	else {
		for (int i = 0; i < p->sons.size(); ++i){
			VALUE v = alphabeta_minmax ( p->sons[i], alpha, beta ); 
			if (v < beta){
				beta = v; 
				if (alpha >= beta){
					break; 
				}
			}
		}
		p->value = beta; 
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<" "			<<setx(G)<<alpha<<", "<<beta<<setx(W)<< endl; 
		return p->value; 
	}
}

VALUE TREE::failsoft_minmax(NODE* p, VALUE alpha, VALUE beta){	
	++ visited;
	VALUE olda = alpha; 
	VALUE oldb = beta; 
	if (p->isleaf){
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<setx(W)<< endl; 
		return (p->value); 
	}
	if (p->ismax ){
		VALUE v, better = INT_MIN; 
		for (int i = 0; i < p->sons.size(); ++i){
			v = failsoft_minmax ( p->sons[i], alpha, beta ); 
			better = max(better, v); 
			if (v > alpha){
				alpha = v; 
				if (alpha >= beta){
					break; 
				}
			}
		}
		p->value = better; // �����ѷ��ֵĽϺ�ֵ����һ������ʵֵ��
							// �����ѱ������������������������ڣ�
							// �����Ϊ����ʵֵ��
							// ��ν��������ʵֵ��ԭ����αֵ��
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<" "			<<setx(G)<<alpha<<", "<<beta<<setx(W)<< endl; 
		return p->value; 
	}
	else {
		VALUE v, better = INT_MAX; 
		for (int i = 0; i < p->sons.size(); ++i){
			v = failsoft_minmax ( p->sons[i], alpha, beta ); 
			better = min(better, v); 
			if (v < beta){
				beta = v; 
				if (alpha >= beta){
					break; 
				}
			}
		}
		p->value = better; 
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<" "			<<setx(G)<<alpha<<", "<<beta<<setx(W)<< endl; 
		return p->value; 
	}
}

		// ��ת�� min Ҷ����ֵ
VALUE TREE::negamax(NODE* p){			
	++ visited;
	if (p->isleaf){
		if (p->flip == 0 && ! p->ismax){
			p->flip = 1; 
			p->value *= -1; 
		}
		//-//cout<< *p<< endl; 
		return (p->value); 
	}
	VALUE v, better = INT_MIN; 
	for (int i = 0; i < p->sons.size(); ++i){
		v = -negamax ( p->sons[i] ); 
		better = max(better, v); 
	}
	if (! p->ismax )
		p->flip = 1; 
	p->value = better; 
	//-//cout<< *p<< endl; 
	return p->value; 
}

		// ��ת�� min Ҷ����ֵ, �޷���ֵ��ʽ
void TREE::negamax2(NODE* p){			
	++ visited;
	if (p->isleaf){
		if (p->flip == 0 && ! p->ismax){
			p->flip = 1; 
			p->value *= -1; 
		}
		//-//cout<< *p<< endl; 
		return; 
	}
	VALUE better = INT_MIN; 
	for (int i = 0; i < p->sons.size(); ++i){
		negamax2 ( p->sons[i] ); 
		better = max(better, - p->sons[i]->value ); 
	}
	if (! p->ismax )
		p->flip = 1; 
	p->value = better; 
	//-//cout<< *p<< endl; 
}
		
		// ��ת�� min Ҷ����ֵ
VALUE TREE::alphabeta_negamax(NODE* p, VALUE alpha, VALUE beta){
	++ visited;
	VALUE olda = alpha; 
	VALUE oldb = beta; 
	if (p->isleaf){
		if (p->flip == 0 && ! p->ismax){
			p->flip = 1; 
			p->value *= -1; 
		}
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<setx(W)<< endl; 
		return (p->value); 
	}
	VALUE v; 
	for (int i = 0; i < p->sons.size(); ++i){
		v = -alphabeta_negamax ( p->sons[i], -beta, -alpha ); 
		if (v > alpha){
			alpha = v; 
			if (alpha >= beta)
				break; 
		}
	}
	if (! p->ismax )
		p->flip = 1; 
	p->value = alpha; 
	//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<" "		<<setx(G)<<alpha<<", "<<beta<<setx(W)<< endl; 
	return p->value; 
}

		// ��ת�� min Ҷ����ֵ
VALUE TREE::failsoft_negamax(NODE* p, VALUE alpha, VALUE beta){
	++ visited;
	VALUE olda = alpha; 
	VALUE oldb = beta; 
	if (p->isleaf){
		if (p->flip == 0 && ! p->ismax){
			p->flip = 1; 
			p->value *= -1; 
		}
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<setx(W)<< endl; 
		return (p->value); 
	}
	VALUE v, better = INT_MIN; 
	for (int i = 0; i < p->sons.size(); ++i){
		v = -failsoft_negamax ( p->sons[i], -beta, -alpha ); 
		better = max(v, better); 
		if (v > alpha){
			alpha = v; 
			if (alpha >= beta)
				break; 
		}
	}
	if (! p->ismax )
		p->flip = 1; 
	p->value = better; 
	//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<" "		<<setx(G)<<alpha<<", "<<beta<<setx(W)<< endl; 
	return p->value; 
}

		// ��ת�� min Ҷ����ֵ
VALUE TREE::negascout(NODE* p, VALUE alpha, VALUE beta){
	++ visited;
	VALUE olda = alpha; 
	VALUE oldb = beta; 
	if (p->isleaf){
		if (p->flip == 0 && ! p->ismax){
			p->flip = 1; 
			p->value *= -1; 
		}
		//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<setx(W)<< endl; 
		return (p->value); 
	}
	VALUE v; 
	VALUE better = -negascout(p->sons [0], -beta, -alpha); 
	if (better >= beta)
		return better; 
	else 
		alpha = better; 
	for (int i = 1; i < p->sons.size(); ++i){
		v = - negascout ( p->sons[i], -alpha-1, -alpha); 
		// v == alpha + 1 ʱȷҲ��Ҫ������������Ϊ�����Ǽ�֦����
		if (v<beta && v>alpha)
			v = -negascout(p->sons [i], -beta, -v); 
		better = max(v, better); 
		if (v>alpha){
			alpha = v; 
			if (alpha >= beta)
				break; 
		}
	}
	if (! p->ismax )
		p->flip = 1; 
	p->value = better; 
	//-//cout<< *p <<setx(R|G)<<olda<<", "<<oldb<<" "		<<setx(G)<<alpha<<", "<<beta<<setx(W)<< endl; 
	return p->value; 
}

		// ��ת�� min Ҷ����ֵ
VALUE TREE::mtdf(NODE* p, VALUE guess){
	++ visited;
    VALUE lowbound = INT_MIN; 
	VALUE upbound = INT_MAX; 
	VALUE beta; 
	while (lowbound < upbound){
		if (guess == lowbound)
			beta = guess+1; 
		else 
			beta = guess; 
		guess = failsoft_negamax(p, beta-1, beta); 
		if (guess >= beta)
			lowbound = guess; 
		else 
			upbound = guess; 
	}
	return guess; 
}

/* _______________________________ ������ _______________________________ */

void TEST_S::timefunc (string which) {
	 RANDER  r; 
}
