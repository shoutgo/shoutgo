#ifndef _INFOBOARD
#define _INFOBOARD

#include "itr.h"
#include "board.h"
#include "transptable.h"

/* class INCREMENT */
class INCREMENT
{
  VI tag;
  VC color;
  VB content;
  VB enemysite;
  VB libertysite;
  VI liberty;
public:
  INCREMENT ();
public:
  BITB getcontent (int i) const;
  COLOR getcolor (int i) const;
  BITB getenemysite (int i) const;
  BITB getlibertysite (int i) const;
  int getliberty (int i) const;
  int gettag (int i) const;
  void setcontent (int i, const BITB & blk);
  void setcolor (int i, COLOR clr);
  void setenemysite (int i, const BITB & bb);
  void setlibertysite (int i, const BITB & bb);
  void setliberty (int i, int n);
  void settag (int i, int tag);
  bool operator == (const INCREMENT & ic) const;
};


/* class ZOBRIST */
class ZOBRIST
{
public:
  static const int ZOBRIST_KIND = 9;
  int count[ZOBRIST_KIND];
  KEY key[ZOBRIST_KIND];
public:
  ZOBRIST ();
  void setzobrist (int i, const POS & lastpos, COLOR lastclr, const BITB & kill);
  KEY getzobrist (int id) const;
  int getcount (int i) const;
};


/* class PATFREQ */
class PATFREQ
{
public:
  static const int PEMIS_KIND = 4;
private:
  int x[PEMIS_KIND][BS * BS];
  int o[PEMIS_KIND][BS * BS];
public:
  PATFREQ ()
  {
  }
  void setflag ();
  void setf (const POS & pos, COLOR clr, int i, int freq);
  int getf (const POS & pos, COLOR clr, int i) const;
  friend ostream & operator<< (ostream & os, const PATFREQ & pf);
};


/* struct EVENT */
struct EVENT
{
  BITB mask;
  SHAPETERM sh;
  LIBTERM lib;
  int value;
};

/* class KO */

// 劫目的至少包括：使劫点空起，使劫点粘起，以此紧逼提子
class KO
{
  pair < POS, POS > pos2;
  int history;
  int liberty;			//缓气劫
  vector < KO > vk;		//相关联的劫
};


/* class AREA */
class AREA
{
public:
  /*
     VB xmass; 
     VB xrawland; 
     VB xrawsea; 
     VB xrawair; 
     VB xland; 
     VB xsea; 
     VB xair; 
     VB omass; 
     VB orawland; 
     VB orawsea; 
     VB orawair; 
     VB oland; 
     VB osea; 
     VB oair; 
     BITB publicsea; 
     BITB publicair; 
     BITB undecided; 
   */
  int worknumb;
  VG massmaps;
  GO clustermap;
  GO potentialmap;
public:
  int delta () const;
  BITB getbb (COLOR clr) const;
  void setclustermap (const BITB & mask, COLOR clr);
  STATE clusterstate (const PBC & pbc) const;
  bool onland (const PBC & pbc, const POS & pos) const;
  BITB publicarea () const;
  void setpotentialmap ();
  BITB getclustermap (COLOR clr, STATE st) const;
  bool newmass (const POS & pos, COLOR clr) const;
  bool isolated (const PBC & pbc) const;
  void setworknumb (const POS & pos);
  PBC getmass (const BITB & pos, COLOR clr) const;
  PBC getcluster (const BITB & pos) const;
  PBC getpotential (const BITB & pos) const;
  vector < pair < int, PBC > > dist_mass (const POS & pos, COLOR clr) const;
  vector < pair < int, PBC > > dist_cluster (const POS & pos) const;
};


/* class INFOGO */
class INFOGO:public GO
{
public:
  static const int SMALLBLOCK_RANGE = 3;
  static const int CORNERJOSEKI_MAX = 30;
  static const int EDGEJOSEKI_MAX = 60;
  static const int FUSEKI_MAX = 100;
  INFOGO *father;
  vector < INFOGO * >sons;
private:
  ZOBRIST zobrist;
public:
  INCREMENT inc;	// 为了执行 scan() 暂时将其 public
  PATFREQ patfreq;
  EVENT event;
  AREA area;
public:
  INFOGO ();
  INFOGO (const GO & go);
  ~INFOGO ();
  INCREMENT scan () const;
  bool move (const POS &, COLOR);
  INFOGO *move_tree (const POS & pos, COLOR clr);
  void delfromhere ();
  void delfromroot ();
  void update ();
  POS genmove () const;
  void setzobrist ();
  KEY getzobrist (int n) const;
  VI getf_zobrist (int n, const BITB mask, COLOR clr) const;
  VI getf_patfreq (int n, const BITB mask, COLOR clr);
  PUU pemis (int n) const;
  VG partition (int times = BS) const;

  /*
     PBB                          lastpartition(int partitiontimes) const; 
     pair<VB, VB >        getallmass(const PBB& lastparti) const; 
     AREA                 setarea(int partitiontimes = 3, 
     int dilatetimes = 5, 
     int airtimes = 3)    const; 
   */
  BITB cutingpos () const;
  BITB cutpos (COLOR sente) const;
  int samemassmapsnumb () const;
  bool operator == (const GO & go) const;
  friend ostream & operator<< (ostream &, const INFOGO &);
};

#endif
