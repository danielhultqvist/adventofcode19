#include <iostream>
#include <fstream>

using namespace std;

int main() {
  ifstream input{"input-1.txt"};
  unsigned long long total = 0;

  std::string line;
  while (std::getline(input, line)) {
    long value = stol(line);
    long fuel = (value / 3) - 2;
    total += fuel;
  }

  cout << to_string(total) << endl;
}