#include "utility.h"
#include "bitboard.h"
#include "itr.h"
#include "inout.h"

//注意这里的函数可以用查表法优化
int
popu (unsigned x)
{
  x = x - ((x >> 1) & 0x55555555);
  x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
  x = (x + (x >> 4)) & 0x0F0F0F0F;
  x = x + (x >> 8);
  x = x + (x >> 16);
  return x & 0x3F;
}

unsigned
connectingone (unsigned x, int n)
{
  int s;
  while (n > 1)
    {
      s = n >> 1;
      x = x & (x << s);
      n -= s;
    }
  return x & ~(x >> 1) & ~(x << 1);
}

unsigned
reverse (unsigned x)
{
  x = (x & 0x55555555) << 1 | (x >> 1) & 0x55555555;
  x = (x & 0x33333333) << 2 | (x >> 2) & 0x33333333;
  x = (x & 0x0F0F0F0F) << 4 | (x >> 4) & 0x0F0F0F0F;
  x = (x << 24) | (x & 0xFF00) << 8 | ((x >> 8) & 0xFF00) | (x >> 24);
  return x;
}

int
nleadingzero (unsigned x)
{
  unsigned y;
  int n = 32;
  y = x >> 16;
  if (y != 0)
    {
      n = n - 16;
      x = y;
    }
  y = x >> 8;
  if (y != 0)
    {
      n = n - 8;
      x = y;
    }
  y = x >> 4;
  if (y != 0)
    {
      n = n - 4;
      x = y;
    }
  y = x >> 2;
  if (y != 0)
    {
      n = n - 2;
      x = y;
    }
  y = x >> 1;
  if (y != 0)
    {
      return n - 2;
    }
  return n - x;
}

int
ntailzero (unsigned x)
{
  return popu (x ^ (x - 1)) - 1;
}

ROW
rowexpand (ROW background, ROW rpos)
{
  rpos &= background;
  ROW tmp[BS + 1];
  tmp[0] = rpos;
  int i;
  for (i = 0; tmp[i + 1] = background & (tmp[i] | tmp[i] << 1 | tmp[i] >> 1), tmp[i + 1] != tmp[i]; ++i);
  return tmp[i + 1];
}

VP
near4 (const POS & pos)
{
  if (g_init.inited)
    {
      return g_init.getnear4 (pos);
    }
  VP vecp;
  vecp.reserve (4);
  POS p;
  p.first = pos.first, p.second = pos.second >> 1;
  if (!outside (p))
    vecp.push_back (p);
  p.second = pos.second << 1;
  if (!outside (p))
    vecp.push_back (p);
  ++p.first, p.second = pos.second;
  if (!outside (p))
    vecp.push_back (p);
  p.first -= 2;
  if (!outside (p))
    vecp.push_back (p);
  return vecp;
}

VP
near8 (const POS & pos)
{
  VP vecp;
  vecp.reserve (8);
  POS p;
  p.first = pos.first, p.second = pos.second >> 1;
  if (!outside (p))
    vecp.push_back (p);
  p.second = pos.second << 1;
  if (!outside (p))
    vecp.push_back (p);
  ++p.first;
  if (!outside (p))
    vecp.push_back (p);
  p.second = pos.second;
  if (!outside (p))
    vecp.push_back (p);
  p.second >>= 1;
  if (!outside (p))
    vecp.push_back (p);
  p.first -= 2;
  if (!outside (p))
    vecp.push_back (p);
  p.second = pos.second;
  if (!outside (p))
    vecp.push_back (p);
  p.second <<= 1;
  if (!outside (p))
    vecp.push_back (p);
  return vecp;
}


// NULL_POS 和 PASS_POS 都返回 1
bool
outside (const POS & pos)
{
  return ((pos.second == 0) || (pos.second > ROWMASK) || (pos.first < 0) || (pos.first >= BS));
}

bool
onboarder (const POS & p)
{
  if ((p.first == 0) || (p.first == (BS - 1)))
    return 1;

  else if ((p.second & LEFTEST) || (p.second & 1))
    return 1;

  else
    return 0;
}

