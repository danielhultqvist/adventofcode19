#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

void add(vector<int> &memory, const int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer+1] : memory[memory[pointer+1]];
  int secondParameter = secondMode == 1 ? memory[pointer+2] : memory[memory[pointer+2]];

  memory[memory[pointer+3]] = firstParameter + secondParameter;
}

void multiply(vector<int> &memory, const int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer+1] : memory[memory[pointer+1]];
  int secondParameter = secondMode == 1 ? memory[pointer+2] : memory[memory[pointer+2]];

  memory[memory[pointer+3]] = firstParameter * secondParameter;
}

int output(vector<int> &memory, const int &pointer) {
  int mode = memory[pointer] / 100;
  return mode == 1 ? memory[pointer+1] : memory[memory[pointer+1]];
}

int main() {
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }

  vector<int> code;
  std::string value;
  while (std::getline(data, value, ',')) {
    code.push_back(stol(value));
  }

  int pointer = 0;
  int inputValue = 1;
  while (code[pointer] != 99) {
    int opcode = code[pointer] % 10;
    switch (opcode) {
      case 1:
        add(code, pointer);
        pointer += 4;
        break;
      case 2:
        multiply(code, pointer);
        pointer += 4;
        break;
      case 3:
        code[code[pointer+1]] = inputValue;
        pointer += 2;
        break;
      case 4:
        cout << "Check: " << output(code, pointer) << endl;
        pointer += 2;
        break;
      default:
        throw runtime_error("Unmapped opcode: " + to_string(opcode));
    }
  }

  for (int &val : code) {
    cout << val << ",";
  }
  cout << endl;
  return 0;
}
