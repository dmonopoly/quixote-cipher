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

#include "NotationConstants.h"
#include "TagGrammarFinder.h"
#include "TrellisAid.h"

#define NUMBER_ITERATIONS 3

int main() {
  string filename_for_bigrams = "GivenCounts/corpus.spanish.sound_bigrams";
  map<string, double> data;  // Storage for probabilities and counts.
  vector<string> tag_list;
  bool found = TagGrammarFinder::FindTagGrammarFromFile(filename_for_bigrams,
                                                        &data, &tag_list);
  if (!found)
    return 0;

  string filename_for_cypher = "GivenCounts/corpus.spanish.quixote.written";
  vector<string> observed_data;
  vector<Node *> nodes;
  vector<Edge *> edges_to_update;
  vector<Edge *> all_edges; // for deletion later
  // TODO
  bool got_obs_data = CypherReader::GetObservedData(filename_for_cypher,
                                                    &observed_data);
  if (!got_obs_data)
    return 0;

  TrellisAid::BuildTrellis(&nodes, &edges_to_update, &all_edges, observed_data,
                           tag_list);
  TrellisAid::ForwardBackwardAndViterbi(NUMBER_ITERATIONS, nodes,
                                        edges_to_update, all_edges, &data);

  TrellisAid::DestroyTrellis(&nodes, &all_edges);
  return 0;
}
