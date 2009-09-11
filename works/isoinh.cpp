#include "CIsoSurface.h"
#include <set>
#include <iostream>

int main(int argc, char ** argv)
{
  CIsoSurface <short> *ciso = new CIsoSurface <short> ();
  std::set <std::pair <int, std::pair <int, int> > > volume;
  volume.insert(std::make_pair(0, std::make_pair(1, 1)));
  volume.insert(std::make_pair(1, std::make_pair(0, 1)));
  volume.insert(std::make_pair(1, std::make_pair(1, 0)));
  volume.insert(std::make_pair(1, std::make_pair(1, 1)));
  volume.insert(std::make_pair(1, std::make_pair(1, 2)));
  volume.insert(std::make_pair(1, std::make_pair(2, 1)));
  volume.insert(std::make_pair(2, std::make_pair(1, 1)));

  ciso->GenerateSurface(volume, 1, 2, 2, 2, 1, 1, 1);
  //  ciso->printSTLAscii();

  return 0;
}
