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
#include <thread>

using namespace std;
using namespace std::chrono;

struct Velocity {
  long long x, y;
};

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

class Ball {
public:
  Ball() : position({-1, -1}), velocity({0, 0}) {}

  void updatePosition(const Position &position_) {
    if (position.x == -1) {
      this->position = position_;
    } else {
      this->velocity = {position_.x - this->position.x, position_.y - this->position.y};
      this->position = position_;
    }
  }

  Position position;
  Velocity velocity;
};

class Paddle {
public:
  Paddle() : position({-1, -1}) {}

  void updatePosition(const Position &position_) {
    this->position = position_;
  }

  Position position;
};

void printTiles(const map<Position, Type> &tiles, const Ball &ball, const Paddle &paddle) {
  cout << "\033[2J\033[1;1H";
  int maxX = -1;
  int maxY = -1;
  for (auto &entry : tiles) {
    if (entry.first.x > maxX) {
      maxX = entry.first.x;
    }
    if (entry.first.y > maxY) {
      maxY = entry.first.y;
    }
  }

  stringstream ss;
  for (int y = 0; y <= maxY; ++y) {
    for (int x = 0; x <= maxX; ++x) {
      auto tile = tiles.find(Position{x, y});
      if (tile != tiles.end()) {
        if (tile->second == Type::PADDLE) {
          ss << "▀";
        }
        if (tile->second == Type::BLOCK) {
          ss << "#";
        }
        if (tile->second == Type::WALL) {
          ss << "|";
        }
        if (tile->second == Type::BALL) {
          ss << "*";
        }
        if (tile->second == Type::EMPTY) {
          ss << " ";
        }
      } else {
        ss << " ";
      }
    }
    ss << endl;
  }
  cout << ss.str() << endl;
  cout << "Ball position: (" << ball.position.x << "," << ball.position.y << ")" << endl;
  cout << "Ball velocity: (" << ball.velocity.x << "," << ball.velocity.y << ")" << endl;
  cout << "Paddle position: (" << paddle.position.x << "," << paddle.position.y << ")" << endl;

  // Needs to be a bit readable yes...
  this_thread::sleep_for(milliseconds(1));
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
  code.resize(100000);

  map<Position, Type> tiles;


  // Inject some quarters yo! Well... 2 is play for free
  code[0] = 2;

  auto program = IntcodeProgram13::loadProgram(code);
  int currentScore = 0;
  Ball ball;
  Paddle paddle;

  program.run({});
  auto output = program.getOutputAndReset();
  for (int i = 0; i < output.size(); i += 3) {
    if (output[i] == -1 && output[i + 1] == 0) {
      currentScore = output[i + 2];
    }

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
        paddle.updatePosition(position);
        break;
      case 4:
        tiles[position] = Type::BALL;
        ball.updatePosition(position);
        break;
    }
  }

  // Set initial velocity based of difference between ball and paddle
  ball.velocity.x = paddle.position.x > ball.position.x ? 1 : paddle.position.x == ball.position.x ? 0 : -1;

  int gameRounds = 0;

  int input = 0;
  while (program.state() != State::HALTED) {
    gameRounds++;
    printTiles(tiles, ball, paddle);

    input = 0;
    // Estimate the touchdown (when ball hits the bottom)
    if (ball.velocity.y >= 0) {
      if (ball.position.y == paddle.position.y - 1) {
        if (ball.position.x < paddle.position.x) {
          input = -1;
        } else if (ball.position.x > paddle.position.x) {
          input = 1;
        }
      } else {
        // going down, calculate where it is going to go down
        int roundsUntilTouchdown = (paddle.position.y - ball.position.y);
        int ballsExpectedTouchdownX = ball.position.x + roundsUntilTouchdown * ball.velocity.x;
        if (ballsExpectedTouchdownX < paddle.position.x) {
          input = -1;
        } else if (ballsExpectedTouchdownX > paddle.position.x) {
          input = 1;
        }
      }
    } else {
      // just follow it
      int ballsExpectedNextX = ball.position.x + ball.velocity.x;
      if (ballsExpectedNextX < paddle.position.x) {
        input = -1;
      } else if (ballsExpectedNextX > paddle.position.x) {
        input = 1;
      }
    }

    program.run({input});
    output = program.getOutputAndReset();

    for (int i = 0; i < output.size(); i += 3) {
      if (output[i] == -1 && output[i + 1] == 0) {
        currentScore = output[i + 2] != 0 ? output[i + 2] : currentScore;
      }

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
          paddle.updatePosition(position);
          break;
        case 4:
          tiles[position] = Type::BALL;
          ball.updatePosition(position);
          break;
      }
    }
  }

  int numBlocks = 0;
  int numBalls = 0;
  int numPaddles = 0;
  for (auto &entry : tiles) {
    if (entry.second == Type::BALL) {
      numBalls++;
    }
    if (entry.second == Type::BLOCK) {
      numBlocks++;
    }
    if (entry.second == Type::PADDLE) {
      numPaddles++;
    }
  }

  cout << "Game rounds: " << gameRounds << endl;
  cout << "Num balls: " << numBalls << endl;
  cout << "Num blocks: " << numBlocks << endl;
  cout << "Score: " << currentScore << endl;

  return 0;
}