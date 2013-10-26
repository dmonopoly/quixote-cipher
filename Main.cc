#include <cassert>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
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

using namespace std;

#define NUMBER_ITERATIONS 3

#define EXTRA_PRINTING true

void DisambiguateDuplicates(const set<string> &obs_symbols,
                            vector<string> *tag_list,
                            map<string, double> *data) {
  // Alters all tags that have overlapping meanings with observed symbols.
  for (auto i = obs_symbols.begin(); i != obs_symbols.end(); ++i) {
    for (auto j = tag_list->begin(); j != tag_list->end(); ++j) {
      cout << "comparing " << *i << " with " << *j << endl;
      if (*i == *j) {
        stringstream ss;
        ss << *j << "'";
        // TODO update data...
        *j = ss.str();
      }
    }
  }
}

void PrepareObsTagProbs(const vector<string> &observed_data,
                        const vector<string> &tag_list,
                        const set<string> &obs_symbols,
                        map<string, double> *data) {
  // Sets initial observed char / tag probabilities. Can seed to uniform
  // probability (1/# of unique observed symbols) or randomize.
  for (auto obs = observed_data.begin(); obs != observed_data.end(); ++obs) {
    for (auto tag = tag_list.begin(); tag != tag_list.end(); ++tag) {
      Notation nObsTagProb("P", {*obs}, Notation::GIVEN_DELIM, {*tag});
      (*data)[nObsTagProb.repr()] = (double) 1/obs_symbols.size();
    }
  }
  // Deal with spaces in the substitute table: set P(any obs|space tag) to 0 and
  // P(space tag|space obs) to 1.
  // "_" means "space" in the ciphertext letter sequence.
  // "_" means "pause" in the spoken spanish plaintext. Altered to "_'" for
  // uniqueness in DisambiguateDuplicates.
  for (auto obs = observed_data.begin(); obs != observed_data.end(); ++obs) {
    Notation nAnythingGivenSpaceTag("P", {*obs}, Notation::GIVEN_DELIM, {"_'"});
    (*data)[nAnythingGivenSpaceTag.repr()] = 0;
  }
  Notation nSpaceTagGivenSpaceObs("P", {"_'"}, Notation::GIVEN_DELIM, {"_"});
  (*data)[nSpaceTagGivenSpaceObs.repr()] = 1;

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
  set<string> obs_symbols;
  vector<Node *> nodes;
  vector<Edge *> edges_to_update;
  vector<Edge *> all_edges; // for deletion later
  bool got_obs_data = CypherReader::GetObservedData(filename_for_cypher,
                                                    &observed_data,
                                                    &obs_symbols);
  if (!got_obs_data)
    return 0;
  else if (EXTRA_PRINTING)
    cout << "Found cyphertext.\n";

  DisambiguateDuplicates(obs_symbols, &tag_list, &data);
//   cout << "new tags: \n";
//   for (auto i = tag_list.begin(); i != tag_list.end(); ++i) {
//     cout << *i << endl;
//   }
//   cout << "end tags\n";
  PrepareObsTagProbs(observed_data, tag_list, obs_symbols, &data);

  TrellisAid::BuildTrellis(&nodes, &edges_to_update, &all_edges, observed_data,
                           tag_list);
  if (EXTRA_PRINTING) {
    cout << "Built trellis.\n";
    // if you want to write probabilities
    cout << "printing probs...";
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
