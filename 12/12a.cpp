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

using namespace std;
using namespace std::chrono;

struct Velocity {
  int x, y, z;
};

struct Position {
  int x, y, z;
};

struct Moon {
  Velocity velocity;
  Position position;
};

void printMoons(const vector<Moon> &moons) {
  for (auto &moon : moons) {
    cout << "pos=<x=" << setw(3) << moon.position.x << ",";
    cout << "y=" << setw(3) << moon.position.y << ",";
    cout << "z=" << setw(3) << moon.position.z << ">, ";

    cout << "vel=<x=" << setw(3) << moon.velocity.x << ",";
    cout << "y=" << setw(3) << moon.velocity.y << ",";
    cout << "z=" << setw(3) << moon.velocity.z << ">";
    cout << endl;
  }
}

int calculateVelocityChange(int leftPosition, int rightPosition) {
  if (leftPosition == rightPosition) {
    return 0;
  } else if (leftPosition < rightPosition) {
    return 1;
  } else {
    return -1;
  }
}

int main() {
  steady_clock::time_point begin = steady_clock::now();
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }

  vector<Moon> moons = {
    {{0, 0, 0}, {14, 2,  8}},
    {{0, 0, 0}, {7,  4,  10}},
    {{0, 0, 0}, {1,  17, 16}},
    {{0, 0, 0}, {-4, -1, 1}}
  };
  const int NUM_STEPS = 1000;

  cout << "After 0 steps: " << endl;
  printMoons(moons);
  cout << endl;

  for (int step = 0; step < NUM_STEPS; ++step) {
    // Update velocity
    for (int i = 0; i < moons.size(); ++i) {
      for (int j = 0; j < moons.size(); ++j) {
        if (i != j) {
          moons[i].velocity.x += calculateVelocityChange(moons[i].position.x, moons[j].position.x);
          moons[i].velocity.y += calculateVelocityChange(moons[i].position.y, moons[j].position.y);
          moons[i].velocity.z += calculateVelocityChange(moons[i].position.z, moons[j].position.z);
        }
      }
    }

    // Update position
    for (auto &moon : moons) {
      moon.position.x += moon.velocity.x;
      moon.position.y += moon.velocity.y;
      moon.position.z += moon.velocity.z;
    }

    cout << "After " << (step + 1) << " steps: " << endl;
    printMoons(moons);
    cout << endl;
  }

  int totalEnergy = 0;
  for (auto &moon : moons) {
    int potentialEnergy = abs(moon.position.x) + abs(moon.position.y) + abs(moon.position.z);
    int kineticEnergy = abs(moon.velocity.x) + abs(moon.velocity.y) + abs(moon.velocity.z);
    totalEnergy += potentialEnergy * kineticEnergy;
  }

  cout << "Total energy: " << totalEnergy << endl;
  return 0;
}