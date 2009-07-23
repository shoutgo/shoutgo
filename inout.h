#ifndef _INOUT
#define _INOUT

#include "infoboard.h"
#include "brain.h"
#include "learn.h"
#include "tsumego.h"
#include "fir.h"
#include "nn.h"

class INOUT
{
public:
  bool axisshow;
public:
  INOUT (bool b = 1);
  string logo_ ();
  string stone (const COLOR &);
  string grid (const POS &);
  string midgrid (const COLOR l, const COLOR r, const POS & pos);
  string axis (const POS & pos) const;
  void print (const GO & go, const BITB & mask = FULL_BB);
  void print (const GO & go, const GO & markgo, RGB xxc = RB, RGB ooc = GB, RGB xoc = RG);

  /*
     string       axis ( string board); 
     string       print__ ( const GO&, const BITB& markbb = NULL_BB, 
     const POS& markp = NULL_POS, MODE_PRINT mode = BOARD_P ); 
     string       printbitboard ( const BITB& ); 
   */
  /*
     string       printblock(const GO&, const BITB& blk, const POS& markp = NULL_POS ); 
     string       printlibertysite(const GO&, const BITB& libsite, const POS& markp = NULL_POS ); 
     string       printliberty( const GO&, const VI& liberty); 
     string       printlabel ( const GO&, const pair<VB, VB >&, MODE_PRINTLABEL mode = L_CLUSTER); 
     string       printarea ( const GO& go, const pair<VB, VB >& pvv); 
     string       printonearea ( const GO&, const BITB& areabb); 
     string       printpublicarea ( const GO&, const BITB& areabb); 
   */
  /*
     string       printpartition (const VG& ); 
     string       printcluster ( const GO& go, const pair<VB, VB >& pvv); 
   */
  /*
     string       printpat ( const GO& go, const BITB& mask); 
     string       printpat ( const BITB& bb, const BITB& mask); 
   */
  PAT in2pat (string in);
  POS in2pos (const POS & pos, string in);
  POS jj2pos (string, const POS & pos = NULL_POS);

  /*
     vector<PII > sgf2xy(string filename); 
     vector<vector<PII > >        sgfs2xy(const VS& vecf); 
     VP                           sgf2pos(string filename); 
     VVP                          sgfs2pos(const VS& vecf); 
   */
  VS getfilename (string path, string type);
  string selectsgf (string path = SGFPATH);
  string readbuf (string filename);
  VM sgf2move (string filename);
  SGFROOT *sgf2tree (string filename);
  GO frommatlab (string filename = "forypgo.txt");
  void infop2sgf (INFOGO * infop, string filename);
  string _infop2sgf (INFOGO * infop);

  /*
     void            save2txt(string content, string filename = "go.txt"); 
     void            save2txt(const VVF& v, string filename = "datayx.txt"); 
     void            save2txt(const GO& go, string filename = "go.txt"); 
   */
  template < class T > bool save (const FSM < T > & fsm, string filename);
  template < class T > bool load (FSM < T > & fsm, string filename);

  // 这个是利用输出重定向按字符方式保存
  template < typename T > void save2txt (const T & t, string filename);

  // 这两个按二进制方式保存及读入单个类
  template < typename T > void save (const T & t, string filename);
  template < typename T > void load (T & t, string filename);

  // 这两个按二进制方式保存及读入一维向量类
  template < typename T > void save (const vector < T > &vt, ofstream & file);
  template < typename T > void load (vector < T > &vt, ifstream & file);

  // 这两个按二进制方式保存及读入二维向量类
  template < typename T > void save (const vector < vector < T > > & vvt, ofstream & file);
  template < typename T > void load (vector < vector < T > > & vvt, ifstream & file);
};

template < typename T >
void
INOUT::save2txt (const T & t, string filename)
{
  ofstream file (filename.c_str (), ios::app);
  if (!file)
    cerr << filename << " : open fail !" << endl;
  streambuf *old = cout.rdbuf ();
  cout.rdbuf (file.rdbuf ());
  cout << t;
  cout.rdbuf (old);

  // file << t; 与以上四句功能同
}

template < typename T >
void
INOUT::save (const T & t, string filename)
{
  ofstream file (filename.c_str (), ios::binary);
  if (!file)
    cerr << filename << " : open fail !" << endl;
  file.write ((char *) &t, sizeof (T));
  file.close ();
}

template < typename T >
void
INOUT::load (T & t, string filename)
{
  ifstream file (filename.c_str (), ios::binary);
  if (!file)
    cerr << filename << " : open fail !" << endl;
  file.read ((char *) &t, sizeof (T));
  file.close ();
}

