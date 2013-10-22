#ifndef CYPHER_READER_H_
#define CYPHER_READER_H_

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

namespace CypherReader {
  // Returns true if success.
  bool GetObservedData(const string &filename_for_cypher,
                       vector<string> *observed_data);
}

#endif  // CYPHER_READER_H_
