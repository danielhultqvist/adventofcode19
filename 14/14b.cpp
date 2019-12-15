#include <utility>

#include <utility>

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

/*
 * Important stuff
 * - every chemical is produced by exactly one reaction (except ORE)
 * - Can only produce integer number of resources
 * - Creating leftovers are fine
 *
 * General idea:
 * - Traverse from the end. Have a bag of available resources right now
 * - Look at what dependencies needed from the graph. Create those
 */
class Recipe {
public:
  Recipe(string outputName, map<string, int> chemicalsAndQuantities, int numResult) :
    outputName(std::move(outputName)), chemicalAndQuantities(std::move(chemicalsAndQuantities)),
    outputQuantity(numResult) {}

  Recipe() = default;

  string outputName;
  map<string, int> chemicalAndQuantities;
  int outputQuantity;
};

Recipe parseRecipe(const string &value) {
  string output = value.substr(value.find('=') + 3);
  int outputQuantity = stoi(output.substr(0, output.find(' ')));
  string outputName = output.substr(output.find(' ') + 1);

  map<string, int> chemicalsAndQuantities;
  string input = value.substr(0, value.find(" ="));

  stringstream ss(input);
  string element;
  while (getline(ss, element, ',')) {
    string trimmed = element.erase(0, element.find_first_not_of(" \n\r\t"));
    int quantity = stoi(trimmed.substr(0, trimmed.find(' ')));
    string inputName = trimmed.substr(trimmed.find(' ') + 1);
    chemicalsAndQuantities[inputName] = quantity;
  }
  return Recipe(outputName, chemicalsAndQuantities, outputQuantity);
}


void manufactureChemical(
  string chemical, long long wantedOutputQuantity, map<string, long long> &extraManufacturedChemicals,
  map<string, Recipe> &recipes, int indentation) {

  Recipe &recipe = recipes.find(chemical)->second;

  long long numBatches =
    wantedOutputQuantity / recipe.outputQuantity + (wantedOutputQuantity % recipe.outputQuantity != 0 ? 1 : 0);

  for (auto &ingredient : recipe.chemicalAndQuantities) {
    long long wantedQuantity = ingredient.second * numBatches;

    if (ingredient.first == "ORE") {
      extraManufacturedChemicals["ORE"] =
        extraManufacturedChemicals.find("ORE") != extraManufacturedChemicals.end()
        ? extraManufacturedChemicals["ORE"] = extraManufacturedChemicals["ORE"] + wantedQuantity
        : extraManufacturedChemicals["ORE"] = wantedQuantity;
      continue;
    }

    long long alreadyHaveQuantity = extraManufacturedChemicals.find(ingredient.first) != extraManufacturedChemicals.end()
                              ? extraManufacturedChemicals.find(ingredient.first)->second : 0;
    long long neededQuantity = wantedQuantity - alreadyHaveQuantity;

    if (neededQuantity > 0) {
      manufactureChemical(ingredient.first, neededQuantity, extraManufacturedChemicals, recipes, indentation + 1);
    } else {
      extraManufacturedChemicals[ingredient.first] = extraManufacturedChemicals[ingredient.first] - wantedQuantity;
    }
  }

  extraManufacturedChemicals[chemical] = numBatches * recipe.outputQuantity - wantedOutputQuantity;
}

int main() {
  steady_clock::time_point begin = steady_clock::now();
  ifstream data{"input.txt"};
  if (!data) {
    throw runtime_error("Unable to load data");
  }

  map<string, Recipe> recipes;
  string value;
  while (getline(data, value)) {
    Recipe recipe = parseRecipe(value);
    recipes[recipe.outputName] = recipe;
  }

  map<string, long long> extraManufacturedChemicals;
  string fuel = "FUEL";

  const long long MAX_ORE = 1000000000000;

  long long numFuel = 0;
  long long currentUsedOre = 0;
  while (currentUsedOre < MAX_ORE) {
    manufactureChemical(fuel, 1, extraManufacturedChemicals, recipes, 0);
    currentUsedOre = extraManufacturedChemicals["ORE"];

    if (currentUsedOre < MAX_ORE) {
      numFuel++;
    }

    if (numFuel % 10000 == 0) {
      cout << "Reached " << numFuel << " fuel so far" << endl;
    }
  }

  cout << "Potential fuel: " << numFuel << endl;

  return 0;
}