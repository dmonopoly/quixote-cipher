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
#include <utility>

#include "EMViterbiPackage/BasicHelper.h"
#include "EMViterbiPackage/Notation.h"

using namespace std;

void TestLogs(double x, double y) {
  cout << "log(" << x << ") and log(" << y << ")" << endl;
  cout << Basic::AddLogs(log(x), log(y)) << " = log(" << x + y << ") =? " << log(x+y) << endl;
}

int main(int argc, char *argv[]) {
//   Notation n1("P", {"A"}, Notation::GIVEN_DELIM, {"B"});
//   map<Notation, double> data;
//   data[n1] = 1.5;
//   Notation n2("P", {"A"}, Notation::GIVEN_DELIM, {"B"});
//   data[n2] = .75;
//   for (auto it = data.begin(); it != data.end(); ++it) {
//     cout << it->first << " " << it->second << endl;
//   }

  cout << Basic::AddLogs(-.27773, -.2145) << endl;
  TestLogs(.2, .5);
  TestLogs(exp(-5.02865), exp(-5.06655));
  TestLogs(exp(-.27773), exp(-.2145));

  return 0;
}
