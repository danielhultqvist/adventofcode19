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
#include <map>

using namespace std;
using namespace std::chrono;

int stationX = 23;
int stationY = 29;

struct Asteroid {
  int x, y;

  bool operator < (const Asteroid& asteroid) const {
    auto myDistance = sqrt(pow((y - stationY), 2) + pow((x - stationX), 2));
    auto asteroidDistance = sqrt(pow((asteroid.y - stationY), 2) + pow((asteroid.x - stationX), 2));
    return (asteroidDistance > myDistance);
  }
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

  map<double, vector<Asteroid>> asteroidsInRange;
  double startAngle;

  for (auto &asteroid : asteroids) {
    if (asteroid.x != stationX || asteroid.y != stationY) {
      // Flip the Y axis yo
      int differenceY = stationY - asteroid.y;
      int differenceX = asteroid.x - stationX;
      double angle = atan2(differenceY, differenceX);

      if (asteroidsInRange.count(angle) > 0) {
        asteroidsInRange[angle].push_back(asteroid);
      } else {
        asteroidsInRange[angle] = vector<Asteroid>{asteroid};
      }

      if (differenceX == 0 && differenceY > 0) {
        startAngle = angle;
      }
    }
  }

  vector<double> keys;
  keys.reserve(asteroidsInRange.size());
  for (auto &entry : asteroidsInRange) {
    keys.push_back(entry.first);

    // sort asteroids based on distance, makes it easy to hit them in order later on
    std::sort(entry.second.begin(), entry.second.end());
  }
  std::sort(keys.begin(), keys.end());
  std::reverse(keys.begin(), keys.end());

  // Find asteroid line to fire at
  int lineCounter = 0;
  for (int i = 0; i < keys.size(); ++i) {
    if (keys[lineCounter] == startAngle) {
      break;
    }
    lineCounter++;
  }

  // Let the shooting begin!
  int round = 0;
  for (int i = 0; i < 200; ++i) {
    vector<Asteroid> asteriodsInLine = asteroidsInRange[keys[lineCounter]];

    if (asteriodsInLine.size() > round) {
      cout << "Shot " << (i + 1) << " vaporizes (" << asteriodsInLine[round].x << "," << asteriodsInLine[round].y << ")" << endl;
    } else {
      i--;
    }
    lineCounter = (lineCounter + 1) % keys.size();
  }

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;

  return 0;
}
