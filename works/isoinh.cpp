#include "CIsoSurface.h"

int main(int argc, char ** argv)
{
  CIsoSurface <short> *ciso = new CIsoSurface <short> ();

  short volume[3][3][3] = {
    0, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 0, 0, 0, 0,
  };

  ciso->GenerateSurface(&volume[0][0][0], 1, 3, 3, 3, 1, 1, 1);
  ciso->printSTLAscii();

  return 0;
}
