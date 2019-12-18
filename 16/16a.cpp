#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <set>
#include <map>
#include <iomanip>
#include <numeric>
#include <array>

using namespace std;
using namespace std::chrono;

const int BASE_PATTERN[] = {0, 1, 0, -1};
const int PATTERN_LENGTH = 4;

int main() {
  steady_clock::time_point begin = steady_clock::now();

  string inputString = "12345678";

  const unsigned long numDigits = inputString.length();
  int bufferA[numDigits];
  int bufferB[numDigits];
  for (int i = 0; i < numDigits; ++i) {
    bufferA[i] = inputString[i] - '0';
  }
  
  vector<vector<int>> patterns;
  for (int i = 0; i < numDigits; ++i) {
    vector<int> pattern(numDigits);

    int countPerDigit = i + 1;
    int currentCount = 1;
    int offset = 0;

    for (int j = 0; j < numDigits; ++j) {
      if (currentCount == countPerDigit) {
        offset++;
        currentCount = 0;
      }
      pattern[j] = BASE_PATTERN[offset % PATTERN_LENGTH];
      currentCount++;
    }
    patterns.push_back(pattern);
  }

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;

  return 0;
}
