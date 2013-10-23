#include "CypherReader.h"

bool CypherReader::GetObservedData(const string &filename,
                                   vector<string> *observed_data) {
  ifstream fin(filename.c_str());
  if (fin.fail()) {
    cerr << "Could not open file " << filename << endl;
    return false;
  } else {
    while (true) {
      string word;
      fin >> word;
      if (fin.eof())
        break;
      if (!word.empty()) {
        word = word.substr(1, word.size() - 2);  // Remove quotes.
        observed_data->push_back(word);
      }
    }
    return true;
  }
}

// Quick test, can uncomment and use.
// int main(int argc, char *argv[]) {
//   if (argc < 2) {
//     cerr << "No filename given." << endl;
//     return 0;
//   }
//   string filename = argv[1];
//   vector<string> observed_data;
//   CypherReader::GetObservedData(filename, &observed_data);
//   for (auto it = observed_data.begin(); it != observed_data.end(); ++it) {
//     cout << *it << " ";
//   }
//   return 0;
// }
