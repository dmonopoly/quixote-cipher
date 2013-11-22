#ifndef CYPHER_READER_H_
#define CYPHER_READER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

namespace CypherReader {
  // Obtains observed data from a file where each character is separated by a
  // space and each space in the observed data is represented by an underscore.
  // Returns true if success. Also stores a set of unique observed cypher
  // symbols.
  bool GetObservedData(const string &filename, vector<string> *observed_data,
                       set<string> *obs_symbols);
}

#endif  // CYPHER_READER_H_
