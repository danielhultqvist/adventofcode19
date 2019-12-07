#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void add(vector<int> &memory, const int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  int secondParameter = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];

  memory[memory[pointer + 3]] = firstParameter + secondParameter;
}

void multiply(vector<int> &memory, const int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  int secondParameter = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];

  memory[memory[pointer + 3]] = firstParameter * secondParameter;
}

int output(vector<int> &memory, const int &pointer) {
  int mode = memory[pointer] / 100 % 10;
  return mode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
}

// Rewrite to return pointer value?
void jumpIfTrue(vector<int> &memory, int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int valueToTest = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  if (valueToTest != 0) {
    pointer = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];
  } else {
    pointer += 3;
  }
}

void jumpIfFalse(vector<int> &memory, int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int valueToTest = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  if (valueToTest == 0) {
    pointer = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];
  } else {
    pointer += 3;
  }
}

void lessThan(vector<int> &memory, const int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  int secondParameter = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];

  if (firstParameter < secondParameter) {
    memory[memory[pointer + 3]] = 1;
  } else {
    memory[memory[pointer + 3]] = 0;
  }
}

void equals(vector<int> &memory, const int &pointer) {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  int secondParameter = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];

  if (firstParameter == secondParameter) {
    memory[memory[pointer + 3]] = 1;
  } else {
    memory[memory[pointer + 3]] = 0;
  }
}

vector<vector<int>> findValidCombinations(int min, int max, vector<int> &usedValues) {
  if (usedValues.size() == 5) {
    return {usedValues};
  } else {
    vector<vector<int>> result;
    for (int i = min; i <= max; ++i) {
      if (std::find(usedValues.begin(), usedValues.end(), i) == usedValues.end()) {
        vector<int> newUsedValues = usedValues;
        newUsedValues.push_back(i);
        vector<vector<int>> children = findValidCombinations(min, max, newUsedValues);
        for (const auto &j : children) {
          result.push_back(j);
        }
      }
    }
    return result;
  }
}

int main() {
  steady_clock::time_point begin = steady_clock::now();
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }

  vector<int> code;
  std::string value;
  while (std::getline(data, value, ',')) {
    code.push_back(stol(value));
  }

  vector<int> usedValues;
  vector<vector<int>> allPhaseSettings = findValidCombinations(0, 4, usedValues);
  vector<int> outputs;

  for (const auto &phaseSettings : allPhaseSettings) {
    int inputValue = 0;
    int inputtedInstructions = 0;

    for (int phaseSetting : phaseSettings) {
      vector<int> program = code;

      int pointer = 0;
      while (program[pointer] != 99) {
        int opcode = program[pointer] % 10;
        switch (opcode) {
          case 1:
            add(program, pointer);
            pointer += 4;
            break;
          case 2:
            multiply(program, pointer);
            pointer += 4;
            break;
          case 3:
            if (inputtedInstructions % 2 == 0) {
              program[program[pointer + 1]] = phaseSetting;
            } else {
              program[program[pointer + 1]] = inputValue;
            }
            inputtedInstructions++;
            pointer += 2;
            break;
          case 4:
            inputValue = output(program, pointer);
            pointer += 2;
            break;
          case 5:
            jumpIfTrue(program, pointer);
            break;
          case 6:
            jumpIfFalse(program, pointer);
            break;
          case 7:
            lessThan(program, pointer);
            pointer += 4;
            break;
          case 8:
            equals(program, pointer);
            pointer += 4;
            break;
          default:
            throw runtime_error("Unmapped opcode: " + to_string(opcode));
        }
      }
    }
    outputs.push_back(inputValue);
  }

  int maxOutput = -1;
  for (int output : outputs) {
    if (maxOutput < output) {
      maxOutput = output;
    }
  }

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
  cout << "Result: " << maxOutput << endl;
  return 0;
}
