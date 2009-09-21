#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"

using namespace std;

const int HEIGHT = 50;
const int WIDTH = 50;

int toInt(string s);
vector <int> split(const string _s, const string del);

int main(int argc, char ** argv)
{
  IplImage * img = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 1);
  IplImage * original = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 1);
  vector <vector <int> > coordinates;
  IplConvKernel* element = cvCreateStructuringElementEx (3, 3, 1, 1, CV_SHAPE_CROSS, NULL);
  int iteration = 2;
  ifstream file (argv[1]);
  string line;

  if (argc != 2 && argc != 3)
    {
      cout << "Usage: " << argv[0] << " <coordinate data tsv file>" << endl;
      return -1;
    }

  if (argc == 3)
    iteration = atoi(argv[2]);

  while (!file.eof())
    {
      vector <int> tmp;
      getline(file, line);
      tmp = split(line, "\t");
      coordinates.push_back(tmp);
    }

  cvSetZero(img);
  for (unsigned int i=0; i<coordinates.size(); i++)
    cvSet2D(img, coordinates[i][0], coordinates[i][1], cvScalar(coordinates[i][2]));

  cvCopy(img, original);

  cvErode (original, img, element, iteration);
  cvDilate (img, img, element, iteration);

  for (int h=0; h<HEIGHT; h++)
    for (int w=0; w<WIDTH; w++)
      {
	CvScalar s = cvGet2D(img, h, w);
	if (s.val[0] != 0)
	  cout << h << "\t" << w << "\t" << s.val[0] << endl;
      }

  //  cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
  //  cvNamedWindow("morph", CV_WINDOW_AUTOSIZE);
  //  cvShowImage("original", original);
  //  cvShowImage("morph", img);
  //  cvWaitKey(0);
  cvReleaseImage(&img);
    
  return 0;
}

int toInt(string s) {int r = 0; istringstream ss(s); ss >> r; return r;}

vector <int> split(const string _s, const string del)
{
  vector <int> ret;
  string s = _s;

  while (!s.empty())
    {
      size_t pos = s.find(del);
      string sub = "";
      sub = s.substr(0, pos);
      ret.push_back(toInt(sub));
      if (pos != string::npos)
	pos += del.size();
      s.erase(0, pos);
    }

  return ret;
}
