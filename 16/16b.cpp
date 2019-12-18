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

string toString(vector<int> value) {
  stringstream ss;
  for (int i = 0; i < value.size(); ++i) {
    ss << value[i];
  }
  return ss.str();
}

int main() {
  steady_clock::time_point begin = steady_clock::now();

  string inputString = "03036732577212944063491565474664";
//  string inputString = "59758034323742284979562302567188059299994912382665665642838883745982029056376663436508823581366924333715600017551568562558429576180672045533950505975691099771937719816036746551442321193912312169741318691856211013074397344457854784758130321667776862471401531789634126843370279186945621597012426944937230330233464053506510141241904155782847336539673866875764558260690223994721394144728780319578298145328345914839568238002359693873874318334948461885586664697152894541318898569630928429305464745641599948619110150923544454316910363268172732923554361048379061622935009089396894630658539536284162963303290768551107950942989042863293547237058600513191659935";

  const unsigned long numDigits = inputString.length();
  std::vector<int> bufferA(numDigits);
  std::vector<int> bufferB(numDigits);
  for (int i = 0; i < numDigits; ++i) {
    bufferA[i] = inputString[i] - '0';
  }


  int repeats = 10000;

  /*
   *  CREATE ALL PATTERNS
   */
  char patterns[numDigits*repeats][numDigits*repeats];
  memset(patterns, 0, sizeof(char) * numDigits*repeats * numDigits*repeats);
  for (int i = 0; i < numDigits*repeats; ++i) {

    int countPerDigit = i + 1;
    int currentCount = 1;
    int offset = 0;

    for (int j = 0; j < numDigits*repeats; ++j) {
      if (currentCount == countPerDigit) {
        offset++;
        currentCount = 0;
      }
      patterns[i][j] = BASE_PATTERN[offset % PATTERN_LENGTH];
      currentCount++;
    }

    if (i % 1000 == 0) {
      cout << "At pattern " << i << endl;
    }
  }

  /*
   * CALCULATE VALUES
   */

  unsigned long digitOffset = stol(inputString.substr(0, 7));

  bool printAll = true;
  int phases = 100;
  vector<int> &inputArray = bufferA;
  vector<int> &outputArray = bufferB;


  cout << "Input signal: " << inputString << endl;
  for (int i = 1; i <= phases; ++i) {

    for (int j = 0; j < numDigits * repeats; ++j) {
      long long outputValue = 0;
      for (int k = 0; k < numDigits * repeats; ++k) {
        int valueToMultiply = inputArray[k % numDigits];
        outputValue += (valueToMultiply * patterns[j][k]);
      }
      outputArray[j] = abs(outputValue) % 10;
    }

    if (printAll || i == phases) {
      cout << "After " << i << " phases: " << toString(outputArray) << endl;
    }

    vector<int> &temp = inputArray;
    inputArray = outputArray;
    outputArray = temp;
  }




  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;

  return 0;
}
