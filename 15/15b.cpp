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
#include <thread>
#include "../Intcode/HeapIntcodeProgram.h"

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
  EMPTY, WALL, OXYGEN_TANK, OXYGEN
};

void drawMap(map<Position, Type> currentMap, const int &droidX, const int &droidY, const int &iteration,
             const bool spreadingGas) {
  cout << "\033[2J\033[1;1H";
  if (spreadingGas) {
    cout << "Spreading gas, minutes elapsed: " << iteration << endl;
  } else {
    cout << "Current steps: " << iteration << endl;
  }
  int minX = droidX;
  int minY = droidY;
  int maxX = droidX;
  int maxY = droidY;
  for (auto &entry : currentMap) {
    if (entry.first.x > maxX) {
      maxX = entry.first.x;
    }
    if (entry.first.x < minX) {
      minX = entry.first.x;
    }
    if (entry.first.y > maxY) {
      maxY = entry.first.y;
    }
    if (entry.first.y < minY) {
      minY = entry.first.y;
    }
  }

  stringstream ss;
  ss << endl;
  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      if (y == droidY && x == droidX) {
        ss << "D";
      } else if (x == 0 && y == 0) {
        ss << "◍";
      } else {
        auto tile = currentMap.find(Position{x, y});
        if (tile != currentMap.end()) {
          switch (tile->second) {
            case Type::EMPTY:
              ss << ".";
              break;
            case Type::WALL:
              ss << "█";
              break;
            case Type::OXYGEN_TANK:
              ss << "X";
              break;
            case Type::OXYGEN:
              ss << "○";
              break;
          }
        } else {
          ss << " ";
        }
      }
    }
    ss << endl;
  }

  cout << ss.str() << endl;
}

enum Direction {
  NORTH = 1,
  SOUTH = 2,
  WEST = 3,
  EAST = 4
};

void moveDroid(int &x, int &y, Direction &currentDirection) {
  switch (currentDirection) {
    case NORTH:
      currentDirection = EAST;
      y--;
      break;
    case WEST:
      currentDirection = NORTH;
      x--;
      break;
    case SOUTH:
      currentDirection = WEST;
      y++;
      break;
    case EAST:
      currentDirection = SOUTH;
      x++;
      break;
    default:
      throw runtime_error("AAAH UNKNOWN MOVE DIRECTION");
  }
}

void placeAt(map<Position, Type> &map, const int &x, const int &y, const int &input, const Type type) {
  switch (input) {
    case NORTH:
      map[{x, (y - 1)}] = type;
      break;
    case WEST:
      map[{(x - 1), y}] = type;
      break;
    case SOUTH:
      map[{x, (y + 1)}] = type;
      break;
    case EAST:
      map[{(x + 1), y}] = type;
      break;
    default:
      throw runtime_error("AAAH UNKNOWN MOVE DIRECTION");
  }
}

int calculateNumEmptySpots(map<Position, Type> &currentMap) {
  int count = 0;
  for (auto &entry : currentMap) {
    if (entry.second == Type::EMPTY) {
      count++;
    }
  }
  return count;
}

void spreadOxygen(map<Position, Type> &currentMap) {
  vector<Position> oxygenPositions;
  for (auto &entry : currentMap) {
    if (entry.second == Type::OXYGEN) {
      oxygenPositions.push_back(entry.first);
    }
  }

  for (Position &position : oxygenPositions) {
    auto west = currentMap.find({position.x - 1, position.y});
    auto north = currentMap.find({position.x, position.y - 1});
    auto east = currentMap.find({position.x + 1, position.y});
    auto south = currentMap.find({position.x, position.y + 1});

    if (west != currentMap.end() && west->second == Type::EMPTY) {
      west->second = Type::OXYGEN;
    }

    if (north != currentMap.end() && north->second == Type::EMPTY) {
      north->second = Type::OXYGEN;
    }

    if (east != currentMap.end() && east->second == Type::EMPTY) {
      east->second = Type::OXYGEN;
    }

    if (south != currentMap.end() && south->second == Type::EMPTY) {
      south->second = Type::OXYGEN;
    }
  }
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

  map<Position, Type> currentMap;
  currentMap[{0, 0}] = Type::EMPTY;
  int currentX = 0;
  int currentY = 0;
  int stepsTaken = 0;

  int oxygenX = -1;
  int oxygenY = -1;

  Direction currentDirection = EAST;

  while (true) {
    drawMap(currentMap, currentX, currentY, stepsTaken, false);

    if (currentX == 0 && currentY == 0 && stepsTaken > 30) {
      break;
    }
    stepsTaken++;

    program.run({currentDirection});
    int status = program.getOutputAndReset()[0];

    switch (status) {
      case 0:
        placeAt(currentMap, currentX, currentY, currentDirection, Type::WALL);
        switch (currentDirection) {
          case NORTH: {
            currentDirection = WEST;
            break;
          };
          case SOUTH: {
            currentDirection = EAST;
            break;
          }
          case WEST: {
            currentDirection = SOUTH;
            break;
          }
          case EAST: {
            currentDirection = NORTH;
            break;
          }
          default:
            throw runtime_error("lol");
        }
        break;
      case 1:
        placeAt(currentMap, currentX, currentY, currentDirection, Type::EMPTY);
        moveDroid(currentX, currentY, currentDirection);
        break;
      case 2:
        placeAt(currentMap, currentX, currentY, currentDirection, Type::OXYGEN_TANK);
        moveDroid(currentX, currentY, currentDirection);
        oxygenX = currentX;
        oxygenY = currentY;
        break;
    }
    this_thread::sleep_for(milliseconds(10));
  }

  // MAP IS DONE! Time to spread some gas!
  int timeForGasToSpread = 0;
  int numEmptySpots = 1;

  currentMap[{oxygenX, oxygenY}] = Type::OXYGEN;

  while (numEmptySpots > 0) {
    spreadOxygen(currentMap);
    numEmptySpots = calculateNumEmptySpots(currentMap);
    timeForGasToSpread++;
    drawMap(currentMap, currentX, currentY, timeForGasToSpread, true);

    this_thread::sleep_for(milliseconds(50));
  }

  cout << "It took " << timeForGasToSpread << " minutes for the oxygen to spread" << endl;

  return 0;
}