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

using namespace std;

int main(int argc, char *argv[]) {
  map<string, double> data;
  data.insert(make_pair("a", .123));
  data.insert(make_pair("b", .123));
  for (auto it = data.begin(); it != data.end(); ++it) {
    cout << it->first << ", " << it->second << endl;
  }
  cout << "--" << endl;
  for (auto it = data.begin(); it != data.end(); ++it) {
    it->second = log(it->second);
  }

  for (auto it = data.begin(); it != data.end(); ++it) {
    cout << it->first << ", " << it->second << endl;
  }
  cout << -DBL_MAX << endl;
  cout << "--" << endl;
  double a = .5;
  double b = .3;
  cout << "Actual log(a+b): " << log(a +b) << endl;
  cout << "From my method: " << Basic::AddLogs(log(a), log(b)) << endl;
  cout << "Actual log(a-b): " << log(a - b) << endl;
  cout << "From my method: " << Basic::SubtractLogs(log(a), log(b)) << endl;
  cout << "good so far.\n";
  a = log(.5);
  b = log(.3);
  double c = Basic::AddLogs(a, b);
  cout << "c: " << c << endl;
  double d = Basic::SubtractLogs(c, b);
  cout << "These should be equal: " << d << " and " << a << endl;
  return 0;
}
