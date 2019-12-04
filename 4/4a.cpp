#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

bool fulfillsCriteria(int value) {
  vector<int> values;
  values.push_back(value / 100000 % 10);
  values.push_back(value / 10000 % 10);
  values.push_back(value / 1000 % 10);
  values.push_back(value / 100 % 10);
  values.push_back(value / 10 % 10);
  values.push_back(value % 10);

  bool increasingDigits =
    values[0] <= values[1] &&
    values[1] <= values[2] &&
    values[2] <= values[3] &&
    values[3] <= values[4] &&
    values[4] <= values[5];


  bool adjecentDigit =
    values[0] == values[1] || 
    values[1] == values[2] || 
    values[2] == values[3] || 
    values[3] == values[4] ||
    values[4] == values[5];
  return increasingDigits && adjecentDigit;
}

int main() {
  ifstream input{"input.txt"};
  if (!input) {
    throw runtime_error("Unable to load data");
  }

  string inputString;
  getline(input, inputString);
  int start = stoi(inputString.substr(0, inputString.find('-')));
  int end = stoi(inputString.substr(inputString.find('-') + 1));

  int numCandidates = 0;
  for (int i = start; i <= end; ++i) {
    if (fulfillsCriteria(i)) {
      numCandidates++;
//      cout << "Candidate: " << to_string(i) << endl;
    }
  }

  cout << "Range: " << start << " - " << end << endl;
  cout << "Number of combinations: " << numCandidates << endl;

  return 0;
}