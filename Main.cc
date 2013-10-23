#include <cassert>
#include <ctime>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "em-training-example/BasicHelper.h"
#include "em-training-example/NLPHelper.h"
#include "em-training-example/Notation.h"
#include "em-training-example/Node.h"
#include "em-training-example/Edge.h"
#include "em-training-example/NotationConstants.h"
#include "em-training-example/TrellisAid.h"

#include "CypherReader.h"
#include "TagGrammarFinder.h"

#define NUMBER_ITERATIONS 3

#define EXTRA_PRINTING true

void PrepareObsTagProbs(map<string, double> *data,
                         const vector<string> &observed_data,
                         const vector<string> &tag_list) {
  // Sets initial observed char / tag probabilities. Can seed to .5 or
  // randomize.
  for (auto obs = observed_data.begin(); obs != observed_data.end(); ++obs) {
    for (auto tag = tag_list.begin(); tag != tag_list.end(); ++tag) {
      Notation nObsTagProb("P", {*obs}, Notation::GIVEN_DELIM, {*tag});
      (*data)[nObsTagProb.repr()] = .5;
    }
  }
  // Also seed NotationConstants.
  (*data)[NotationConstants::p1.repr()] = 1;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: ./<exec> <bigram-counts-file> <cyphertext>" << endl;
    return 0;
  }
  string filename_for_bigrams = argv[1];
  map<string, double> data;  // Storage for probabilities and counts.
  vector<string> tag_list;
  bool found = TagGrammarFinder::FindTagGrammarFromFile(filename_for_bigrams,
                                                        &data, &tag_list);
  if (!found)
    return 0;
  else if (EXTRA_PRINTING)
    cout << "Found tag grammar.\n";

  string filename_for_cypher = argv[2];
  vector<string> observed_data;
  vector<Node *> nodes;
  vector<Edge *> edges_to_update;
  vector<Edge *> all_edges; // for deletion later
  bool got_obs_data = CypherReader::GetObservedData(filename_for_cypher,
                                                    &observed_data);
  if (!got_obs_data)
    return 0;
  else if (EXTRA_PRINTING)
    cout << "Found cyphertext.\n";

  PrepareObsTagProbs(&data, observed_data, tag_list);

  TrellisAid::BuildTrellis(&nodes, &edges_to_update, &all_edges, observed_data,
                           tag_list);
  if (EXTRA_PRINTING) {
    cout << "Built trellis.\n";
    // if you want to write probabilities
//     for (auto it = data.begin(); it != data.end(); ++it) {
//       cout << it->first << " " << it->second << endl;
//     }
  }
  TrellisAid::ForwardBackwardAndViterbi(NUMBER_ITERATIONS, nodes,
                                        edges_to_update, all_edges, &data,
                                        observed_data);

  TrellisAid::DestroyTrellis(&nodes, &all_edges);
  return 0;
}
