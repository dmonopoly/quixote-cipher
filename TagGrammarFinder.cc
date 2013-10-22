// Reads count data and determines P(x), P(x|y), P(y|x), etc. for all tags x,y.
// Main method outputs results (quick test).

#include "TagGrammarFinder.h"

#define DO_MAIN true

#define EXTRA_PRINTING false

bool TagGrammarFinder::FindTagGrammarFromFile(const string &filename,
                                              map<string, double> *data,
                                              vector<string> *tag_list) {
  ifstream fin(filename.c_str());
  if (fin.fail()) {
    cerr << "Could not open file " << filename << endl;
    return false;
  } else {
    set<string> sounds;
    map<string, int> unigram_counts; // Key: Notation string repr.
    map<string, int> bigram_counts; // Key: Notation string repr.
    // Read bigram counts from file.
    int count;
    string sound1, sound2;
    // SUM_i C(s_i)
    Notation n_count_total(SIGMA + "C", {ARB_SOUND_PLACEHOLDER});
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

      Notation n_count_seq("C", {sound1}, TagGrammarFinder::SEQ_DELIM, {sound2});
      Notation n_count_seq_total(SIGMA + "C", {sound1}, TagGrammarFinder::SEQ_DELIM,
          {ARB_SOUND_PLACEHOLDER});
      bigram_counts[n_count_seq.repr()] = count; // should only encounter once 
      bigram_counts[n_count_seq_total.repr()] += count;

      // Single probabilities. Treat C(s1) = SUM_i C(s1 s_i).
      Notation n_count("C", {sound1});  // C(s1), followed by SUM_i C(s_i).
      unigram_counts[n_count.repr()] += count;
      unigram_counts[n_count_total.repr()] += count;
    }
    fin.close();

    // Determine tag grammar probabilities.
    // Unigrams.
    for (auto s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      Notation nSingle("P", {*s1});
      Notation n_count("C", {*s1});
      try {
        (*data)[nSingle.repr()] = (double) unigram_counts.at(n_count.repr()) / 
                                  unigram_counts.at(n_count_total.repr());
      } catch (out_of_range &e) {
        cerr << "Out of range error in unigram stuff: " << e.what() << endl;
      }
    }
    // Bigrams.
    for (auto s1 = sounds.begin(); s1 != sounds.end(); ++s1) {
      for (auto s2 = sounds.begin(); s2 != sounds.end(); ++s2) {
        Notation nGiven("P", {*s2}, TagGrammarFinder::GIVEN_DELIM, {*s1});
        Notation n_count_seq("C", {*s1}, TagGrammarFinder::SEQ_DELIM, {*s2});
        Notation n_count_seq_total(SIGMA + "C", {*s1}, TagGrammarFinder::SEQ_DELIM,
            {ARB_SOUND_PLACEHOLDER});

        // If no key found, then just set to 0.
        if (bigram_counts.find(n_count_seq.repr()) == bigram_counts.end() ||
            bigram_counts.find(n_count_seq_total.repr()) == bigram_counts.end()) {
          if (EXTRA_PRINTING) {
            if (bigram_counts.find(n_count_seq.repr()) == bigram_counts.end())
              cout << "Not found: " << n_count_seq.repr() << endl;
            if (bigram_counts.find(n_count_seq_total.repr()) == bigram_counts.end())
              cout << "Not found: " << n_count_seq_total.repr() << endl;
          }
          (*data)[nGiven.repr()] = 0;
        } else {
          if (EXTRA_PRINTING)
            cout << bigram_counts.at(n_count_seq.repr()) << "/" << bigram_counts.at(n_count_seq_total.repr()) << endl;
          double val = (double) bigram_counts.at(n_count_seq.repr()) /
            bigram_counts.at(n_count_seq_total.repr());
          (*data)[nGiven.repr()] = val;
        }
      }
    }

    // Pass sounds to tag_list.
    for (auto it = sounds.begin(); it != sounds.end(); ++it) {
      tag_list->push_back(*it);
    }
  }
  return true;
}

// Quick test.
#if DO_MAIN
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "No filename given." << endl;
    return 0;
  } 
  string filename = argv[1];
  map<string, double> data;
  vector<string> tag_list;
  TagGrammarFinder::FindTagGrammarFromFile(filename, &data, &tag_list);
  cout << "Data:\n";
  for (auto it = data.begin(); it != data.end(); ++it) {
    cout << it->first << " " << it->second << endl;
  }
//   cout << "Tag list:\n";
//   for (auto it = tag_list.begin(); it != tag_list.end(); ++it) {
//     cout << *it << ",";
//   }
  cout << endl;
  return 0;
}
#endif
