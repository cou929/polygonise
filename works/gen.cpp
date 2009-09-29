#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include "CIsoSurface.h"
#include "configuration.h"
#include "util.hpp"

using namespace std;

struct args
{
  int xlow;
  int xhigh;
  int ylow;
  int yhigh;
  int zlow;
  int zhigh;
  int round;
};

int main(int argc, char **argv)
{
  if (argc != 3)
    {
      cout << "Usage: " << argv[0] << " <coordinateData.tsv> <configuration file>" << endl;
      return -1;
    }

  ifstream file (argv[1]);
  string line;
  vector <int> minis (3, 10000);
  vector <int> maxis (3, -10000);
  set <pair <int, pair <int, int> > > tmpvol;
  set <pair <int, pair <int, int> > > volume;
  CIsoSurface <short> *ciso = new CIsoSurface <short> ();
  struct args ar;

  confLoad(argv[2], "\t");
  ar.xlow = atoi(confGet("xlow"));
  ar.xhigh = atoi(confGet("xhigh")); 
  ar.ylow = atoi(confGet("ylow"));
  ar.yhigh = atoi(confGet("yhigh")); 
  ar.zlow = atoi(confGet("zlow"));
  ar.zhigh = atoi(confGet("zhigh")); 
  ar.round = atoi(confGet("round"));

  while (!file.eof())
    {
      vector <int> tmpi;
      getline(file, line);
      tmpi = split(line, "\t");

      if (tmpi.empty())
	break;

      if ((tmpi[0] < ar.xlow || ar.xhigh < tmpi[0]) ||
	  (tmpi[1] < ar.ylow || ar.yhigh < tmpi[1]) ||
	  (tmpi[2] < ar.zlow || ar.zhigh < tmpi[2]))
	continue;


      // to reduce size of scalar field,
      // round off coordinates
      for (unsigned int i=0; i<tmpi.size(); i++)
	{
	  int mid = ar.round / 2;
	  int mod = tmpi[i] % ar.round;
	  tmpi[i] /= ar.round;
	  if (mod >= mid)
	    tmpi[i]++;

	  minis[i] = min(minis[i], tmpi[i]);
	  maxis[i] = max(maxis[i], tmpi[i]);
	}

      tmpvol.insert(make_pair(tmpi[0], make_pair(tmpi[1], tmpi[2])));
    }

  // Transform each coordinate value to start with 0.
  // For example, supporse a set as (-2, -1, 1, 5),
  // transform it to (0, 1, 2, 7).
  for (set <pair <int, pair <int, int> > >::iterator i=tmpvol.begin(); i!=tmpvol.end(); i++)
    {
      pair <int, pair <int, int> > p = *i;
      p.first -= minis[0];
      p.second.first -= minis[1];
      p.second.second -= minis[2];
      volume.insert(p);
    }

#if DEBUG
  for (set <pair <int, pair <int, int> > >::iterator i=volume.begin(); i!=volume.end(); i++)
    {
      cerr << i->first << " " << i->second.first << " " << i->second.second << endl;
    }
#endif

  ciso->GenerateSurface(volume, 1, maxis[0]-1, maxis[1]-1, maxis[2]-1, 1, 1, 1);
  ciso->printSTLAscii();

  return 0;
}
