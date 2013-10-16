// Reads count data and determines P(x), P(x|y), P(y|x), etc. for all tags x,y.

#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>

#include "em-training-example/Notation.h"

using namespace std;

#define EXTRA_PRINTING false

// TODO: could be moved to a better spot
const string GIVEN_DELIM = "|";
const string AND_DELIM = ",";
const string SEQ_DELIM = " ";
const string SIGMA = "SUM_i "; // key helper for Notation
const string ARB_SOUND_PLACEHOLDER = "s_i";

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "No filename given." << endl;
    return 0;
  } 
  string filename = argv[1];
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
      if (EXTRA_PRINTING)
        cout << "Read " << count << " " << sound1 << " " << sound2 << endl;
      // Remove quotes.
      sound1 = sound1.substr(1, sound1.size() - 2);
      sound2 = sound2.substr(1, sound2.size() - 2);
      sounds.insert(sound1);
      sounds.insert(sound2);
      Notation nc("C", {sound1}, SEQ_DELIM, {sound2});
      Notation nc_total(SIGMA + "C", {sound1}, SEQ_DELIM,
          {ARB_SOUND_PLACEHOLDER});
      bigram_counts[nc.repr()] = count; // should only encounter once 
      bigram_counts[nc_total.repr()] += count;
    }
    fin.close();

    // Determine tag grammar probabilities.
    for (auto s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      for (auto s2 = sounds.begin(); s2 != sounds.end(); ++s2) {
        Notation n("P", {*s2}, GIVEN_DELIM, {*s1});
        Notation nc("C", {*s1}, SEQ_DELIM, {*s2});
        Notation nc_total(SIGMA + "C", {*s1}, SEQ_DELIM,
            {ARB_SOUND_PLACEHOLDER});

        // If no key found, then just set to 0.
        if (bigram_counts.find(nc.repr()) == bigram_counts.end() || bigram_counts.find(nc_total.repr()) ==
            bigram_counts.end()) {
          if (EXTRA_PRINTING) {
            if (bigram_counts.find(nc.repr()) == bigram_counts.end())
              cout << "Not found: " << nc.repr() << endl;
            if (bigram_counts.find(nc_total.repr()) == bigram_counts.end())
              cout << "Not found: " << nc_total.repr() << endl;
          }
          data[n.repr()] = 0;
        } else {
          if (EXTRA_PRINTING)
            cout << bigram_counts.at(nc.repr()) << "/" << bigram_counts.at(nc_total.repr()) << endl;
          double val = (double) bigram_counts.at(nc.repr()) /
                        bigram_counts.at(nc_total.repr());
          data[n.repr()] = val;
        }
      }
    }
    for (auto it = data.begin(); it != data.end(); ++it) {
      cout << "data " << it->first << " = " << it->second << endl;
    }
  }

  return 0;
}
