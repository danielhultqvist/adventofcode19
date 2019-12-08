#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <memory>
#include <chrono>

using namespace std;
using namespace std::chrono;

// For this assignment, just skip the image concept and count while iterating through -> O(n)
int main() {
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }
  steady_clock::time_point begin = steady_clock::now();

  string value;
  getline(data, value);

  // Calculate constants for traversing
  int width = 25;
  int height = 6;
  int layers = value.size() / (width * height);

  // Just keep counters for everything
  int totalFewestDigits = INT32_MAX;
  int fewestNumber = 0;
  int firstNumber = 1;
  int secondNumber = 2;

  // End result
  int output = -1;

  // Iterate through all layers, will be offset
  for (int l = 0; l < layers; ++l) {
    // Offset in input stream, new layer
    int layerOffset = l * width * height;

    // Reset counters per layer
    int countFewestNumber = 0;
    int countFirstNumber = 0;
    int countSecondNumber = 0;
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        // Offset in input stream due to new row
        int rowOffset = y * width;

        // Magic ASCII conversion ('0' = 48)
        int currentNumber = value[x + rowOffset + layerOffset] - '0';

        // Just calculate yo
        if (currentNumber == fewestNumber) {
          countFewestNumber++;
        } else if (currentNumber == firstNumber) {
          countFirstNumber++;
        } else if (currentNumber == secondNumber) {
          countSecondNumber++;
        }
      }
    }

    // If this layer has fewest zeros, calculate output
    if (countFewestNumber < totalFewestDigits) {
      totalFewestDigits = countFewestNumber;
      output = countFirstNumber * countSecondNumber;
    }
  }

  cout << "Total fewest: " << totalFewestDigits << endl;

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
  cout << "Value to output: " << output;
}
