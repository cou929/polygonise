#include <iostream>
#include <vector>
#include <cmath>

std::vector <std::vector <int> > generateSphere(int radius);

std::vector <std::vector <int> > generateSphere(int radius)
{
  std::vector <std::vector <int> > ret;

  for (int x=-radius; x<=radius; x++)
    for (int y=-radius; y<=radius; y++)
      for (int z=-radius; z<=radius; z++)
	{
	  int distance = (int)sqrt(x*x + y*y + z*z);
	  int range = 1;
	  if (distance - range <= (int)sqrt(radius*radius) && (int)sqrt(radius*radius) <= distance + range)
	    {
	      std::vector <int> tmp;
	      tmp.push_back(x); tmp.push_back(y); tmp.push_back(z);
	      ret.push_back(tmp);
	    }
	}

  return ret;
}

int main(int argc, char ** argv)
{
  std::vector <std::vector <int> > sphere;
  int radius = 5;

  if (argc == 2)
    radius = atoi(argv[1]);

  sphere = generateSphere(radius);

  for (int i=0; i<sphere.size(); i++)
    {
      for (int j=0; j<sphere[i].size(); j++)
	std::cout << sphere[i][j] << "\t";
      std::cout << std::endl;
    }

  return 0;
}

