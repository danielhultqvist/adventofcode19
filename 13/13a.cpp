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
#include "IntcodeProgram13.h"

using namespace std;
using namespace std::chrono;

struct Position {
  long long x, y;

  bool operator==(const Position &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  bool operator!=(const Position &rhs) const {
    return !(rhs == *this);
  }

  bool operator<(const Position &rhs) const {
    return y < rhs.y || (y == rhs.y && x < rhs.x);
  }
};

enum class Type {
  EMPTY, WALL, BLOCK, PADDLE, BALL
};

struct Tile {
  Position position;
};

int main() {
  steady_clock::time_point begin = steady_clock::now();
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }

  vector<long long> code;
  string value;
  while (getline(data, value, ',')) {
    code.push_back(stoll(value));
  }
  code.resize(100000);

  map<Position, Type> tiles;

  auto program = IntcodeProgram13::loadProgram(code);
  program.run({});
  auto output = program.getOutputAndReset();

  for (int i = 0; i < output.size(); i += 3) {
    Position position{output[i], output[i + 1]};
    switch (output[i + 2]) {
      case 0:
        tiles[position] = Type::EMPTY;
        break;
      case 1:
        tiles[position] = Type::WALL;
        break;
      case 2:
        tiles[position] = Type::BLOCK;
        break;
      case 3:
        tiles[position] = Type::PADDLE;
        break;
      case 4:
        tiles[position] = Type::BALL;
        break;
    }
  }

  int numBlocks = 0;
  int numBalls = 0;
  for (auto &entry : tiles) {
    if (entry.second == Type::BALL) {
      numBalls++;
    }
    if (entry.second == Type::BLOCK) {
      numBlocks++;
    }
  }

  cout << "Num balls: " << numBalls << endl;
  cout << "Num blocks: " << numBlocks << endl;

  return 0;
}