POS
rot (int angle, const POS & pos)
{
  ___PARASSERT (angle == 0 || angle == 90 || angle == 180 || angle == 270);
  if (pos == NULL_POS)
    return pos;
  switch (angle)
    {
    case 90:
      return make_pair (BS - 1 - ntailzero (pos.second), 1 << pos.first);
    case 180:
      return make_pair (BS - 1 - pos.first, 1 << (BS - 1 - ntailzero (pos.second)));
    case 270:
      return make_pair (ntailzero (pos.second), 1 << (BS - 1 - pos.first));
    default:
      return pos;               // 2006.8.20 以使在angle = 0时正确
    }
}

POS
transp (const POS & pos)
{
  if (pos == NULL_POS)
    return pos;
  return make_pair (BS - 1 - ntailzero (pos.second), 1 << (BS - 1 - pos.first));
}

POS
star (int n)
{
  ___PARASSERT (BS > 0 && BS == (int) (BS / 2) * 2 + 1);
  switch (n)
    {
    case 1:
      return make_pair (3, 1 << 3);
    case 2:
      return make_pair (3, CENTEREST);
    case 3:
      return make_pair (3, 1 << (BS - 1 - 3));
    case 4:
      return make_pair (MIDBS, 1 << 3);
    case 5:
      return make_pair (MIDBS, CENTEREST);
    case 6:
      return make_pair (MIDBS, 1 << (BS - 1 - 3));
    case 7:
      return make_pair (BS - 1 - 3, 1 << 3);
    case 8:
      return make_pair (BS - 1 - 3, CENTEREST);
    case 9:
      return make_pair (BS - 1 - 3, 1 << (BS - 1 - 3));
    default:
      return NULL_POS;
    }
}

POS
star (DIRECTION d)
{
  ___PARASSERT (BS > 0 && BS == (int) (BS / 2) * 2 + 1);
  switch (d)
    {
    case EN:
      return make_pair (3, 1 << 3);
    case NORTH:
      return make_pair (3, CENTEREST);
    case WN:
      return make_pair (3, 1 << (BS - 1 - 3));
    case EAST:
      return make_pair (MIDBS, 1 << 3);
    case CENTER:
      return make_pair (MIDBS, CENTEREST);
    case WEST:
      return make_pair (MIDBS, 1 << (BS - 1 - 3));
    case ES:
      return make_pair (BS - 1 - 3, 1 << 3);
    case SOUTH:
      return make_pair (BS - 1 - 3, CENTEREST);
    case WS:
      return make_pair (BS - 1 - 3, 1 << (BS - 1 - 3));
    default:
      return NULL_POS;
    }
}

POS
corner (DIRECTION d)
{
  switch (d)
    {
    case EN:
      return make_pair (0, 1);
    case NORTH:
      return make_pair (0, CENTEREST);
    case WN:
      return make_pair (0, LEFTEST);
    case WEST:
      return make_pair (MIDBS, LEFTEST);
    case WS:
      return make_pair ((BS - 1), LEFTEST);
    case SOUTH:
      return make_pair ((BS - 1), CENTEREST);
    case ES:
      return make_pair ((BS - 1), 1);
    case EAST:
      return make_pair (MIDBS, 1);
    case CENTER:
      return make_pair (MIDBS, CENTEREST);
    default:
      return NULL_POS;
    }
}

int
xy2i_ (int x, int y)
{
  return x * BS + y;
}

PII
i2xy_ (int i)
{
  int x = i / BS;
  return make_pair (x, i - x * BS);
}

POS
_xy2pos (int x, int y)
{
  return make_pair (x, 1 << y);
}


// 注意这几个函数的前提： popu(pos.second) == 1 
PII
_pos2xy (const POS & pos)
{
  return make_pair (pos.first, ntailzero (pos.second));
}

POS
i2pos (int i)
{
  ___PARASSERT (i != NULL_TAG);
  int x = i / BS;
  return make_pair (x, 1 << (i - x * BS));
}

int
pos2i (const POS & pos)
{
  ___PARASSERT (pos != NULL_POS);
  return pos.first * BS + ntailzero (pos.second);
}

POS
aa2pos (string in)
{
  return make_pair (tolower (in[1]) - 'a', 1 << (tolower (in[0]) - 'a'));
}

string
pos2aa (const POS & pos)
{
  char c[3];
  c[0] = ntailzero (pos.second) + 'a';
  c[1] = pos.first + 'a';
  c[2] = '\0';
  return string (c);
}

