#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

bool fulfillsCriteria(int value) {
  int first = value / 100000 % 10;
  int second = value / 10000 % 10;
  int third = value / 1000 % 10;
  int fourth = value / 100 % 10;
  int fifth = value / 10 % 10;
  int sixth = value % 10;

  bool increasingDigits = first <= second && second <= third && third <= fourth && fourth <= fifth && fifth <= sixth;
  bool adjecentDigit = first == second || second == third || third == fourth || fourth == fifth || fifth == sixth;
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