#include "go.h"
#include "tsumego.h"
#include "fir.h"
#include "inout.h"
#include "test.h"

TIMER ___, gametimer;
INIT g_init;
LEARN g_learn;
COMMAND g_cmd;

int
main (int argc, char *argv[])
{
  command (BIND < string, char *>()(1, " "));
  for (;;)
    {
      cout << endl << ">> ";
      command (getline ());
    }
  return 0;
}

void
command (VS in)
{
  GO go, g;
  ITR itr;
  switch (g_cmd.which (in[0]))
    {
    case VERSION_C:
      cout << "ShoutGo 1.0 " << endl << "by ShoutGo (shoutgo@gmail.com)" << endl << "All rights reserved " << endl;
      break;
    case PLAY_C:
      play < BRAIN > ();
      break;
    case FIR_C:
      play < FIR > ();
      break;
    case REVIEW_C:
      review < INFOGO > ();
      break;
    case SGFTREE_C:
      sgftree < INFOGO > ();
      break;
    case INPUTPAT_C:
      //TEST_IO().inputpat(); 
      break;
    case EXIT_C:
      exit (0);
      break;
    default:
      ;
    }
  command_test (in);
}

void
command_test (VS in)
{
  if (g_cmd.which (in.back ()) == BACK_SAVE2TXT_C)
    {
      ofstream file ("GO.txt", ios::app);
      if (!file)
        cerr << "GO.txt" << " : open fail !" << endl;
      streambuf *old = cout.rdbuf ();
      cout.rdbuf (file.rdbuf ());
      in.pop_back ();
      command_test (in);
      cout.rdbuf (old);
      command_test (in);
      return;
    }
  switch (g_cmd.which (in[0]))
    {
    case HELP_C:
      if (in.size () > 1)
        g_cmd.help (g_cmd.which (in[1]));

      else
        g_cmd.help ();
      break;
    case SIZE_C:
      testsize ();
      break;
    default:
      ;
    }
}

void
command_state (const INFOGO & info, VS in)
{
  if (g_cmd.which (in.back ()) == BACK_SAVE2TXT_C)
    {
      ofstream file ("GO.txt", ios::app);
      if (!file)
        cerr << "GO.txt" << " : open fail !" << endl;
      streambuf *old = cout.rdbuf ();
      cout.rdbuf (file.rdbuf ());
      in.pop_back ();
      command_state (info, in);
      cout.rdbuf (old);
      command_state (info, in);
      return;
    }
  int m;
  string s;
  VS vs;
  VI vi;
  BITB bb;
  MOVE move;
  INFOGO inf;
  INFOGO *infp;
  ITR itr;
  switch (g_cmd.which (in[0]))
    {
    case PASS_C:
      break;
    case END_C:
      break;
    case HISTORY_C:
      cout << info.gethistory () << endl;
      break;
    case SHOWBOARD_C:
      cout << info;
      break;
    case DELTAMASSMAPS_C:
      if (!info.father)
        break;
      if (in.size () > 1)
        m = str2i (in[1], 0, info.area.massmaps.size () - 1);
      else
        m = info.area.massmaps.size () - 1;
      INOUT ().print (info, info.area.massmaps[m] ^ info.father->area.massmaps[m]);
      break;
    case MASSMAPS_C:
      if (in.size () > 1)
        m = str2i (in[1], 0, info.area.massmaps.size () - 1);
      else
        m = info.area.massmaps.size () - 1;
      INOUT ().print (info, info.area.massmaps[m]);
      break;
    case CLUSTER_C:
      INOUT ().print (info, info.area.clustermap);
      break;
    case POTENTIAL_C:
      INOUT ().print (info, info.area.potentialmap);
      break;
    case PUBLICAREA_C:
      cout << info (info.area.publicarea ());
      break;
    case ALIVE_C:
      INOUT ().print (info, GO (info.area.getclustermap (BLACK, ALIVE), info.area.getclustermap (WHITE, ALIVE)));
      break;
    case DEAD_C:
      INOUT ().print (info, GO (info.area.getclustermap (BLACK, DEAD), info.area.getclustermap (WHITE, DEAD)));
      break;
    case UNCLEAR_C:
      INOUT ().print (info, GO (info.area.getclustermap (BLACK, UNCLEAR), info.area.getclustermap (WHITE, UNCLEAR)));
      break;
    case LAND_C:
      break;
    case SEA_C:
      INOUT ().print (info, info.area.clustermap);
      break;
    case AIR_C:
      break;
    case SCAN_C:
      ___ASSERT (info.inc == info.scan ());
      clog << "Scan over ." << endl;
      break;
    case TRYMOVE_C:
      if (in.size () < 3)
        break;
      inf = info;
      if (in[1][0] == '-')
        {
          infp = &inf;
          m = str2i (in[1], INT_MIN, -1);
          for (int i = m; i < 0 && infp->father; ++i)
            infp = infp->father;
        }
      else
        {
          infp = inf.move_tree (INOUT ().in2pos (inf.getlastpos (), in[1]), inf.turn ());
          infp->update ();
        }
      reverse (in.begin (), in.end ());
      in.pop_back ();
      in.pop_back ();
      reverse (in.begin (), in.end ());
      command_state (*infp, in);
      break;
    case ZOBRIST_C:
      if (in.size () > 1)
        m = str2i (in[1], 0, ZOBRIST::ZOBRIST_KIND - 1);
      else
        m = ZOBRIST::ZOBRIST_KIND - 1;
      vi = info.getf_zobrist (m, FULL_BB, info.turn ());
      for (POS pos = itr.posbegin (); !itr.posend (); pos = itr.posnext ())
        if (vi[pos2i (pos)] > 0)
          bb |= pos;
      cout << info (bb);
      break;
    case PEMIS_C:
      if (in.size () > 1)
        m = str2i (in[1], 0, PATFREQ::PEMIS_KIND - 1);
      else
        m = PATFREQ::PEMIS_KIND - 1;
      inf = info;
      vi = inf.getf_patfreq (m, FULL_BB, inf.turn ());
      for (POS pos = itr.posbegin (); !itr.posend (); pos = itr.posnext ())
        if (vi[pos2i (pos)] > 1)
          bb |= pos;
      cout << inf (bb);
      break;
    case SAVE_C:
      INOUT ().save (info, "info");
      break;
    case SAVE2SGF_C:
      inf = info;
      infp = &inf;
      INOUT ().infop2sgf (infp, "go.txt");
      clog << "Saved in sgf file ." << endl;
      break;
    default:
      ;
    }
}
