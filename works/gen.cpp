#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include "CIsoSurface.h"

using namespace std;

vector <string> split(const string _s, const string del);
int toInt(string s) {int r = 0; istringstream ss(s); ss >> r; return r;}

// ranges of x, y, z axis
const int xlow = -10000;
const int xhigh = 1000;
const int ylow = -10000;
const int yhigh = 10000;
const int zlow = 1000;
const int zhigh = 3000;

// value for compress coordinates from range camera
const int round = 30;

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      cout << "Usage: " << argv[0] << " <coordinateData.tsv>" << endl;
      return -1;
    }

  ifstream file (argv[1]);
  string line;
  vector <int> minis (3, 10000);
  vector <int> maxis (3, -10000);
  set <pair <int, pair <int, int> > > tmpvol;
  set <pair <int, pair <int, int> > > volume;
  CIsoSurface <short> *ciso = new CIsoSurface <short> ();

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

      if ((tmpi[0] < xlow || xhigh < tmpi[0]) ||
	  (tmpi[1] < ylow || yhigh < tmpi[1]) ||
	  (tmpi[2] < zlow || zhigh < tmpi[2]))
	continue;


      // to reduce size of scalar field,
      // round off coordinates
      for (unsigned int i=0; i<tmpi.size(); i++)
	{
	  int mid = round / 2;
	  int mod = tmpi[i] % round;
	  tmpi[i] /= round;
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
