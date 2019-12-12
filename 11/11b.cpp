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
#include "IntcodeProgram.h"

using namespace std;
using namespace std::chrono;

struct Point {
  int x;
  int y;
  int color;

  bool operator<(const Point &point) const {
    return x < point.x && y < point.y;
  }
};

enum class Direction {
  UP = 1, LEFT = 2, DOWN = 3, RIGHT = 4
};

int main() {
  steady_clock::time_point begin = steady_clock::now();
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }

  map<string, Point> points;

  vector<long long> code;
  string value;
  while (getline(data, value, ',')) {
    code.push_back(stoll(value));
  }
  code.resize(10000000);

  auto program = IntcodeProgram::loadProgram(code);

  int iterations = 0;
  int currentInput = 1;
  int currentX = 0;
  int currentY = 0;
  auto currentDirection = Direction::UP;
  while (true) {
    program.run({currentInput});
    vector<long long> output = program.getOutputAndReset();
    iterations++;

    int outputColor = output[0];
    int directionSwitch = output[1];

    // First paint current position
    Point paintedPoint = {currentX, currentY, outputColor};
    points[to_string(currentX) + "_" + to_string(currentY)] = paintedPoint;

    // Go to next position and set next input
    switch (currentDirection) {
      case Direction::UP: {
        if (directionSwitch == 1) {
          currentDirection = Direction::RIGHT;
          currentX++;
        } else {
          currentDirection = Direction::LEFT;
          currentX--;
        }
        break;
      }
      case Direction::LEFT: {
        if (directionSwitch == 1) {
          currentDirection = Direction::UP;
          currentY++;
        } else {
          currentDirection = Direction::DOWN;
          currentY--;
        }
        break;
      }
      case Direction::DOWN: {
        if (directionSwitch == 1) {
          currentDirection = Direction::LEFT;
          currentX--;
        } else {
          currentDirection = Direction::RIGHT;
          currentX++;
        }
        break;
      }
      case Direction::RIGHT: {
        if (directionSwitch == 1) {
          currentDirection = Direction::DOWN;
          currentY--;
        } else {
          currentDirection = Direction::UP;
          currentY++;
        }
        break;
      }
    }

    auto it = points.find(to_string(currentX) + "_" + to_string(currentY));
    if (it != points.end()) {
      currentInput = it->second.color;
    } else {
      currentInput = 0;
    }

    if (program.state() == State::HALTED) {
      cout << "I'm done sir..." << endl;
      break;
    }
  }

  int maxX = -INFINITY;
  int minX = INFINITY;
  int minY = INFINITY;
  int maxY = -INFINITY;

  for (auto &point : points) {
    if (point.second.x < minX) {
      minX = point.second.x;
    }
    if (point.second.x > maxX) {
      maxX = point.second.x;
    }
    if (point.second.y < minY) {
      minY = point.second.y;
    }
    if (point.second.y > maxY) {
      maxY = point.second.y;
    }
  }

  stringstream ss;
  for (int y = maxY; y >= minY; --y) {
    for (int x = minX; x <= maxX; ++x) {
      auto it = points.find(to_string(x) + "_" + to_string(y));
      string color = " ";
      if (it != points.end()) {
        color = it->second.color == 1 ? "█" : " ";
      }
      ss << color;
    }
    ss << endl;
  }
  ss << endl;

  cout << ss.str() << endl;

  return 0;
}