int
interval_ (const POS & p1, const POS & p2)
{
  return (((p1.first >= p2.first) ? (p1.first - p2.first) : (p2.first - p1.first)) + popu ((p1.second - 1) ^ (p2.second - 1)));
}

BITB
cross (const POS & center, int radius)
{
  int up = center.first - radius;
  int down = center.first + radius;
  ROW left = center.second << (radius + 1);
  ROW right = center.second >> radius;
  up = (up < 0) ? 0 : up;
  down = (down >= BS) ? (BS - 1) : down;
  left = (left > (1 << BS) || left == 0) ? (1 << BS) : left;
  right = (right == 0) ? 1 : right;
  BITB bb;
  for (int i = up; i <= down; ++i)
    bb.r[i] = center.second;
  bb.r[center.first] = left - right;
  return bb;
}

BITB
square (const POS & center, int radius)
{
  int up = center.first - radius;
  int down = center.first + radius;
  ROW left = center.second << (radius + 1);
  ROW right = center.second >> radius;
  up = (up < 0) ? 0 : up;
  down = (down >= BS) ? (BS - 1) : down;
  left = (left > (1 << BS) || left == 0) ? (1 << BS) : left;
  right = (right == 0) ? 1 : right;
  BITB bb;
  for (int i = up; i <= down; ++i)
    bb.r[i] = left - right;
  return bb;
}

BITB
frame (const POS & center, int radius)
{
  int up = center.first - radius;
  int down = center.first + radius;
  ROW left = center.second << radius;
  ROW right = center.second >> radius;
  up = (up < 0) ? 0 : up;
  down = (down >= BS) ? (BS - 1) : down;
  right = (right == 0) ? 1 : right;
  left = (left > LEFTEST || left == 0) ? LEFTEST : left;
  BITB bb;
  for (int i = up + 1; i < down; ++i)
    bb.r[i] = (left | right);
  bb.r[up] = bb.r[down] = (left << 1) - right;
  return bb;
}

BITB
strip (const POS & pos, int horizontal, int vertical, DIRECTION d)
{
  int up, down;
  ROW left, right;
  if (d != SOUTH && d != ES && d != WS)
    {
      up = pos.first - vertical;
      up = (up < 0) ? 0 : up;
    }
  else
    up = pos.first;
  if (d != NORTH && d != EN && d != WN)
    {
      down = pos.first + vertical;
      down = (down >= BS) ? (BS - 1) : down;
    }
  else
    down = pos.first;
  if (d != EAST && d != ES && d != EN)
    {
      left = pos.second << (horizontal + 1);
      left = (left > (1 << BS) || left == 0) ? (1 << BS) : left;
    }
  else
    left = pos.second << 1;
  if (d != WEST && d != WS && d != WN)
    {
      right = pos.second >> horizontal;
      right = (right == 0) ? 1 : right;
    }
  else
    right = pos.second;
  BITB bb;
  for (int i = up; i <= down; ++i)
    bb.r[i] = (left - right);
  return bb;
}

BITB
strip (const POS & p1, const POS & p2)
{
  return (NULL_BB | p1 | p2).rangemask ();
}

BITB
strip (const POS & center, int u, int l, int d, int r)
{
  int up = center.first - u;
  int down = center.first + d;
  ROW left = center.second << (l + 1);
  ROW right = center.second >> r;
  up = (up < 0) ? 0 : up;
  down = (down >= BS) ? (BS - 1) : down;
  left = (left > (1 << BS) || left == 0) ? (1 << BS) : left;
  right = (right == 0) ? 1 : right;
  BITB bb;
  for (int i = up; i <= down; ++i)
    bb.r[i] = left - right;
  return bb;
}

BITB
diamond (const POS & center, int radius)
{
  int up = center.first - radius;
  int down = center.first + radius;
  ROW left = center.second << (radius + 1);
  ROW right = center.second >> radius;
  up = (up < 0) ? 0 : up;
  down = (down >= BS) ? (BS - 1) : down;
  left = (left > (1 << BS) || left == 0) ? (1 << BS) : left;
  right = (right == 0) ? 1 : right;
  ROW middle = left - right;
  BITB bb;
  for (int i = 0; i <= (center.first - up); ++i)
    bb.r[center.first - i] = (middle >> i) & (middle << i);
  for (int i = 1; i <= (down - center.first); ++i)
    bb.r[center.first + i] = (middle >> i) & (middle << i);
  return bb;
}

