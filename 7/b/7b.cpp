#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <algorithm>

#include "IntcodeProgram.h"

using namespace std;
using namespace std::chrono;

// Yes, it's recursively. Yep, a lot of memory. Yep, it's only '5!' combinations -> 120 -> super fast
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

  // Read instructions from disk
  vector<int> code;
  std::string value;
  while (std::getline(data, value, ',')) {
    code.push_back(stol(value));
  }

  // Calculate all phase settings
  vector<int> dummy;
  vector<vector<int>> allPhaseSettings = findValidCombinations(5, 9, dummy);

  // Set number of amplifiers, let's be dynamic yeaaah.
  const int numAmplifiers = 5;

  // Store all possible outputs
  vector<int> outputs;

  // Run for every possible phase settings
  for (const auto &phaseSettings : allPhaseSettings) {

    // Load all amplifiers once per phase settings, so we don't reset memory
    vector<IntcodeProgram> amplifiers;
    for (int i = 0; i < numAmplifiers; ++i) {
      amplifiers.push_back(IntcodeProgram::loadProgram(code));
    }

    // Keep a global input variable since we run sequential single threaded
    int nextInputValue = 0;

    // What amplifier will run next
    int currentAmplifier = 0;

    // We should only input phase settings on first iteration
    bool firstIteration = true;

    while (true) {
      if (firstIteration) {
        amplifiers[currentAmplifier].run({phaseSettings[currentAmplifier], nextInputValue});
      } else {
        amplifiers[currentAmplifier].run({nextInputValue});
      }

      // Get the output from the executed amplifier program
      nextInputValue = amplifiers[currentAmplifier].getLastOutput();

      // If we are on the last amplifier, set that first iteration has run
      if (currentAmplifier == (numAmplifiers - 1)) {
        firstIteration = false;
      }

      // Has the last amplifier run and have been halted? If so, stop
      if (currentAmplifier == (numAmplifiers - 1) && amplifiers[currentAmplifier].state() == State::HALTED) {
        break;
      } else {
        // Otherwise, let's go to the next amplifier in the loop
        currentAmplifier = ++currentAmplifier % numAmplifiers;
      }
    }

    // Store the last output value, which has been set to nextInputValue (going into the thrusters)
    outputs.push_back(nextInputValue);
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
