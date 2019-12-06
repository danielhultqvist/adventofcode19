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
  int mode = memory[pointer] / 100 % 10;
  return mode == 1 ? memory[pointer+1] : memory[memory[pointer+1]];
}

// Rewrite to return pointer value?
void jumpIfTrue(vector<int> &memory, int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int valueToTest = firstMode == 1 ? memory[pointer+1] : memory[memory[pointer+1]];
  if (valueToTest != 0) {
    pointer = secondMode == 1 ? memory[pointer+2] : memory[memory[pointer+2]];
  } else {
    pointer += 3;
  }
}

void jumpIfFalse(vector<int> &memory, int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int valueToTest = firstMode == 1 ? memory[pointer+1] : memory[memory[pointer+1]];
  if (valueToTest == 0) {
    pointer = secondMode == 1 ? memory[pointer+2] : memory[memory[pointer+2]];
  } else {
    pointer += 3;
  }
}

void lessThan(vector<int> &memory, const int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer+1] : memory[memory[pointer+1]];
  int secondParameter = secondMode == 1 ? memory[pointer+2] : memory[memory[pointer+2]];

  if (firstParameter < secondParameter) {
    memory[memory[pointer+3]] = 1;
  } else {
    memory[memory[pointer+3]] = 0;
  }
}

void equals(vector<int> &memory, const int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer+1] : memory[memory[pointer+1]];
  int secondParameter = secondMode == 1 ? memory[pointer+2] : memory[memory[pointer+2]];

  if (firstParameter == secondParameter) {
    memory[memory[pointer+3]] = 1;
  } else {
    memory[memory[pointer+3]] = 0;
  }
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

  const int inputValue = 5;

  int pointer = 0;
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
      case 5:
        jumpIfTrue(code, pointer);
        break;
      case 6:
        jumpIfFalse(code, pointer);
        break;
      case 7:
        lessThan(code, pointer);
        pointer += 4;
        break;
      case 8:
        equals(code, pointer);
        pointer += 4;
        break;
      default:
        throw runtime_error("Unmapped opcode: " + to_string(opcode));
    }
  }
  return 0;
}
