#ifndef _TEST
#define _TEST

#include "infoboard.h"
#include "brain.h"
#include "nn.h"
#include "learn.h"
#include "tsumego.h"
#include "fir.h"
#include "inout.h"

/* class TREESEARCH */
class TREESEARCH:public SEARCH < int, NODE, NODE *, bool >
{
  vector < NODE * >findpath (NODE * p, bool)
  {
    return p->sons;
  }
  bool isleaf (NODE * p, bool)
  {
    return p->isleaf;
  }
  int leafvalue (NODE * p, bool)
  {
    return p->value;
  }
  NODE *findson (NODE * fatherp, NODE * sonp)
  {
    return sonp;
  }
  int upbound ()
  {
    return INT_MAX;
  }
  int downbound ()
  {
    return INT_MIN;
  }
  bool ismax (NODE * p, bool)
  {
    return p->ismax;
  }
  bool control (NODE * p, bool)
  {
    return 0;
  }
  int recordpath (int i, NODE *)
  {
    return i;
  }
};


/* NEWPOS */
class NEWPOS:public POS
{
public:
  int i;
  void hash ()
  {
  };
};


//尽管现在已申明，但不会影响原代码，会自动脱壳
NEWPOS make_pair (int a, unsigned b);

/* class TEST */
class TEST
{
public:
  int &add100 (int &a) const;
  int minus5 (int &a) const;
};


/* global function */
void testtree ();
void testnewpos ();
void testc ();
void testcpp ();
void teststl ();
void testsize ();
void testload ();
void testrand ();
void test_array_if ();
void testindex8 ();
void testtransptable ();

#endif
