#include <iostream>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
using namespace std;

int main(int argc, char ** argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "Usage: %s <coordinate data>\n", argv[0]);
      return -1;
    }

  FILE * fh;
  fh = fopen(argv[1], "r");
  char line[50];

  while (fgets(line, sizeof(line), fh) != NULL)
    {
      float x, y, z;
      int u, v;
      sscanf(line, "%f\t%f\t%f\t%d\t%d\n", &x, &y, &z, &u, &v);
      printf("%d\t%d\t%d\n", u, v, (int)z);
    }

  return 0;
}