BITB
pie (const POS & center, int radius)
{
  BITB bb = square (center, radius);
  int m, n;
  ITR itr (bb);
  for (POS pos = itr.stonebegin (); !itr.stoneend (); pos = itr.stonenext ())
    {
      m = pos.first - center.first;
      n = popu ((pos.second - 1) ^ (center.second - 1));
      if ((m * m + n * n) > radius * radius)
        bb ^= pos;
    }
  return bb;
}

BITB
circle (const POS & center, int radius)
{
  BITB bb = square (center, radius);
  int m, n, l;
  ITR itr (bb);
  for (POS pos = itr.stonebegin (); !itr.stoneend (); pos = itr.stonenext ())
    {
      m = pos.first - center.first;
      n = popu ((pos.second - 1) ^ (center.second - 1));
      l = (m * m + n * n);
      if (l > radius * radius || l <= (radius - 1) * (radius - 1))
        bb ^= pos;
    }
  return bb;
}

BITB
line (POS p1, POS p2)
{
  if ((p1.first == p2.first) || (p1.second == p2.second))
    return strip (p1, p2);
  BITB bb = NULL_BB | p1 | p2;
  if (p1.second < p2.second)
    swap (p1, p2);
  int op = (p1.first > p2.first) ? -1 : 1;
  int dy = (p1.first > p2.first) ? (p1.first - p2.first) : (p2.first - p1.first);
  int dx = popu ((p1.second - 1) ^ (p2.second - 1));
  int steps = (dx >= dy) ? dx : dy;
  float xincreasement = (float) dx / steps;
  float yincreasement = (float) dy / steps;
  for (int i = 1; i < steps; ++i)
    bb.r[p1.first + (int) (i * yincreasement + 1 / 2) * op] |= p1.second >> (int) (i * xincreasement + 1 / 2);
  return bb;
}

unsigned
mulhigh32 (unsigned u, unsigned v)
{
  unsigned u0, v0, w0, u1, v1, w1, w2, t;
  u0 = u & 0xffff;
  u1 = u >> 16;
  v0 = v & 0xffff;
  v1 = v >> 16;
  w0 = u0 * v0;
  t = u1 * v0 + (w0 >> 16);
  w1 = t & 0xffff;
  w2 = t >> 16;
  w1 += u0 * v1;
  return u1 * v1 + w2 + (w1 >> 16);
}

bool
muloverflow (unsigned x, unsigned y)
{
  int m = nleadingzero (x);
  int n = nleadingzero (y);
  if (m + n <= 30)
    return 1;
  int t = x * (y >> 1);
  if ((int) t < 0)
    return 1;
  int z = t * 2;
  if (y & 1)
    if ((z + x) < x)
      return 1;
  return 0;
}


