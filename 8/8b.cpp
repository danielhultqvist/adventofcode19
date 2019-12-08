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

  // Default output image
  vector<int> output;
  output.reserve(width*height);
  for(int i = 0; i < width*height; ++i) {
    output[i] = 2;
  }

  // Iterate through all layers
  for (int l = 0; l < layers; ++l) {
    stringstream ss;

    // Offset in input stream, new layer
    int layerOffset = l * width * height;

    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        // Offset in input stream due to new row
        int rowOffset = y * width;
        int totalOffset = x + rowOffset + layerOffset;

        // Magic ASCII conversion ('0' = 48)
        int currentNumber = value[totalOffset] - '0';
        if (output[x + rowOffset] == 2) {
          output[x + rowOffset] = currentNumber;
        }
        ss << currentNumber;
      }
      ss << endl;
    }

    cout << "Layer " << l << endl;
    cout << ss.str() << endl << endl;
  }

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
  stringstream ss;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      string digit = output[x + (y * width)] == 0 ? " " : "█";
      ss << digit << " ";
    }
    ss << endl;
  }
  cout << "Picture:" << endl << ss.str() << endl;
}
