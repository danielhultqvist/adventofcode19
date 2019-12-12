#include "IntcodeProgram.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

enum class Operation : int {
  ADD = 1,
  MULTIPLY,
  INPUT,
  OUTPUT,
  JUMP_IF_TRUE,
  JUMP_IF_FALSE,
  LESS_THAN,
  EQUALS,
  ADJUST_BASE
};

void IntcodeProgram::run(const vector<long long> &inputValues) {
  currentState = State::PROCESSING;
  long long inputPointer = 0;
  performedOperations = 0;
  while (memory[pointer] != 99) {
    auto opcode = static_cast<Operation>(memory[pointer] % 10);
    switch (opcode) {
      case Operation::ADD:
        add();
        pointer += 4;
        break;
      case Operation::MULTIPLY:
        multiply();
        pointer += 4;
        break;
      case Operation::INPUT: {
        if (inputPointer == inputValues.size()) {
          currentState = State::WAITING_FOR_INPUT;
          return;
        }

        int mode = memory[pointer] / 100 % 10;
        if (mode == 0) {
          memory[memory[pointer + 1]] = inputValues[inputPointer++];
        } else if (mode == 2) {
          memory[currentRelativeBase + memory[pointer + 1]] = inputValues[inputPointer++];
        } else {
          throw runtime_error("Unmapped mode for input: " + to_string(mode));
        }

        pointer += 2;
        break;
      }
      case Operation::OUTPUT:
        output();
        pointer += 2;
        break;
      case Operation::JUMP_IF_TRUE:
        jumpIfTrue();
        break;
      case Operation::JUMP_IF_FALSE:
        jumpIfFalse();
        break;
      case Operation::LESS_THAN:
        lessThan();
        pointer += 4;
        break;
      case Operation::EQUALS:
        equals();
        pointer += 4;
        break;
      case Operation::ADJUST_BASE:
        adjustBase();
        pointer += 2;
        break;
      default:
        throw runtime_error("Unmapped opcode: " + to_string(static_cast<int>(opcode)));
    }
    performedOperations++;
  }

  if (memory[pointer] == 99) {
    currentState = State::HALTED;
  }
}

long long IntcodeProgram::getParameter(int mode, int offset) {
  switch (mode) {
    case 0:
      return memory[memory[pointer + offset]];
    case 1:
      return memory[pointer + offset];
    case 2:
      return memory[currentRelativeBase + memory[pointer + offset]];
    default:
      throw runtime_error("Unknown parameter mode");
  }
}

void IntcodeProgram::add() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int thirdMode = (memory[pointer] / 10000) % 10;

  long long firstParameter = getParameter(firstMode, 1);
  long long secondParameter = getParameter(secondMode, 2);

  long long result = firstParameter + secondParameter;

  if (thirdMode == 0) {
    memory[memory[pointer + 3]] = result;
  } else if (thirdMode == 2) {
    memory[currentRelativeBase + memory[pointer + 3]] = result;
  } else {
    throw runtime_error("Unmapped mode for mode: " + to_string(thirdMode));
  }
}

void IntcodeProgram::multiply() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int thirdMode = (memory[pointer] / 10000) % 10;

  long long firstParameter = getParameter(firstMode, 1);
  long long secondParameter = getParameter(secondMode, 2);

  long long result = firstParameter * secondParameter;

  if (thirdMode == 0) {
    memory[memory[pointer + 3]] = result;
  } else if (thirdMode == 2) {
    memory[currentRelativeBase + memory[pointer + 3]] = result;
  } else {
    throw runtime_error("Unmapped mode for mode: " + to_string(thirdMode));
  }
}

void IntcodeProgram::output() {
  int mode = memory[pointer] / 100 % 10;
  currentOutput.push_back(getParameter(mode, 1));
  currentState = State::OUTPUT_AVAILABLE;
}

void IntcodeProgram::jumpIfTrue() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  long long valueToTest = getParameter(firstMode, 1);

  if (valueToTest != 0) {
    pointer = getParameter(secondMode, 2);
  } else {
    pointer += 3;
  }
}

void IntcodeProgram::jumpIfFalse() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  long long valueToTest = getParameter(firstMode, 1);

  if (valueToTest == 0) {
    pointer = getParameter(secondMode, 2);
  } else {
    pointer += 3;
  }
}

void IntcodeProgram::lessThan() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int thirdMode = (memory[pointer] / 10000) % 10;

  long long firstParameter = getParameter(firstMode, 1);
  long long secondParameter = getParameter(secondMode, 2);

  long long result = firstParameter < secondParameter ? 1 : 0;

  if (thirdMode == 0) {
    memory[memory[pointer + 3]] = result;
  } else if (thirdMode == 2) {
    memory[currentRelativeBase + memory[pointer + 3]] = result;
  } else {
    throw runtime_error("Unmapped mode for mode: " + to_string(thirdMode));
  }
}

void IntcodeProgram::equals() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int thirdMode = (memory[pointer] / 10000) % 10;

  long long firstParameter = getParameter(firstMode, 1);
  long long secondParameter = getParameter(secondMode, 2);

  long long result = firstParameter == secondParameter ? 1 : 0;

  if (thirdMode == 0) {
    memory[memory[pointer + 3]] = result;
  } else if (thirdMode == 2) {
    memory[currentRelativeBase + memory[pointer + 3]] = result;
  } else {
    throw runtime_error("Unmapped mode for mode: " + to_string(thirdMode));
  }
}

void IntcodeProgram::adjustBase() {
  int mode = (memory[pointer] / 100) % 10;

  switch (mode) {
    case 0:
      currentRelativeBase += memory[memory[pointer + 1]];
      break;
    case 1:
      currentRelativeBase += memory[pointer + 1];
      break;
    case 2: {
      long long value = memory[pointer + 1];
      currentRelativeBase += memory[currentRelativeBase + value];
    }
      break;
    default:
      throw runtime_error("Unknown parameter mode");
  }
}

std::vector<long long> IntcodeProgram::getOutputAndReset() {
  vector<long long> outputToReturn = currentOutput;
  currentOutput = {};
  return outputToReturn;
}