/*
bool    issamepat(string a, string b){  
        string ax = a; 
        for (int i = 0; i<a.size (); ++i){
                if              (ax.substr (i, 1) == "x")   
                        ax.replace (i, 1, "o"); 
                else if (ax.substr (i, 1) == "o")   
                        ax.replace (i, 1, "x"); 
                else if (ax.substr (i, 1) == "X")   
                        ax.replace (i, 1, "O"); 
                else if (ax.substr (i, 1) == "O")   
                        ax.replace (i, 1, "X"); 
        }
        //第 1 种情况 (未翻转) 
    if (a == b || ax == b)                                              
                return 1; 
        //把 b 读入 bmn, m 行 n 列.
        char bmn[BS][BS]; 
        int  m = 0, n = 0, p; 
        for (p = 0; p<b.size(); ++p){
                if (b.substr (p, 1) != "\n")                    
                        bmn[m][n++] = b[p]; 
                else {
                        n = 0; 
                        ++m; 
                }
        }
        n = p/m - 1; //-// 取模？与in2pat()有重复

        //其它 7 种对称情况 (看作是座标读取方式的不同)
        string tmp = ""; 
        for (int i = 0; i<n; ++i)               
                for(int j = 0; j<m; ++j){
                        tmp += bmn[j][i]; 
                        if (j == m-1) tmp += "\n"; 
                }                                                                       
    if (a == tmp || ax == tmp) 
                return 1; 
                        
        tmp = ""; 
        for (int i = m-1; i >= 0; --i)
                for(int j = 0; j<n; ++j){
                        tmp += bmn[i][j]; 
                        if (j == n-1) tmp += "\n"; 
                }                                                                       
    if (a == tmp || ax == tmp) 
                return 1; 

        tmp = ""; 
        for (int i = 0; i<n; ++i)
                for(int j = m-1; j >= 0; --j){
                        tmp += bmn[j][i]; 
                        if (j == 0) tmp += "\n"; 
                }                                                                       
    if (a == tmp || ax == tmp) 
                return 1; 

        tmp = ""; 
        for (int i = m-1; i >= 0; --i)
                for(int j = n-1; j >= 0; --j){
                        tmp += bmn[i][j]; 
                        if (j == 0) tmp += "\n"; 
                }                                                                       
    if (a == tmp || ax == tmp) 
                return 1; 

        tmp = ""; 
        for (int i = n-1; i >= 0; --i)
                for(int j = m-1; j >= 0; --j){
                        tmp += bmn[j][i]; 
                        if (j == 0) tmp += "\n"; 
                }                                                                       
    if (a == tmp || ax == tmp) 
                return 1; 

        tmp = ""; 
        for (int i = 0; i<m; ++i)
                for(int j = n-1; j >= 0; --j){
                        tmp += bmn[i][j]; 
                        if (j == 0) tmp += "\n"; 
                }                                                                       
    if (a == tmp || ax == tmp) 
                return 1; 

        tmp = ""; 
        for (int i = n-1; i >= 0; --i)
                for(int j = 0; j<m; ++j){
                        tmp += bmn[j][i]; 
                        if (j == m-1) tmp += "\n"; 
                }                                                                       
    if (a == tmp || ax == tmp) 
                return 1; 
        
        return 0; 
}

//此函数应当避免使用, 也未有 X O 符号
string  asciipat(string patstr){        
        for (int p = 0; p<patstr.size (); ++p){                 // p += 2
                if (patstr.substr(p, 2) == "●")         { patstr.replace (p, 2, "x"); continue; }
                if (patstr.substr(p, 2) == "○")         { patstr.replace (p, 2, "o"); continue; }
                if (patstr.substr(p, 2) == "┼")         { patstr.replace (p, 2, "."); continue; }
                if (patstr.substr(p, 2) == "┏")         { patstr.replace (p, 2, "+"); continue; }
                if (patstr.substr(p, 2) == "┓")         { patstr.replace (p, 2, "+"); continue; }
                if (patstr.substr(p, 2) == "┛")         { patstr.replace (p, 2, "+"); continue; } 
                if (patstr.substr(p, 2) == "┗")         { patstr.replace (p, 2, "+"); continue; }
                if (patstr.substr(p, 2) == "┯")         { patstr.replace (p, 2, "-"); continue; }
                if (patstr.substr(p, 2) == "┨")         { patstr.replace (p, 2, "-"); continue; }
                if (patstr.substr(p, 2) == "┷")         { patstr.replace (p, 2, "-"); continue; }
                if (patstr.substr(p, 2) == "┠")         { patstr.replace (p, 2, "-"); continue; }
                if (patstr.substr(p, 2) == "◎")         { patstr.replace (p, 2, "o"); continue; }
                if (patstr.substr(p, 2) == "⊙")         { patstr.replace (p, 2, "*"); continue; }
                if (patstr.substr(p, 2) == "\n\n")      { patstr.replace (p, 2, "\n"); continue; }
        }
        return patstr; 
}

//此 mask 可以呈不规则形状
//注意可以生成 "xbb\no-\n" 模式, 并不能用in2pat()还原
string  pat2ascii ( const BITB& xbb, const BITB& obb, const BITB& mask){   
                                                                                                
        string s = ""; 
        BITB squarem = project(((xbb|obb)&mask).rangemask(), mask.rangemask()); 
        POS pos; //应该调用已存的位置向量
        for (int i = 0; i<BS*BS; ++i) {
                pos = i2pos(i); 
                if (squarem[pos]){
                        if              (xbb[pos]&&!obb[pos])   {
                                if (mask[pos]) 
                                        s += (onboarder(pos)?"X":"x"); 
                                else s += "  "; 
                        }
                        else if(!xbb[pos]&&obb[pos])    {
                                if (mask[pos]) 
                                        s += (onboarder(pos)?"O":"o"); 
                                else s += "  "; 
                        }
                        else if (xbb[pos]&&obb[pos])    {
                                if (mask[pos]) 
                                        s += "*"; 
                                else s += "  "; 
                        }
                        else if (!xbb[pos]&&!obb[pos]) {
                                if (mask[pos]) 
                                        s += asciigrid(pos); 
                                else s += "  "; 
                        }
                }
                if (pos.second == LEFTEST && squarem.r[pos.first])             
                        s += "\n"; 
        }
        return s; 
}

//此 mask 可以呈不规则形状
//注意可以生成 "xx\nx-\n" 模式, 并不能用in2pat()还原
//可以生成 "+o\n" 或 "-o\n" 模式, 待改进
string  pat2ascii(const BITB& bb, const BITB& mask) { 
        string s; 
        BITB squarem = project((bb&mask).rangemask(), mask.rangemask()); 
        POS pos; //应该调用已存的位置向量
        for (int i = 0; i<BS*BS; ++i) {
                pos = i2pos(i); 
                if (squarem[pos]){
                        switch (bb[pos]) {
                                case 1:             
                                        if (mask[pos]) 
                                                s += (onboarder(pos)?"O":"o"); 
                                        else s += "  "; 
                                        break; 
                                case 0:                         
                                        if (mask[pos]) 
                                                s += asciigrid(pos); 
                                        else s += "  "; 
                                        break; 
                        }
                }
                if (pos.second == LEFTEST && squarem.r[pos.first])           
                        s += "\n"; 
        }
        return s; 
}

string  asciigrid(const POS& pos) {  
        int              r = pos.first; 
        ROW              p = pos.second; 
        if (r == 0&&p == LEFTEST)       {return "+"; }
        if (r == 0&&p == 1)                     {return "+"; }
        if (r == BS-1&&p == 1)          {return "+"; }
        if (r == BS-1&&p == LEFTEST){return "+"; }
        if (r == 0)                             {return "-"; }
        if (p == 1)                             {return "-"; }
        if (r == (BS-1))                        {return "-"; }
        if (p == LEFTEST)                       {return "-"; }
        if ((BS == 19) &&       ((r == 3) || (r == 9) || (r == 15) ) &&
                ((p == (1<<(BS-4))) || (p == (1<<(BS-10))) || (p == (1<<(BS-16))))
                )
                                                        {return "."; }
        else                                    {return "."; }
}
*/

