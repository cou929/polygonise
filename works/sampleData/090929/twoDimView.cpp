#include <iostream>
#include <cstdio>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
using namespace std;

#define LINE_MAX_NUM 80
#define IMG_WIDTH 176
#define IMG_HEIGHT 144
#define WHITE 256

int main(int argc, char ** argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "Usage: %s <coordinate file name>\n", argv[0]);
      return -1;
    }

  FILE *fh;
  char line[LINE_MAX_NUM];
  IplImage * img = cvCreateImage(cvSize(IMG_WIDTH, IMG_HEIGHT), IPL_DEPTH_8U, 1);

  fh = fopen(argv[1], "r");
  cvSetZero(img);

  while (fgets(line, sizeof(line), fh) != NULL)
    {
      double x, y, z;
      int u, v;
      sscanf(line, "%f\t%f\t%f\t%d\t%d", &x, &y, &z, &u, &v);
      cvSet2D(img, v, u, cvScalar(WHITE));
    }

  cvNamedWindow("result");
  cvShowImage("result", img);

  cvWaitKey(0);

  cvReleaseImage(&img);
  fclose(fh);

  return 0;
}
