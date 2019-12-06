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

void calculateCost(shared_ptr<Node> &node, int offsetFromCom) {
  node->setCost(offsetFromCom);
  for (auto & orbitingNode : node->nodesOrbiting) {
    calculateCost(orbitingNode, offsetFromCom + 1);
  }
}

// General idea, start from COM and find distance to all nodes.
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

  shared_ptr<Node> comNode = nodes["COM"];
  calculateCost(comNode, 0);

  int totalCost = 0;
  for (auto &node : nodes) {
//    cout << node.first << " => " << node.second->cost << '\n';
    totalCost += node.second->cost;
  }

  steady_clock::time_point end = steady_clock::now();
  cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
  cout << "Total orbits: " << totalCost;
}