bool
prob (int small, int large)
{
  if ((rand () % large) < small)
    return 1;
  else
    return 0;
}

int
randsqrt (int m)
{
  return sqrtf ((float) (rand () % (m * m)));
}

int
randlog_ (int m)
{                               //-// right ?
  return log ((float) (rand () + 1)) / 15 * m;
}

float
randf ()
{
  return ((float) rand () / (float) (rand () + 1) * (prob (1, 2) ? 1 : -1));

  //UL seed = (unsigned)clock(); 
  //seed = rand32(seed); 
  //return ( * (float *) & seed ); 
}

VF
randvf (int m)
{
  VF v;
  for (int j = 0; j < m; ++j)
    v.push_back ((float) rand () / (float) (rand () + 1) * (prob (1, 2) ? 1 : -1));
  return v;
}

VVF
randvvf (int m, int n)
{
  VVF vv;

  //2009// vv.assign (m, 0); 
  //for (int i = 0; i<m; ++i)
  //      for (int j = 0; j<n; ++j)
  //              vv[i].push_back ((float)rand()/(float)(rand()+1)*(prob(1, 2)?1:-1)); 
  for (int i = 0; i < m; ++i)
    {
      VF vf;
      for (int j = 0; j < n; ++j)
        vf.push_back ((float) rand () / (float) (rand () + 1) * (prob (1, 2) ? 1 : -1));
      vv.push_back (vf);
    } return vv;
}

VVVF
randvvvf_ (int m, int n, int l)
{
  VVVF vvv;

  //2009// vvv.assign (m, 0); //-//
  //for (int i = 0; i<m; ++i)
  //      for (int j = 0; j<n; ++j)
  //              for (int k = 0; k<l; ++k)
  //                      vvv[i][j].push_back ((float)rand()/(float)(rand()+1)*(prob(1, 2)?1:-1)); 
  for (int i = 0; i < m; ++i)
    {
      VVF vvf;
      for (int j = 0; j < n; ++j)
        {
          VF vf;
          for (int k = 0; k < l; ++k)
            vf.push_back ((float) rand () / (float) (rand () + 1) * (prob (1, 2) ? 1 : -1));
          vvf.push_back (vf);
        } vvv.push_back (vvf);
    } return vvv;
}

