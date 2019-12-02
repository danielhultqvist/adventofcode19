#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main() {
//  ifstream input{"test.txt"};
  ifstream input{"input.txt"};
  if (!input) {
    throw runtime_error("Unable to load data");
  }

  vector<int> code;
  std::string value;
  while (std::getline(input, value, ',')) {
    code.push_back(stol(value));
  }

  // Extra instructions
  code[1] = 12;
  code[2] = 2;

  int position = 0;
  while (code[position] != 99) {
    if (code[position] == 1) {
      code[code[position + 3]] = code[code[position + 1]] + code[code[position + 2]];
    } else {
      code[code[position + 3]] = code[code[position + 1]] * code[code[position + 2]];
    }
    position += 4;
  }

  stringstream ss;
  for (int i = 0; i < code.size() - 1; ++i) {
    ss << code[i] << ",";
  }
  ss << code[code.size() - 1] << endl;
  cout << ss.str() << endl;
}