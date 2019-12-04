#include <iostream>
#include <fstream>
#include <string>

using namespace std;

long calculateFuel(long value) {
  long requiredFuel = (value / 3) - 2;
  if (requiredFuel > 0) {
    return requiredFuel + calculateFuel(requiredFuel);
  } else {
    return 0;
  }
}

int main() {
  ifstream input{"input-1.txt"};
  unsigned long long total = 0;
  if (!input) {
    throw runtime_error("Unable to load data");
  }

  std::string line;
  while (std::getline(input, line)) {
    long value = stol(line);
    total += calculateFuel(value);
  }

  cout << total << endl;
}