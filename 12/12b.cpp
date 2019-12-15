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

using namespace std;
using namespace std::chrono;


int calculateVelocityChange(int leftPosition, int rightPosition) {
  if (leftPosition == rightPosition) {
    return 0;
  } else if (leftPosition < rightPosition) {
    return 1;
  } else {
    return -1;
  }
}

string toString(const vector<int>& velocities, const vector<int>& positions) {
  stringstream ss;
  for (int velocity : velocities) {
    ss << to_string(velocity) + "-";
  }
  for (int position : positions) {
    ss << to_string(position) + "-";
  }
  return ss.str();
}

long long findPeriod(vector<int> velocities, vector<int> positions) {
  set<string> previousPositions;
  string position = toString(velocities, positions);
  int iterations = 0;

  while(previousPositions.count(position) == 0) {
    previousPositions.insert(position);

    for (int i = 0; i < positions.size(); ++i) {
      for (int j = 0; j < positions.size(); ++j) {
        if (i != j) {
          velocities[i] += calculateVelocityChange(positions[i], positions[j]);
        }
      }
    }

    for (int i = 0; i < positions.size(); ++i) {
      positions[i] += velocities[i];
    }

    position = toString(velocities, positions);

    iterations++;
  }

  return iterations;
}

int main() {
  steady_clock::time_point begin = steady_clock::now();
  vector<int> xpositions = {14, 7, 1, -4};
  vector<int> ypositions = {2, 4, 17, -1};
  vector<int> zpositions = {8, 10, 16, 1};
  vector<int> initialVelocity = {0, 0, 0, 0};

  long long periodX = findPeriod(initialVelocity, xpositions);
  long long periodY = findPeriod(initialVelocity, ypositions);
  long long periodZ = findPeriod(initialVelocity, zpositions);

  long long xyGCD = gcd(periodX, periodY);
  long long xyLCM = (periodX*periodY)/xyGCD;

  long long xyzGCD = gcd(xyLCM, periodZ);
  long long xyzLCM = (xyLCM*periodZ)/xyzGCD;

  steady_clock::time_point end = steady_clock::now();
  cout << "It takes " << xyzLCM << " steps to get back to a previous state" << endl;
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;

  return 0;
}
