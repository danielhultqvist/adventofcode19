#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
  ifstream input{"input-1.txt"};
  unsigned long long total = 0;
  if (!input) {
    throw runtime_error("Unable to load data");
  }

  std::string line;
  while (std::getline(input, line)) {
    long value = stol(line);
    total += (value / 3) - 2;
  }

  cout << to_string(total) << endl;
}