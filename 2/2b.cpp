#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

// Do not make code a reference, it should be copied to "reset" memory
int calculate(vector<int> code, int noun, int verb) {
  code[1] = noun;
  code[2] = verb;

  int position = 0;
  while (code[position] != 99) {
    if (code[position] == 1) {
      code[code[position + 3]] = code[code[position + 1]] + code[code[position + 2]];
    } else {
      code[code[position + 3]] = code[code[position + 1]] * code[code[position + 2]];
    }
    position += 4;
  }
  return code[0];
}

int main() {
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
  for (int noun = 0; noun < 100; ++noun) {
    for (int verb = 0; verb < 100; ++verb) {
      int result = calculate(code, noun, verb);
      if (result == 19690720) {
        cout << "Matching candidate, noun: " << noun << ", verb: " << verb << endl;
        cout << "Value to enter: " << ((100*noun)+verb) << endl;
      }
    }
  }
}

