#ifndef _BITBOARD
#define _BITBOARD

#include "utility.h"
class BITB
{
public:ROW r[BS];
public:BITB (ROW x = 0);
  BITB (ROW a[]);
  BITB (const POS & p);
public:void random ();
  BITB ruboff (const BITB & mask) const;
  BITB ruboff (int width1, int width2) const;
  BITB diagonal () const;
  BITB _fly (int a, int b) const;
  BITB fly (int a, int b) const;
  bool empty () const;
  bool only (const POS &) const;
  bool notbigger (int width1, int width2) const;
  int count () const;
  int blockcount () const;
  POS tag () const;
  PII range () const;
  BITB rangemask () const;
  bool contain (const BITB & smallbb) const;
  bool ismassof (const BITB & big) const;
  bool touch_ (const BITB & bb) const;
  bool touchboarder () const;
  bool diagonal_touch_ (const BITB & bb) const;
  int dist (const BITB &) const;
  pair < BITB, PII > relation_ (const BITB & bb) const;
  BITB blockat (const POS &) const;
  BITB blockon (const BITB & mask) const;
  BITB blockon_ (const BITB & mask) const;
  BITB touch_blockon_ (const BITB & mask) const;
  BITB diagonal_blockon_ (const BITB & mask) const;
  BITB dilate (int times = 1) const;
  BITB erode (int times = 1) const;
  BITB dry (const BITB & bone, int times) const;
  BITB adsorb (const BITB & bone, int times) const;
  BITB inner (int times = 7) const;
  BITB close (int times) const;
  BITB open (int times) const;
  BITB land (int times) const;
  BITB sea (int times) const;
  BITB wrapper_ (int dist) const;
  BITB transpose () const;
  BITB rotate (int angle) const;
  vector < short >pospemis () const;
  vector < short >rowpemis () const;
  PUU pemis3232 () const;
  ULL pemis64 () const;

  /*
     BITB project(const BITB& direction) const; 
     BITB pemismask() const; 
     ULL   pemis64_(const BITB& patmask) const; 
     PUU   pemis3232_(const BITB& patmask) const; 
   */
  BITB project () const;
  UL encode32 () const;
  VP keypos_ () const;
  VP jointpos () const;
  pair < EYEKIND, VP > eyekind (ACTION action) const;
public:
  BITB operator~ () const;
  BITB operator& (const BITB & bb) const;
  BITB operator| (const BITB & bb) const;
  BITB operator^ (const BITB & bb) const;
  void operator &= (const BITB & bb);
  void operator |= (const BITB & bb);
  void operator ^= (const BITB & bb);

  //BITB operator>>  (int n)  const; 
  //BITB operator<<  (int n)  const; 
  //void   operator> >= (int n); 
  //void   operator< <= (int n); 
  bool operator< (const BITB & bb) const;

  //bool   operator <= (const BITB& bb) const; 
  //bool   operator>   (const BITB& bb) const; 
  //bool   operator >= (const BITB& bb) const; 
  bool operator == (const BITB & bb) const;
  bool operator != (const BITB & bb) const;

  //BITB operator& (const POS& pos)       const; 
  BITB operator| (const POS & pos) const;
  BITB operator^ (const POS & pos) const;

  //void   operator &= (const POS& pos); 
  void operator |= (const POS & pos);
  void operator ^= (const POS & pos);

  //bool   operator&&  (const POS& pos)const; 
  //bool   operator||  (const POS& pos)const; 
  //bool   operator!   ( )                         const; 

  //BITB operator&        (const ROW rr)    const; 
  //BITB operator|        (const ROW rr)    const; 
  //BITB operator^        (const ROW rr)    const; 
  bool operator[] (const POS & pos) const;
  friend ostream & operator<< (ostream &, const BITB &);
};

const BITB NULL_BB;
const BITB FULL_BB = ~NULL_BB;

#endif
