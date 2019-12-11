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

using namespace std;
using namespace std::chrono;

struct Asteroid {
  int x, y;
};

int main() {
  steady_clock::time_point begin = steady_clock::now();
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }

  vector<Asteroid> asteroids;

  std::string value;
  int row = 0;
  while (std::getline(data, value)) {
    for (int col = 0; col < value.length(); ++col) {
      if (value[col] == '#') {
        asteroids.push_back({col, row});
      }
    }
    row++;
  }

  int bestX = -1, bestY = -1, numSeenAsteroids = 0;
  for (int i = 0; i < asteroids.size(); ++i) {
    std::set<double> angles;
    for (int j = 0; j < asteroids.size(); ++j) {
      if (i != j) {
        double angle;
        // Flip the Y axis yo
        int differenceY = asteroids[i].y - asteroids[j].y;
        int differenceX = asteroids[j].x - asteroids[i].x;
        angle = atan2(differenceY, differenceX);
        angles.insert(angle);
      }
    }
    if (angles.size() > numSeenAsteroids) {
      bestX = asteroids[i].x;
      bestY = asteroids[i].y;
      numSeenAsteroids = angles.size();
    }
  }

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
  cout << "Best asteroid: (" << bestX << "," << bestY << ") with " << numSeenAsteroids << " seen asteroids" << endl;

  return 0;
}
