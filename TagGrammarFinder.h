#ifndef TAG_GRAMMAR_H_
#define TAG_GRAMMAR_H_

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>

#include "EMViterbiPackage/Notation.h"

using namespace std;

namespace TagGrammarFinder {
  const string GIVEN_DELIM = "|";
  const string AND_DELIM = ",";
  const string SEQ_DELIM = " ";
  const string SIGMA = "SUM_i "; // key helper for Notation
  const string ARB_SOUND_PLACEHOLDER = "s_i";

  // Returns true if success.
  bool FindTagGrammarFromFile(const string &filename, map<Notation, double> *data,
                              vector<string> *tag_list);
}
#endif // End TAG_GRAMMAR_H_
