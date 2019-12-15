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
  EMPTY, WALL, OXYGEN_TANK
};

void drawMap(map<Position, Type> currentMap, const int &droidX, const int &droidY, const int &currentSteps) {
  cout << "\033[2J\033[1;1H";
  cout << "Press Q to quit, R to reset steps counter" << "\r\n";
  cout << "Current steps: " << currentSteps << "\r\n";
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
  ss << "\r";
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
          }
        } else {
          ss << " ";
        }
      }
    }
    ss << endl << "\r";
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

void placeAt(map<Position, Type> &map, const int &x, const int &y, const Direction &currentDirection, const Type type) {
  switch (currentDirection) {
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

int main() try {
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
  int currentX = 0;
  int currentY = 0;
  Direction currentDirection = EAST;
  int stepsTaken = 0;

  while (true) {
    drawMap(currentMap, currentX, currentY, stepsTaken);

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
        break;
    }
  }

  // SWITCH TO MANUAL MODE
  stepsTaken = 0;
  currentX = 0;
  currentY = 0;

  // Configure terminal to avoid using enter for commands
  system("stty raw");

  char keyPressed = 0;
  while (true) {
    drawMap(currentMap, currentX, currentY, stepsTaken);
    cin >> keyPressed;

    // Stop by pressing q
    if (keyPressed == 'q') {
      break;
    }

    switch (keyPressed) {
      case 'w':
        currentDirection = NORTH;
        break;
      case 'a':
        currentDirection = WEST;
        break;
      case 's':
        currentDirection = SOUTH;
        break;
      case 'd':
        currentDirection = EAST;
        break;
      case 'r':
        stepsTaken = 0;
        continue;
      default:
        cout << "Unsupported key: " << keyPressed << endl;
        continue;
    }

    program.run({currentDirection});

    int status = program.getOutputAndReset()[0];

    switch (status) {
      case 0:
        placeAt(currentMap, currentX, currentY, currentDirection, Type::WALL);
        break;
      case 1:
        placeAt(currentMap, currentX, currentY, currentDirection, Type::EMPTY);
        moveDroid(currentX, currentY, currentDirection);
        stepsTaken++;
        break;
      case 2:
        placeAt(currentMap, currentX, currentY, currentDirection, Type::OXYGEN_TANK);
        moveDroid(currentX, currentY, currentDirection);
        stepsTaken++;
        break;
    }
  }

  system("stty cooked");

  return 0;
} catch (exception &e) {
  system("stty cooked");
}