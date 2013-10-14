// Reads count data and determines P(x), P(x|y), P(y|x), etc. for all tags x,y.

#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>

#include "em-training-example/Notation.h"

using namespace std;

// TODO: could be moved to a better spot
const string GIVEN_DELIM = "|";
const string AND_DELIM = ",";
const string SEQ_DELIM = "";
const string SIGMA = "SUM_i "; // key helper for Notation
const string ARB_SOUND_PLACEHOLDER = "s_i";

int main() {
  string filename = "GivenCounts/corpus.spanish.sound_bigrams";
  ifstream fin(filename.c_str());
  if (fin.fail())
    cerr << "Could not open file " << filename << endl;
  else {
    map<string, double> data;
    set<string> sounds;
    map<string, int> bigram_counts; // Key: Notation string repr.
    // Read bigram counts from file.
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
      sounds.insert(sound1);
      sounds.insert(sound2);
      Notation n("C", {sound1}, SEQ_DELIM, {sound2});
      Notation nc_total(SIGMA + "C", {sound1}, SEQ_DELIM,
          {ARB_SOUND_PLACEHOLDER});
      bigram_counts[n.repr()] += count;
    }
    fin.close();

    // Determine tag grammar probabilities.
    for (auto s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      for (auto s2 = sounds.begin(); s2 != sounds.end(); ++s2) {
        Notation n("P", {*s2}, GIVEN_DELIM, {*s1});
        Notation nc("C", {*s1}, SEQ_DELIM, {*s2});
        Notation nc_total(SIGMA + "C", {*s1}, SEQ_DELIM,
            {ARB_SOUND_PLACEHOLDER});
        data[n.repr()] = (double) bigram_counts.at(nc.repr()) /
          bigram_counts.at(nc_total.repr());
      }
    }
    for (auto it = data.begin(); it != data.end(); ++it) {
      cout << "data " << it->first << " = " << it->second << endl;
    }
  }

  return 0;
}
