#include "nn.h"  
#include "inout.h"

// 注意 \\matlab\\work\\linearmodal.m 里有写好了的验证算法
VVF	NN::producedata(int m, int n){
	float yrange = 5000.1; 
	float xrange = 100; 
	//2009//datayx.assign (m, 0); 
	srand((unsigned)time(0)); 
	for (int i = 0; i<m; ++i){
		for (int j = 0; j<n; ++j)
			if (j == 0) 
				datayx[i].push_back (fmod((float)rand(), yrange)); 
			else
				datayx[i].push_back (fmod((float)rand(), xrange)); 
	}
	return datayx; 
}

VF			NN::linear_genetic(){
	___.tic("linear");
	int m = 20;
	int n = 1+6; 
	int POPU = 50; 
	int ALLPOPU = 500; 
	int MUTATION_BASE = 10; 
	int ITERATE_MAX = 30; 
	VVF w; 
	VVF tmpw; 
	//2009//w.assign (ALLPOPU, 0); 
	//2009//tmpw.assign (ALLPOPU, 0); 
	VF tu; 
	VF tv; 
	float bias; 
	producedata(m, n);
	___COUT1(datayx); 
	INOUT().save2txt(datayx, "datayx.txt"); //
	srand((unsigned)time(0)); 
	w = randvvf(ALLPOPU, n); 
    for (int iterate = 0; ; ++iterate){						
		for (int times = 0; times<(ALLPOPU/2); ++times){
			tu = w[POPU-1-randsqrt(POPU)]; 
			tv = w[POPU-1-randsqrt(POPU)]; 
			for (int i = 1; i<n; ++i){
				if (prob(1, 2)) 
					swap(tu[i], tv[i]); 
				if (prob(1, MUTATION_BASE))
					tu[i] = randf(); 
				if (prob(1, MUTATION_BASE))
					tv[i] = randf(); 
			}											
			tu[0] = linearmodal(tu); 
			tv[0] = linearmodal(tv); 
			tmpw[times] = tu; 
			tmpw[times+ALLPOPU/2] = tv; 
		}												
		___ASSERT(w.size () == tmpw.size () && w[0].size () == tmpw[0].size ()); //
		___ASSERT(w != tmpw); //
		partial_sort(tmpw.begin (), tmpw.begin ()+POPU, tmpw.end()); 
		swap(w, tmpw); 
		bias = 0; 
		for (int i = 0; i<POPU; ++i)
			bias += w[i][0]; 
		if (bias<100000)	break; 
		if (iterate>ITERATE_MAX) break; 
														//cout<<"linear: "<<iterate
														//	<<"  bias: "<<(bias/POPU)
														//	<<"  best: "<<w[0][0]<< endl; 
														___ESC;
	}													
	___.toc();
	return w[0]; 
}

float					NN::linearmodal(VF w){
	___ASSERT(w.size () == (datayx[0].size ())); 
	float viariance = 0, f; 
	for (int i = 0; i<datayx.size (); ++i){
		f = 0; 
		for (int j = 1; j<w.size (); ++j)
			f += w[j]*datayx[i][j]; 
		viariance += pow(datayx[i][0]-f, 2); 
	}
	return viariance/datayx.size(); // 
}

