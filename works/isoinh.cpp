#include <iostream>
#include "CIsoSurface.h"

using namespace std;

class myIso : public CIsoSurface <double>
{
public:
  myIso();
  ~myIso();
  int printResult();
};

int myIso::printResult()
{


  return 0;
}

int main(int argc, char ** argv)
{
  //  myIso *mis = new myIso();



  return 0;
}
