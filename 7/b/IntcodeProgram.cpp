#include "IntcodeProgram.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

void IntcodeProgram::run(const vector<int> &input) {
  currentState = State::PROCESSING;
  int inputPointer = 0;
  while (memory[pointer] != 99 && currentState != State::OUTPUT_AVAILABLE) {
    int opcode = memory[pointer] % 10;
    switch (opcode) {
      case 1:
        add();
        pointer += 4;
        break;
      case 2:
        multiply();
        pointer += 4;
        break;
      case 3:
        if (inputPointer == input.size()) {
          currentState = State::WAITING_FOR_INPUT;
          return;
        }

        memory[memory[pointer + 1]] = input[inputPointer++];
        pointer += 2;
        break;
      case 4:
        output();
        pointer += 2;
        break;
      case 5:
        jumpIfTrue();
        break;
      case 6:
        jumpIfFalse();
        break;
      case 7:
        lessThan();
        pointer += 4;
        break;
      case 8:
        equals();
        pointer += 4;
        break;
      default:
        throw runtime_error("Unmapped opcode: " + to_string(opcode));
    }
  }

  if (memory[pointer] == 99) {
    currentState = State::HALTED;
  }
}

void IntcodeProgram::add() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  int secondParameter = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];

  memory[memory[pointer + 3]] = firstParameter + secondParameter;
}

void IntcodeProgram::multiply() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  int secondParameter = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];

  memory[memory[pointer + 3]] = firstParameter * secondParameter;
}

void IntcodeProgram::output() {
  int mode = memory[pointer] / 100 % 10;
  currentOutput = mode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  currentState = State::OUTPUT_AVAILABLE;
}

// Rewrite to return pointer value?
void IntcodeProgram::jumpIfTrue() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int valueToTest = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  if (valueToTest != 0) {
    pointer = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];
  } else {
    pointer += 3;
  }
}

void IntcodeProgram::jumpIfFalse() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;
  int valueToTest = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  if (valueToTest == 0) {
    pointer = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];
  } else {
    pointer += 3;
  }
}

void IntcodeProgram::lessThan() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  int secondParameter = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];

  if (firstParameter < secondParameter) {
    memory[memory[pointer + 3]] = 1;
  } else {
    memory[memory[pointer + 3]] = 0;
  }
}

void IntcodeProgram::equals() {
  int firstMode = (memory[pointer] / 100) % 10;
  int secondMode = (memory[pointer] / 1000) % 10;

  int firstParameter = firstMode == 1 ? memory[pointer + 1] : memory[memory[pointer + 1]];
  int secondParameter = secondMode == 1 ? memory[pointer + 2] : memory[memory[pointer + 2]];

  if (firstParameter == secondParameter) {
    memory[memory[pointer + 3]] = 1;
  } else {
    memory[memory[pointer + 3]] = 0;
  }
}

int IntcodeProgram::getLastOutput() {
  if (currentState == State::HALTED || currentState == State::OUTPUT_AVAILABLE) {
    return currentOutput;
  } else {
    throw runtime_error("Output not available, waiting for input state");
  }
}
