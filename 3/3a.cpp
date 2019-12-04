#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>

using namespace std;

/*
 *        /\ (0,1)
 *        |
 *        |
 *       (0,0)-----> (1,0)
 */

struct Line {
public:
  Line(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

  const int x1, y1;
  const int x2, y2;

  bool horizontal() { return y1 == y2; }

  bool vertical() { return x1 == x2; }
};

struct Intersection {
  Intersection(int x, int y) : x(x), y(y) {}

  int x, y;
};

void findIntersectionIfAny(vector<Intersection> &intersections, const Line &firstLine, const Line &secondLine);

vector<Line> calculateLine(string coordinates) {
  std::string value;
  int currentX = 0;
  int currentY = 0;
  stringstream ss(coordinates);
  vector<Line> result;
  while (std::getline(ss, value, ',')) {
    int step = stoi(value.substr(1));
    switch (value[0]) {
      case 'U':
        result.push_back(Line(currentX, currentY, currentX, currentY + step));
        currentY += step;
        break;
      case 'D':
        result.push_back(Line(currentX, currentY, currentX, currentY - step));
        currentY -= step;
        break;
      case 'L':
        result.push_back(Line(currentX, currentY, currentX - step, currentY));
        currentX -= step;
        break;
      case 'R':
        result.push_back(Line(currentX, currentY, currentX + step, currentY));
        currentX += step;
        break;
      default:
        throw runtime_error("Unmapped direction: " + to_string(value[0]));
    }
  }
  return result;
}

vector<Intersection> findIntersections(vector<Line> &firstLines, vector<Line> &secondLines) {
  vector<Intersection> intersections;
  for (auto firstLine : firstLines) {
    for (auto secondLine : secondLines) {
      if (firstLine.horizontal() && secondLine.vertical()) {
        findIntersectionIfAny(intersections, firstLine, secondLine);
      }
      if (firstLine.vertical() && secondLine.horizontal()) {
        findIntersectionIfAny(intersections, secondLine, firstLine);
      }
    }
  }

  return intersections;
}

void findIntersectionIfAny(vector<Intersection> &intersections, const Line &horizontal, const Line &vertical) {
  int x = vertical.x1;
  int y = horizontal.y1;

  if (x == 0 && y == 0) {
    // skip me
    return;
  }

  if ((horizontal.x1 < horizontal.x2 && horizontal.x1 <= x && x <= horizontal.x2) ||
      (horizontal.x2 < horizontal.x1 && horizontal.x2 <= x && x <= horizontal.x1)) {
    if ((vertical.y1 < vertical.y2 && vertical.y1 <= y && y <= vertical.y2) ||
        (vertical.y2 < vertical.y1 && vertical.y2 <= y && y <= vertical.y1)) {
      intersections.emplace_back(x, y);
    }
  }
}

int main() {
  //ifstream input{"test.txt"};
  ifstream input{"input.txt"};
  if (!input) {
    throw runtime_error("Unable to load data");
  }

  string firstLine;
  string secondLine;
  std::getline(input, firstLine);
  std::getline(input, secondLine);

  auto firstLines = calculateLine(firstLine);
  auto secondLines = calculateLine(secondLine);

  vector<Intersection> intersections = findIntersections(firstLines, secondLines);

  Intersection shortest = intersections[0];
  for (unsigned int i = 1; i < intersections.size(); ++i) {
    if ((abs(shortest.x) + abs(shortest.y) - abs(intersections[i].x) - abs(intersections[i].y)) > 0) {
      shortest = intersections[i];
    }
  }
  cout << "Value to enter: " << (abs(shortest.x) + abs(shortest.y)) << endl;
  return 0;
}