unsigned
randu (int m)
{
  return rand () % m;
}

bool
apos_ (const POS & pos)
{
  return popu (pos.second) == 1;
}

VVI
index8 (int a, int b)
{

  // 8 种对称方式
  int id[8][6] = { {0, a - 1, 1, 0, b - 1, 1},  // +0
  {0, a - 1, 1, b - 1, 0, -1},  // |
  {a - 1, 0, -1, 0, b - 1, 1},  // -
  {a - 1, 0, -1, b - 1, 0, -1}, // +180
  {0, b - 1, 1, 0, a - 1, 1},   // "\"
  {0, b - 1, 1, a - 1, 0, -1},  // +90
  {b - 1, 0, -1, 0, a - 1, 1},  // +270
  {b - 1, 0, -1, a - 1, 0, -1}  // /
  };

  // 生成从 0 到 a*b-1 的一维坐标，对应 a 行 b 列的二维坐标
  VI index;
  for (int i = 0; i < a * b; ++i)
    index.push_back (i);

  // 生成 8 种对称方式下新的一维坐标
  VVI vvi;
  vvi.reserve (8);
  for (int i = 0; i < 8; ++i)
    {
      VI tmp;
      for (int j = id[i][0]; id[i][0] < id[i][1] ? j <= id[i][1] : j >= id[i][1]; j += id[i][2])
        for (int k = id[i][3]; id[i][3] < id[i][4] ? k <= id[i][4] : k >= id[i][4]; k += id[i][5])

          // 前 4 种先固定行，后 4 种先固定列
          tmp.push_back (index[(i < 4 ? j : k) * b + (i < 4 ? k : j)]);
      vvi.push_back (tmp);
    } return vvi;
}


// Here is the random number algorithm issued by Lewis, Goodman and Miller in  1969:
// 1. Multiplier = 5 ^ 7; 
// 2. Divisor = 2 ^ 31 - 1, which is a prime number; 
// 3. Seed *= Multiplier; 
// 4. Seed % = Divisor; 
UL
rand32 (UL & Seed, UL Multiplier)
{
  Seed = longmulmod (Seed, Multiplier, 0x7fffffff);
  return Seed;
}

UL
longmulmod (UL Multiplier, UL Multiplicand, UL Divisor)
{
  UL Result;

  /*
     __asm {
     mov eax, Multiplier; 
     mul Multiplicand; 
     div Divisor; 
     mov Result, edx; 
     }
   *///2009//
  Result = (Multiplicand * Multiplier) % Divisor;
  return Result;
}

COLOR
operator ~ (const COLOR c)
{
  switch (c)
    {
    case BLACK:
      return WHITE;
    case WHITE:
      return BLACK;
    default:
      return NULL_CLR;
    }
}

POS
ai2pos (string ai)
{
  if ((ai.size () != 2 && ai.size () != 3) || !isalpha (ai[0]) || !isdigit (ai[1]))
    {
      return NULL_POS;
    }
  if (ai.size () == 3)
    if (!isdigit (ai[2]))
      return NULL_POS;
  int x = atoi (ai.substr (1, ai.size () - 1).c_str ()) - 1;
  int y = tolower (ai[0]) - 'a';
  if (x > BS - 1 || y > BS - 1)
    return NULL_POS;
  return make_pair (x, 1 << y);
}

int
str2i (string s, int minv, int maxv)
{
  int i = atoi (s.c_str ());
  if (i < minv)
    return minv;
  if (i > maxv)
    return maxv;
  return i;
}

VS
getline ()
{
  VS vs;
  string s;
  while (cin >> s, vs.push_back (s), cin.peek () != '\n');

  // cout << endl; // 只在 command(thesis) 时打开
  return vs;
}

VI
indexsort (const VI & vi)
{
  vector < PII > vpii;
  VI tmp;
  for (int i = 0; i < vi.size (); ++i)
    vpii.push_back (make_pair (vi[i], i));
  sort (vpii.begin (), vpii.end ());
  for (int i = 0; i < vpii.size (); ++i)
    tmp.push_back (vpii[i].second);
  return tmp;
}
