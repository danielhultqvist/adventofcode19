#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <memory>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Node {
public:
  Node(string name): name(name) {}

  void addOrbitingNode(const shared_ptr<Node> node) {
    nodesOrbiting.push_back(node);
  }

  void setCost(int _cost) {
    cost = _cost;
  }

  void setOrbitingAround(shared_ptr<Node> &node) {
    orbitingAround = node;
  }

  vector<shared_ptr<Node>> nodesOrbiting;
  shared_ptr<Node> orbitingAround = nullptr;

  const string name;

  int cost = -1;
};

vector<shared_ptr<Node>> findVectorsToTraverse(const shared_ptr<Node> &currentNode, const shared_ptr<Node> &previousNode) {
  vector<shared_ptr<Node>> nodesToTraverse;
  if (previousNode->name != currentNode->orbitingAround->name) {
    nodesToTraverse.push_back(currentNode->orbitingAround);
  }
  for (auto &orbitingNode : currentNode->nodesOrbiting) {
    if (orbitingNode->name != previousNode->name) {
      nodesToTraverse.push_back(orbitingNode);
    }
  }
  return nodesToTraverse;
}

// Traverse all nodes, forbid going back to where you came from. Early exit if already found
// IFF memory is an issue, just use a queue to traverse iteratively (standard Breadth first search implementation)
int traverseBetween(shared_ptr<Node> &currentNode, shared_ptr<Node> &previousNode, shared_ptr<Node> &endNode, int steps) {
  // Are we at the end node? Return steps needed to get here
  if (currentNode->name == endNode->name) {
    return steps;
  }

  // Have we reached COM leaf? -> abort this path
  if (currentNode->name == "COM") {
    return -1;
  }

  // Find new paths to traverse, forbid where we came from
  vector<shared_ptr<Node>> nodesToTraverse = findVectorsToTraverse(currentNode, previousNode);

  // Are we at a leaf? (nowhere to go) -> abort this path
  if (nodesToTraverse.empty()) {
    return -1;
  } else {
    // Traverse all possible nodes
    for (auto &node : nodesToTraverse) {
      int stepsForNode = traverseBetween(node, currentNode, endNode, steps + 1);

      // Did the path find a way to end node? If so, return it! There is always only one way if acyclic.
      if (stepsForNode != -1) {
        return stepsForNode;
      }
    }

    // No paths would go to end node, abort this path
    return -1;
  }
}

// Idea: Use the fact that it is acyclic
int main() {
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }
  steady_clock::time_point begin = steady_clock::now();

  map<string, shared_ptr<Node>> nodes;
  string value;
  while (getline(data, value)) {
    string left = value.substr(0, value.find(')'));
    string right = value.substr(value.find(')') + 1);

    auto orbitingNode = nodes.count(right) == 1 ? nodes[right] : make_shared<Node>(right);
    auto orbitNode = nodes.count(left) == 1 ? nodes[left] : make_shared<Node>(left);

    orbitNode->addOrbitingNode(orbitingNode);
    orbitingNode->setOrbitingAround(orbitNode);

    nodes[left] = orbitNode;
    nodes[right] = orbitingNode;
  }

  shared_ptr<Node> youNode = nodes["YOU"];
  shared_ptr<Node> santaNode = nodes["SAN"];
  int offsetYouAndSantaNode = 2;
  int steps = traverseBetween(youNode, youNode, santaNode, 0) - offsetYouAndSantaNode;

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
  cout << "Total orbits: " << steps;
}
