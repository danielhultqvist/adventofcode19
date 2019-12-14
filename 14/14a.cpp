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

string calculateIndentation(int indentation) {
  stringstream ss;
  for (int i = 0; i < indentation; ++i) {
    ss << "  ";
  }
  return ss.str();
}

void printExtraQuantities(const string &indentationPrefix, const map<string, int> &extraManufacturedChemicals) {
  stringstream ss;
  ss << indentationPrefix << "Extra quantities of ";
  for (auto &entry : extraManufacturedChemicals) {
    ss << "(" << entry.first << "," << entry.second << "), ";
  }
  cout << ss.str() << endl << endl;
}

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
  string chemical, int wantedOutputQuantity, map<string, int> &extraManufacturedChemicals,
  map<string, Recipe> &recipes, int indentation) {
  string indentationPrefix = calculateIndentation(indentation);

  Recipe &recipe = recipes.find(chemical)->second;

  cout << indentationPrefix << "Need to manufacture " << wantedOutputQuantity << " " << chemical << endl;

  int numBatches =
    wantedOutputQuantity / recipe.outputQuantity + (wantedOutputQuantity % recipe.outputQuantity != 0 ? 1 : 0);

  for (auto &ingredient : recipe.chemicalAndQuantities) {
    int wantedQuantity = ingredient.second * numBatches;

    if (ingredient.first == "ORE") {
      extraManufacturedChemicals["ORE"] =
        extraManufacturedChemicals.find("ORE") != extraManufacturedChemicals.end()
        ? extraManufacturedChemicals["ORE"] = extraManufacturedChemicals["ORE"] + wantedQuantity
        : extraManufacturedChemicals["ORE"] = wantedQuantity;

      cout << indentationPrefix << "---- Adding " << wantedQuantity << " ore" << endl;
      continue;
    }

    int alreadyHaveQuantity = extraManufacturedChemicals.find(ingredient.first) != extraManufacturedChemicals.end()
                              ? extraManufacturedChemicals.find(ingredient.first)->second : 0;

    int neededQuantity = wantedQuantity - alreadyHaveQuantity;

    cout << indentationPrefix << "Want " << wantedQuantity << " " << ingredient.first << ", already have "
         << alreadyHaveQuantity
         << ", will manufacture " << neededQuantity << " more" << endl;

    if (neededQuantity > 0) {
      manufactureChemical(ingredient.first, neededQuantity, extraManufacturedChemicals, recipes, indentation + 1);
    } else {
      extraManufacturedChemicals[ingredient.first] = extraManufacturedChemicals[ingredient.first] - wantedQuantity;
    }
  }

  extraManufacturedChemicals[chemical] = numBatches * recipe.outputQuantity - wantedOutputQuantity;
  printExtraQuantities(indentationPrefix, extraManufacturedChemicals);
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

  map<string, int> extraManufacturedChemicals;
  string fuel = "FUEL";
  manufactureChemical(fuel, 1, extraManufacturedChemicals, recipes, 0);

  cout << "Ore required: " << extraManufacturedChemicals["ORE"] << endl;

  return 0;
}