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

      for (int i=0; i<tmpStr.size(); i++)
	{
	  int n = toInt(tmpStr[i]);
	  tmpi.push_back(n);
	  minis[i] = min(minis[i], n);
	}

      if ((tmpi[0] == 0 || tmpi[0] == -1) &&
	  (tmpi[1] == 0 || tmpi[1] == -1) &&
	  (tmpi[2] == 0 || tmpi[2] == -1))
	continue;

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

  cout << volume.begin()->first << endl;
  cout << volume.end()->first << endl;

  ciso->GenerateSurface(volume, 1, volume.end()->first, volume.end()->second.first, volume.end()->second.second, 1, 1, 1);
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
