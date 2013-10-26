#include <cassert>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>

using namespace std;

void test(set<string> *s) {
  for (auto i = s->begin(); i != s->end(); ++i) {
    *i = "hey";
  }
}
int main(int argc, char *argv[]) {
  set<string> here;
  here.insert("my life");
  here.insert("my life 2");
  test(&here);

  for (auto i = here.begin(); i != here.end(); ++i) {
    cout << *i << endl;
  }

  return 0;
}
