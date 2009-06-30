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

		// numb 是开始生成时分配，cout 时已生成完毕。
		// 逻辑结构与下面各搜索算法一样，所以 cout 顺序一样。
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
// alpha, beta 的意义？
// alpha 理解为上层 max 结点目前发现的最好值，
// beta 理解为上层 min 结点目前发现的最好值，
// 作为根结点的返回值才能认为是真实值。
// 因为总应保持 alpha <= beta 所以可抽象为真实值的界值。
// 在 negamax() 中则理解为在奇数次反转和
// 偶数次反转层上发现的最好值。
// 搜索过程是在兄弟 min 结点上更新 alpha ，
// 在兄弟 max 结点上更新 beta 。

// 函数的意义？
// 以alpha, beta为界剪枝（并改值后）后得到的新树的最好值。
// alphabeta_minmax() 不仅剪枝，
// 而且将剪枝前的结点值修改为界值 alpha 或 beta；
// 而再回传alpha, beta则可以看作是新树的真实值了，
// 当然对原树而言是伪值。
// failsoft_minmax() 则仅仅剪枝，
// 返回值则可以看作是剪枝后的新树的真实值。
// 只有如此理解才可自然地解释为何算法中可以递归调用。

// 为什么如此上传伪值不会影响最终正确值？
// 因为在同层即使存在多个伪值最终也只会上传一个，
// 而这个伪值即使多次上传后也会因为越界而被“抹平”。

// 为什么界值合适时结果一定正确？
// 界值是自上而下传递，最终应该上传的那个结点的同层的初界一定包含它，
// 否则则确实是因为有比它更佳的值已在上层发现。

// 为什么界值不合适时，failsoft_minmax() 能反映真实值在哪侧？
// 目的值没有被上传一定是因为所在子树被剪掉，
// 它处在此子树哪层没有关系，因为它的值应可传到与引发剪枝的结点的同层
// 而引发剪枝的结点值一定是越界值( 导致 alpha >= beta )，
// 而目的值一定更越界，因为目的值本应更优，
// 所以它们也一定是处在界外同侧。
// 至于哪个大哪个小就不一定，因为更优可能是更大也可能是更小。
// 所以如果目的值总处在引发剪枝的位置，
// 它还是能被正确回传，但无法断定它就是最终正确值。

// 为什么以同样的界值搜索同样的树，
// failsoft_minmax()和failsoft_negamax() 表现不一 ?
// 如果头结点为 min 结点，则传入的(alpha, beta)相当于( -beta, -alpha)
// 而头结点为 max 结点时剪枝过程应该完全一样！
//_______________________________________________________________________

		// 逻辑最简单之极大极小搜索算法，结果可作为标准。
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
							// alpha, beta 确能顺传，因为界外值
							// 传到相应上层的时候一定会被"抹平"
							// 所以可作为界值深度优先向下传递
			VALUE v = alphabeta_minmax ( p->sons[i], alpha, beta ); 
			if (v > alpha){
				alpha = v; 
				if (alpha >= beta){
					break; // 剪枝之后返回越界的 alpha 还是 beta 
							// 并不影响最终值，因为总会被 beta "抹平"
				}
			}
		}
		p->value = alpha; // 可能高于原结点真实值，但不影响最终值，
							// 因为当为非剪枝返回，真实值会被 alpha "抹平"
							// 当为剪枝返回，则此越界的alpha值
							// 会被 beta "抹平"
							// 这个赋值可看作是对子层一个或全部
							// 结点改值后给此结点赋真实值。
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
		p->value = better; // 返回已发现的较好值，不一定是真实值。
							// 但若把被剪掉的子树看作根本不存在，
							// 则可认为是真实值。
							// 所谓新树的真实值，原树的伪值。
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

		// 翻转了 min 叶结点的值
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

		// 翻转了 min 叶结点的值, 无返回值形式
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
		
		// 翻转了 min 叶结点的值
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

		// 翻转了 min 叶结点的值
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

		// 翻转了 min 叶结点的值
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
		// v == alpha + 1 时确也需要重新搜索，因为可能是剪枝而来
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

		// 翻转了 min 叶结点的值
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

/* _______________________________ 测试区 _______________________________ */

void TEST_S::timefunc (string which) {
	 RANDER  r; 
}
