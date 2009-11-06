#include <iostream>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
using namespace std;

int main(int argc, char ** argv)
{
  if (argc != 3)
    {
      fprintf(stderr, "Usage: %s <depth image> <coordinate data>\n", argv[0]);
      return -1;
    }

  IplImage * img = cvLoadImage(argv[1]);

  FILE * fh;
  fh = fopen(argv[2], "r");
  char line[50];

  while (fgets(line, sizeof(line), fh) != NULL)
    {
      float x, y, z;
      int u, v;
      sscanf(line, "%f\t%f\t%f\t%d\t%d\n", &x, &y, &z, &u, &v);
      CvScalar s = cvGet2D(img, v, u);
      printf("%d\t%d\t%d\n", u, v, (int)s.val[0]);
    }

  return 0;
}
