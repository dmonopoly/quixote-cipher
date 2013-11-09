#include <cassert>
#include <cfloat>
#include <ctime>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>

#include "EMViterbiPackage/BasicHelper.h"

using namespace std;

int main(int argc, char *argv[]) {
  double x = .4;
  double y = .4;
  for (x = 0; x < 1; x += .1) {
    for (y = 0; y < 1; y += .1) {
      cout << Basic::AddLogs(x, y) << endl;
    }
  }
  return 0;
}
