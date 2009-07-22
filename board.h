#ifndef _BOARD
#define _BOARD

#include "bitboard.h"
class GO
{
public:
  BITB xx;
  BITB oo;
protected:
  BITB kill;
  POS hotko;
  POS lastpos;
  COLOR lastclr;
  int history;
public:
  GO ();
  GO (const BITB & x, const BITB & o, const BITB & d = NULL_BB, const POS & k = NULL_POS, const POS & l = NULL_POS, COLOR c = WHITE, int h = 0);
  GO (const PBB & pbb, const BITB & d = NULL_BB, const POS & k = NULL_POS, const POS & l = NULL_POS, COLOR c = WHITE, int h = 0);
  GO (const GO & go);
public:
  /*inline */ BITB getbb (COLOR) const;
  /*inline */ BITB getkill () const;
  /*inline */ POS gethotko () const;
  /*inline */ POS getlastpos () const;
  /*inline */ COLOR getlastclr () const;
  /*inline */ int gethistory () const;
  /*inline */ COLOR color (const POS &) const;
  /*inline */ COLOR turn () const;
  /*
     GO           snap() const; 
     void restore(const GO&); 
   */
  void random (MODE_RANDOMBOARD mode = LEGALIZE);
  void add (const POS &, COLOR);
  void legalize ();
  BLOCK expand (const BLOCK & realblk, COLOR clr) const;
  BITB libertysite (const POS &) const;
  int liberty_ (const POS &) const;
  BITB enemysite (const BLOCK & realblk, COLOR clr) const;
  BITB enemy_ (const BLOCK & realblk, COLOR clr) const;
  bool moveable (const POS &, COLOR) const;
  bool moveable_ (const POS &, COLOR) const;
  bool move (const POS &, COLOR);
  bool move_ (const POS &, COLOR);
  bool move__ (const POS &, COLOR);
  bool endgame () const;
  POS genmove () const;
  void init ();
  void update ();
  int delta () const;
  PUU pemis3232 (const BITB & patmask) const;
  ULL pemis64 (const BITB & patmask) const;
  GO transpose () const;
  GO rotate (int angle) const;
public:
  GO operator& (const GO & go) const;
  GO operator| (const GO & go) const;
  GO operator^ (const GO & go) const;
  COLOR operator[] (const POS & pos) const;
  GO operator() (const BITB & mark, const POS & markp = NULL_POS) const;
  friend ostream & operator<< (ostream &, const GO &);
};


#endif
