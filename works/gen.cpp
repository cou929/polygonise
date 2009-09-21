#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include "CIsoSurface.h"

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

vector <string> split(const string _s, const string del);
int toInt(string s) {int r = 0; istringstream ss(s); ss >> r; return r;}
int getArgs(char * filename, struct args *a);

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

  getArgs(argv[2], &ar);

  cout << "xlow\t" << ar.xlow << endl;
  cout << "ylow\t" << ar.ylow << endl;
  cout << "round\t" << ar.round << endl;

  while (!file.eof())
    {
      vector <string> tmpStr;
      vector <int> tmpi;
      getline(file, line);
      tmpStr = split(line, "\t");

      if (tmpStr.empty())
	break;

      for (unsigned int i=0; i<tmpStr.size(); i++)
	{
	  int n = toInt(tmpStr[i]);
	  tmpi.push_back(n);
	}

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
  // For example, supporse to set as (-2, -1, 1, 5),
  // transform it to (0, 1, 2, 7).
  for (set <pair <int, pair <int, int> > >::iterator i=tmpvol.begin(); i!=tmpvol.end(); i++)
    {
      pair <int, pair <int, int> > p = *i;
      p.first -= minis[0];
      p.second.first -= minis[1];
      p.second.second -= minis[2];
      volume.insert(p);
    }

  for (set <pair <int, pair <int, int> > >::iterator i=volume.begin(); i!=volume.end(); i++)
    {
      cerr << i->first << " " << i->second.first << " " << i->second.second << endl;
    }

  // complete (cover) losted pixels on scalar field 
  char dirx[6] = {0, 1, 0, -1, 0, 0};
  char diry[6] = {1, 0, -1, 0, 0, 0};
  char dirz[6] = {0, 0, 0, 0, 1, -1};

  for (int z=0; z<maxis[2]-1; z++)
    for (int y=0; y<maxis[1]-1; y++)
      for (int x=0; x<maxis[0]-1; x++)
	{
	  if (volume.find(make_pair(x, make_pair(y, z))) == volume.end())
	    {
	      int countAround = 0;
	      for (int i=0; i<6; i++)
		{
		  int curx = x + dirx[i];
		  int cury = y + diry[i];
		  int curz = z + dirz[i];
		  if (volume.find(make_pair(curx, make_pair(cury, curz))) != volume.end())
		    countAround++;
		}
	      if (countAround > 3)
		volume.insert(make_pair(x, make_pair(y, z)));
	    }
	}

  ciso->GenerateSurface(volume, 1, maxis[0]-1, maxis[1]-1, maxis[2]-1, 1, 1, 1);
  ciso->printSTLAscii();

  return 0;
}

vector <string> split(const string _s, const string del)
{
  vector <string> ret;
  string s = _s;

  while (!s.empty())
    {
      size_t pos = s.find(del);
      string sub = "";
      sub = s.substr(0, pos);
      ret.push_back(sub);
      if (pos != string::npos)
	pos += del.size();
      s.erase(0, pos);
    }

  return ret;
}

int getArgs(char * filename, struct args *a)
{
  ifstream file (filename);
  string line;

  while (!file.eof())
    {
      char n[10];
      string name;
      int param = 0;
      getline(file, line);
      sscanf(line.c_str(), "%s\t%d\n", n, &param);
      name = n;

      if (name == "xlow")
	a->xlow = param;
      else if (name == "xhigh")
	a->xhigh = param;
      else if (name == "ylow")
	a->ylow = param;
      else if (name == "yhigh")
	a->yhigh = param;
      else if (name == "zlow")
	  a->zlow = param;
      else if (name == "zhigh")
	  a->zhigh = param;
      else if (name == "round")
	a->round = param;
    }

  return 0;
}