template < typename T >
void
INOUT::save (const vector < T > &vt, ofstream & file)
{

  //ofstream      file(filename.c_str (), ios::binary); 
  //if (!file) 
  //      cerr<< filename << " : open fail !"<< endl; 
  int m = vt.size ();
  file.write ((char *) &m, sizeof (int));
  for (int i = 0; i < vt.size (); ++i)
    file.write ((char *) &vt[i], sizeof (T));

  //file.close(); 
}

template < typename T >
void
INOUT::load (vector < T > &vt, ifstream & file)
{

  //ifstream file(filename.c_str (), ios::binary); 
  //if (!file) 
  //      cerr<< filename << " : open fail !"<< endl; 
  T t;
  int m;
  vt.clear ();
  file.read ((char *) &m, sizeof (int));
  for (int i = 0; i < m; ++i)
    {
      file.read ((char *) &t, sizeof (T));
      vt.push_back (t);
    }

  //file.close(); 
}

template < typename T >
void
INOUT::save (const vector < vector < T > > & vvt, ofstream & file)
{

  //ofstream      file(filename.c_str (), ios::binary); 
  //if (!file) 
  //      cerr<< filename << " : open fail !"<< endl; 
  int m = vvt.size ();
  file.write ((char *) &m, sizeof (int));
  for (int i = 0; i < m; ++i)
    {
      int n = vvt[i].size ();
      file.write ((char *) &n, sizeof (int));
      for (int j = 0; j < n; ++j)
        file.write ((char *) &vvt[i][j], sizeof (T));
    }

  //file.close(); 
}

template < typename T >
void
INOUT::load (vector < vector < T > > & vvt, ifstream & file)
{

  //ifstream file(filename.c_str (), ios::binary); 
  //if (!file) 
  //      cerr<< filename << " : open fail !"<< endl; 
  T t;
  int m, n;
  vvt.clear ();
  file.read ((char *) &m, sizeof (int));
  for (int i = 0; i < m; ++i)
    {
      file.read ((char *) &n, sizeof (int));
      vector < T > vt;
      for (int j = 0; j < n; ++j)
        {
          file.read ((char *) &t, sizeof (T));
          vt.push_back (t);
        }
      vvt.push_back (vt);
    }

  //file.close(); 
}

ostream & operator<< (ostream & os, const PAT & pat);
ostream & operator<< (ostream & os, const POS & pos);
ostream & operator<< (ostream & os, const VS & vecs);

//ostream& operator<<(ostream& os, const VB&  vecb); 
//ostream& operator<<(ostream& os, const VP&  vecp); 
ostream & operator<< (ostream & os, const VI & veci);

//ostream& operator<<(ostream& os, const VC&  vecc); 
//ostream& operator<<(ostream& os, const VF&  vf); 

//ostream& operator<<(ostream& os, const VVF& vvf); 
//ostream& operator<<(ostream& os, const VVI& vvi); 

//ostream& operator<<(ostream& os, const PII& pii); 
//ostream& operator<<(ostream& os, const PUU& puu); 
ostream & operator<< (ostream & os, const pair < EYEKIND, VP > &ts);
ostream & operator<< (ostream & os, MPI m);

template < class T >
ostream & operator<< (ostream & os, const vector < T > &vt)
{
  for (int i = 0; i < vt.size (); ++i)
    os << setw (22) << vt[i] << " ";
  return os;
}

template < class T >
ostream & operator<< (ostream & os, const vector < vector < T > > & vvt)
{
  for (int i = 0; i < vvt.size (); ++i)
    os << vvt[i] << endl;
  return os;
}

template < class T, class S >
ostream & operator<< (ostream & os, const pair < T, S > pts)
{
  os << "[" << pts.first << ", " << pts.second << "] ";
  return os;
}

template < class T >
bool INOUT::save (const FSM < T > & fsm, string filename)
{
  ofstream file (filename.c_str (), ios::binary);
  if (!file)
    {
      cerr << filename << " : open fail !" << endl;
      return 0;
    }
  save (fsm.father, file); 
  save (fsm.sons, file); 
  save (fsm.freq, file); 
  file.close ();
  clog << "A FSM been saved in " << filename << " ." << endl;
  return 1;
}

template < class T >
bool INOUT::load (FSM < T > & fsm, string filename)
{
  ifstream file (filename.c_str (), ios::binary);
  if (!file)
    {
      cerr << filename << " : open fail !" << endl;
      return 0;
    }
  load (fsm.father, file); 
  load (fsm.sons, file); 
  load (fsm.freq, file); 
  file.close ();
  clog << "A FSM been loaded from " << filename << " ." << endl;
  return 1;
}

#endif
