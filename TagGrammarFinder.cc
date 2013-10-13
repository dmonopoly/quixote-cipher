// Reads count data and determines P(x), P(x|y), P(y|x), etc. for all tags x,y.

#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

const string KEY_DELIM = "|";

int main() {
  string filename = "GivenCounts/corpus.spanish.sound_bigrams";
  ifstream fin(filename.c_str());
  if (fin.fail())
    cerr << "Could not open file " << filename << endl;
  else {
    map<string, int> bigram_counts;
    int count;
    string sound1, sound2;
    while (true) {
      fin >> count;
      if (fin.eof())
        break;
      fin >> sound1 >> sound2;
      // Remove quotes.
      sound1 = sound1.substr(1, sound1.size() - 2);
      sound2 = sound2.substr(1, sound2.size() - 2);
      bigram_counts[sound2 + KEY_DELIM + sound1] += count;
    }
  }

  return 0;
}