void					NN::perception_genetic(){
	___.tic("perception");
	int m = 20;
	int n = 1+6; 
	int POPU = 50; 
	int ALLPOPU = 500; 
	int CROSS_BASE = 2; 
	int MUTATION_BASE = 10; 
	int ITERATE_MAX = 30; 
	int h1 = 8; 
	int h2 = 3; 
	vector<N3WEIGHT>	w(ALLPOPU); 
	vector<N3WEIGHT>	tmpw(ALLPOPU); 
	N3WEIGHT			tu; 
	N3WEIGHT			tv; 
	float bias; 
	//producedata(m, n); 
	___COUT1(datayx); 
	//INOUT().save2txt(datayx, "datayx.txt"); //
	srand((unsigned)time(0)); 
	for(int i = 0; i<ALLPOPU; ++i){
		w[i].w1 = randvvf(h1, n-1+1);
		w[i].w2 = randvvf(h2, h1+1); 
		w[i].w3 = randvf(h2 + 1); 
	}															
    for (int iterate = 0; ; ++iterate){								
		for (int times = 0; times<ALLPOPU/2; ++times){				
			tu = w[POPU-1-randsqrt(POPU)]; 
			tv = w[POPU-1-randsqrt(POPU)]; 
			for(int i = 0; i<h1; ++i)
				for (int j = 0; j<n; ++j)
					if (prob(1, CROSS_BASE))				
						swap(tu.w1[i][j], tv.w1[i][j]); 
			for(int i = 0; i<h2; ++i)
				for (int j = 0; j<h1+1; ++j)
					if (prob(1, CROSS_BASE)) 
						swap(tu.w2[i][j], tv.w2[i][j]); 
			for (int i = 0; i<h2+1; ++i)
				if (prob(1, CROSS_BASE)) 
					swap(tu.w3[i], tv.w3[i]); //--
			for(int i = 0; i<h1; ++i)
				for (int j = 0; j<n; ++j)
					if (prob(1, MUTATION_BASE))
						tu.w1[i][j] = randf(); 
			for(int i = 0; i<h2; ++i)
				for (int j = 0; j<h1+1; ++j)
					if (prob(1, MUTATION_BASE)) 
						tu.w2[i][j] = randf(); 
			for (int i = 0; i<h2+1; ++i)
				if (prob(1, MUTATION_BASE)) 
					tu.w3[i] = randf(); //--
			for(int i = 0; i<h1; ++i)
				for (int j = 0; j<n; ++j)
					if (prob(1, MUTATION_BASE)) 
						tv.w1[i][j] = randf(); 
			for(int i = 0; i<h2; ++i)
				for (int j = 0; j<h1+1; ++j)
					if (prob(1, MUTATION_BASE)) 
						tv.w1[i][j] = randf(); 
			for (int i = 0; i<h2+1; ++i)
				if (prob(1, MUTATION_BASE)) 
					tv.w3[i] = randf(); 
			tmpw[times] = perception(tu); 
			tmpw[times+ALLPOPU/2] = perception(tv); 
		}														
		___ASSERT(w.size () == tmpw.size ()); //
		___ASSERT(w != tmpw); //
		//tmpw.back () = w[0]; 
		partial_sort(tmpw.begin (), tmpw.begin ()+POPU, tmpw.end()); 
		swap(w, tmpw); 
		bias = 0; 
		for (int i = 0; i<POPU; ++i)
			bias += w[i].fitness; 
		if (bias<100000)	break; 
		if (iterate>ITERATE_MAX) break; 
														//cout<<"perception: "<<iterate
														//	<<"  bias: "<<(bias/POPU)
														//	<<"  best: "<<w[0].fitness << endl; 
														___ESC;
	}								
	___.toc();
}

N3WEIGHT				NN::perception(N3WEIGHT w){
	VVF w1 = w.w1; 
	VVF w2 = w.w2; 
	VF	w3 = w.w3; 
	___ASSERT(datayx[0].size () == w1[0].size ()); 
	___ASSERT(w1.size () == w2[0].size ()-1); 
	___ASSERT(w2.size () == w3.size ()-1); 
	int h1 = w1.size (); 
	int h2 = w2.size (); 
	VF hide1(h1); 
    VF hide2(h2); 
	float viariance = 0, output; 
    for (int i = 0; i<datayx.size (); ++i){					
		for (int j = 0; j<h1; ++j){						
			hide1[j] = inner_product(datayx[j].begin ()+1, datayx[j].end (), w1[j].begin (), -w1[j].back ()); 
			hide1[j] = 1/(1+exp(-hide1[j])); 
		}											
		for (int k = 0; k<h2; ++k){
            hide2[k] = inner_product(hide1.begin (), hide1.end (), w2[k].begin (), -w2[k].back ()); 
			hide2[k] = 1/(1+exp(-hide2[k])); 
		}														
		output = inner_product(hide2.begin (), hide2.end (), w3.begin(), -w3.back ()); 
		viariance += pow(datayx[i][0]-output, 2); 
	}
	w.fitness = viariance/datayx.size(); 
	return w; //
}

/* _______________________________ 测试区 _______________________________ */

void TEST_NN::timefunc (string which) {
	RANDER  r; 
}
