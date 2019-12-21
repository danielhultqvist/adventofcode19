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
#include "../Intcode/HeapIntcodeProgram.h"

using namespace std;
using namespace std::chrono;

struct Position {
  int x, y;
};

enum Tile {
  SPACE = '.', DROID = 'D', SCAFFOLD = '#'
};

Tile tileFrom(char c) {
  switch (c) {
    case '#':
      return Tile::SCAFFOLD;
    case '.':
      return Tile::SPACE;
    case '^':
    case 'v':
    case '<':
    case '>':
      return Tile::DROID;
    default:
      throw runtime_error("Unmapped map tile: " + to_string(c));
  }
}

Tile getTile(vector<vector<Tile>> &currentMap, const int &x, const int &y) {
  if (x < 0 || x >= currentMap[0].size()) {
    return SPACE;
  }
  if (y < 0 || y >= currentMap.size()) {
    return SPACE;
  }
  return currentMap[y][x];
}

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
  auto program = HeapIntcodeProgram::loadProgram(code);

  vector<long long> input;
  program.run(input);
  const vector<long long> output = program.getOutputAndReset();

  vector<vector<Tile>> currentMap;

  vector<Tile> row;
  for (long long i : output) {
    char currentTile = (char) i;
    if (currentTile == 10) {
      if (row.size() > 0) {
        currentMap.push_back(row);
      }
      row = vector<Tile>();
    } else {
      row.push_back(tileFrom(currentTile));
    }
  }

  stringstream ss;
  vector<Position> intersections;
  for (int y = 0; y < currentMap.size(); ++y) {
    auto currentRow = currentMap[y];
    for (int x = 0; x < currentRow.size(); ++x) {
      Tile westOfTile = getTile(currentMap, x - 1, y);
      Tile northOfTile = getTile(currentMap, x, y - 1);
      Tile eastOfTile = getTile(currentMap, x + 1, y);
      Tile southOfTile = getTile(currentMap, x, y + 1);
      Tile currentTile = getTile(currentMap, x, y);

      if (westOfTile == SCAFFOLD &&
          northOfTile == SCAFFOLD &&
          eastOfTile == SCAFFOLD &&
          southOfTile == SCAFFOLD &&
          currentTile == SCAFFOLD) {
        ss << 'O';
        intersections.push_back({x, y});
      } else {
        ss << (char) getTile(currentMap, x, y);
      }
    }
    ss << endl;
  }
  cout << ss.str() << endl;

  cout << "Intersections:" << endl;
  int calibration = 0;
  for (auto &intersection : intersections) {
    cout << "(" << intersection.x << "," << intersection.y << ")" << endl;
    calibration += intersection.x * intersection.y;
  }
  cout << endl;

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
  cout << "Calibration sum: " << calibration << endl;
  return 0;
}
