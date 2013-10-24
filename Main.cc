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
      // TODO: make 1 / number of uniq observed characters. also check too
      // low...
      (*data)[nObsTagProb.repr()] = .1;
    }
  }
  // Deal with spaces in the substitute table: set P(any obs|space) to 0 and
  // P(space|space) to 1.
  // "_" means "space" in the ciphertext letter sequence.
  // "_" means "pause" in the spoken spanish plaintext.
  for (auto obs = observed_data.begin(); obs != observed_data.end(); ++obs) {
    Notation nAnythingGivenSpace("P", {*obs}, Notation::GIVEN_DELIM, {"_"});
    (*data)[nAnythingGivenSpace.repr()] = 0;
  }
  Notation nSpaceGivenSpace("P", {"_"}, Notation::GIVEN_DELIM, {"_"});
  (*data)[nSpaceGivenSpace.repr()] = 1;

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
    for (auto it = data.begin(); it != data.end(); ++it) {
      cout << it->first << " " << it->second << endl;
    }
  }
  cout << NUMBER_ITERATIONS << " iterations:" << endl;

  Notation nObsSeq("P", observed_data, Notation::SEQ_DELIM);
  vector<double> saved_obs_seq_probs;
  bool very_small_data_set = false;
  TrellisAid::ForwardBackwardAndViterbi(NUMBER_ITERATIONS, nodes,
                                        edges_to_update, all_edges, &data,
                                        observed_data, nObsSeq,
                                        &saved_obs_seq_probs,
                                        very_small_data_set);
  TrellisAid::DestroyTrellis(&nodes, &all_edges);

  ofstream fout("observed_data_probabilities.txt");
  for (int i = 0; i < saved_obs_seq_probs.size(); ++i) {
    fout << saved_obs_seq_probs[i] << endl;
  }
  cout << "Values of " << nObsSeq << " have been written to "
    "observed_data_probabilities.txt." << endl << endl;
  return 0;
}
