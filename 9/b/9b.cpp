#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <cstring>

#include "IntcodeProgram.h"

using namespace std;
using namespace std::chrono;

int main() {
  steady_clock::time_point begin = steady_clock::now();
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }

  // Read instructions from disk
  vector<long long> code;
  std::string value;
  while (std::getline(data, value, ',')) {
    code.push_back(stoll(value));
  }
  code.resize(94915192, 0);

  vector<long long> input = {2};

  auto program = IntcodeProgram::loadProgram(code);

  program.run(input);

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
//  cout << "Result: " << program.getLastOutput() << endl;
  return 0;
}
