#ifndef _SEARCH
#define _SEARCH

#include "define.h"

//#include "infoboard.h"

/* class NODE, TREE */

// 注意 class TREE 的代码不要改，保留。
struct NODE
{
  int numb;
  int depth;
  bool ismax;
  bool isleaf;
  bool flip;
  VALUE value;
  NODE *father;
  vector < NODE * >sons;
public:
  NODE ():value (0), flip (0)
  {
  }
  ~NODE ()
  {
  }
  friend ostream & operator<< (ostream & os, const NODE n);
};

class TREE
{
public:
  const static int NODE_MAX = 500000;
  const static int SON_MAX = 5000;
  const static int DEPTH_MAX = 600;
  const static int VALUE_MAX = 500000;
  const static int VALUE_MIN = -400000;

  /*
     const static int     NODE_MAX = 50; 
     const static int     SON_MAX = 5; 
     const static int     DEPTH_MAX = 6; 
     const static int     VALUE_MAX = 100; 
     const static int     VALUE_MIN = -50; 
   */
  int nodes;
  int visited;
public:
  NODE * root ();
  void gen (NODE * p);
  VALUE minmax (NODE * p);
  VALUE alphabeta_minmax (NODE * p, VALUE alpha, VALUE beta);
  VALUE failsoft_minmax (NODE * p, VALUE alpha, VALUE beta);
  VALUE negamax (NODE * p);
  void negamax2 (NODE * p);
  VALUE alphabeta_negamax (NODE * p, VALUE alpha, VALUE beta);
  VALUE failsoft_negamax (NODE * p, VALUE alpha, VALUE beta);
  VALUE negascout (NODE * p, VALUE alpha, VALUE beta);

  // memory-enhanced test driver with node n and value f
  VALUE mtdf (NODE * p, VALUE guess = 0);
};


/* class SEARCH */
template < class T_RESULT, class T_NODE, class T_PATH, class T_CONTROL = bool > class SEARCH
{
public:
  int numb;
  SEARCH ():numb (0)
  {
  }
public:
  T_RESULT minmax (T_NODE * node, T_CONTROL cntr = T_CONTROL ())
  {
    ++numb;
    if (isleaf (node, cntr))
      {

	//cout<< *node << endl; 
	return leafvalue (node, cntr);
      }
    vector < T_PATH > paths = findpath (node, cntr);
    bool maxnode = ismax (node, cntr);
    T_RESULT tmp, better = maxnode ? downbound () : upbound ();
    for (int i = 0; i < paths.size (); ++i)
      {
	T_NODE *sonp = findson (node, paths[i]);
	tmp = minmax (sonp, control (node, cntr));

	//better = maxnode ? max(tmp, better):min(tmp, better); 
	if (maxnode)
	  {
	    if (better < tmp)
	      {
		better = tmp;
		better = recordpath (better, paths[i]);
	      }
	  }

	else if (tmp < better)
	  {
	    better = tmp;
	    better = recordpath (better, paths[i]);
	  }

	//  加deletefromsons()集中delete. 可不删，按指针登记到一 map 以供查询？
	if (sonp != node)
	  {

	    // node->sons.clear(); // node.sons 还没清空，
	    delete sonp;
	  }
      }

    //cout<< *node << endl; 
    return better;
  }
  virtual vector < T_PATH > findpath (T_NODE *, T_CONTROL = T_CONTROL ()) = 0;
  virtual bool isleaf (T_NODE *, T_CONTROL) = 0;
  virtual T_RESULT leafvalue (T_NODE *, T_CONTROL) = 0;
  virtual T_NODE *findson (T_NODE *, T_PATH) = 0;
  virtual T_RESULT upbound () = 0;
  virtual T_RESULT downbound () = 0;
  virtual bool ismax (T_NODE *, T_CONTROL) = 0;
  virtual T_CONTROL control (T_NODE *, T_CONTROL) = 0;
  virtual T_RESULT recordpath (T_RESULT, T_PATH) = 0;
};

#endif
