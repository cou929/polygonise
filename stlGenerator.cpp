#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "CIsoSurface.h"
#include "configuration.h"
#include "util.hpp"

using namespace std;

vector <vector <int> > getSearchRange(int pixel);
bool isInRange(int x, int y, int z, int len);
bool hasEnoughPoints(int x, int y, int z,   set <pair <int, pair <int, int> > > & volume, vector <vector <int> > & range);

double getrusageSec(){
  struct rusage t;
  struct timeval s;
  getrusage(RUSAGE_SELF, &t);
  s = t.ru_utime;
  return s.tv_sec + (double)s.tv_usec*1e-6;
}

struct args
{
  int xlow;
  int xhigh;
  int ylow;
  int yhigh;
  int zlow;
  int zhigh;
  int round[3];
  int thickness;
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
  ar.round[0] = atoi(confGet("roundx"));
  ar.round[1] = atoi(confGet("roundy"));
  ar.round[2] = atoi(confGet("roundz"));
  ar.thickness = atoi(confGet("thickness"));

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
	  int mid = ar.round[i] / 2;
	  int mod = tmpi[i] % ar.round[i];
	  tmpi[i] /= ar.round[i];
	  if (mod >= mid)
	    tmpi[i]++;

	  minis[i] = min(minis[i], tmpi[i]);
	  maxis[i] = max(maxis[i], tmpi[i]);
	}

      tmpvol.insert(make_pair(tmpi[0], make_pair(tmpi[1], tmpi[2])));
    }

  double t1 = getrusageSec();

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
  maxis[0] -= minis[0];
  maxis[1] -= minis[1];
  maxis[2] -= minis[2];
 
#if COVER
  // cover losted pixels. 
  // check around pixel and if there is enough points, mark current pixel as exist (value is 1).
  vector <vector <int> > range = getSearchRange(2);
  for (int x=0; x<=maxis[0]; x++)
    for (int y=0; y<=maxis[1]; y++)
      for (int z=0; z<=maxis[2]; z++)
	{
	  if (volume.find(make_pair(x, make_pair(y, z))) != volume.end())
	    continue;

	  if(hasEnoughPoints(x, y, z, volume, range))
	    volume.insert(make_pair(x, make_pair(y, z)));
	}
#endif

#if THICKNESS
  set <pair <int, pair <int, int> > > vtmp = volume;
  volume.clear();

  for (set <pair <int, pair <int, int> > >::iterator i=vtmp.begin(); i!=vtmp.end(); i++)
    for (int j=0; j<ar.thickness; j++)
      {
	volume.insert(make_pair(i->first, make_pair(i->second.first, i->second.second + j)));
      }
#endif

#if 1
  for (set <pair <int, pair <int, int> > >::iterator i=volume.begin(); i!=volume.end(); i++)
    {
      cerr << i->first << " " << i->second.first << " " << i->second.second << endl;
    }
  cerr << "x min, x max: " << minis[0] << ", " << maxis[0] << endl;
  cerr << "y min, y max: " << minis[1] << ", " << maxis[1] << endl;
  cerr << "z min, z max: " << minis[2] << ", " << maxis[2] << endl;
#endif

  ciso->GenerateSurface(volume, 1, maxis[0]-1, maxis[1]-1, maxis[2]-1, 1, 1, 1);
  ciso->printSTLAscii();
  double t2 = getrusageSec();
  cerr << "Generated " << t2 - t1 << " sec" << endl;

  return 0;
}

vector <vector <int> > getSearchRange(int pixel)
{
  vector <vector <int> > ret;

  for (int x=-pixel; x<=pixel; x++)
    for (int y=-pixel; y<=pixel; y++)
      for (int z=-pixel; z<=pixel; z++)
	if (isInRange(x, y, z, pixel))
	  {
	    vector <int> tmp(3);
	    tmp[0] = x;
	    tmp[1] = y;
	    tmp[2] = z;
	    ret.push_back(tmp);
	  }

  return ret;
}

bool isInRange(int x, int y, int z, int len)
{
  bool ret = false;

  if (x*x + y*y + z*z <= len*len)
    ret = true;

  return ret;
}

bool hasEnoughPoints(int x, int y, int z, set <pair <int, pair <int, int> > > & volume, vector <vector <int> > & range)
{
  bool ret = false;
  int pointNum = 0;
  double threshold = 0.6;

  for (int i=0; i<(int)range.size(); i++)
    if (volume.find(make_pair(x+range[i][0], make_pair(y+range[i][1], z+range[i][2]))) != volume.end())
      pointNum++;

  if (((double)pointNum / (double)range.size()) >= threshold)
    ret = true;

  return ret;
}
