#include "HeapIntcodeProgram.h"
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

void HeapIntcodeProgram::run(const vector<long long> &inputValues) {
  currentState = State::PROCESSING;
  long long inputPointer = 0;
  performedOperations = 0;
  while (readMemory(pointer) != 99) {
    auto opcode = static_cast<Operation>(readMemory(pointer) % 10);
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

        int mode = readMemory(pointer) / 100 % 10;
        if (mode == 0) {
          writeMemory(readMemory(pointer + 1), inputValues[inputPointer++]);
        } else if (mode == 2) {
          writeMemory(currentRelativeBase + readMemory(pointer + 1), inputValues[inputPointer++]);
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

  if (readMemory(pointer) == 99) {
    currentState = State::HALTED;
  }
}

long long HeapIntcodeProgram::getParameter(int mode, int offset) {
  switch (mode) {
    case 0:
      return readMemory(readMemory(pointer + offset));
    case 1:
      return readMemory(pointer + offset);
    case 2:
      return readMemory(currentRelativeBase + readMemory(pointer + offset));
    default:
      throw runtime_error("Unknown parameter mode");
  }
}

void HeapIntcodeProgram::add() {
  int firstMode = (readMemory(pointer) / 100) % 10;
  int secondMode = (readMemory(pointer) / 1000) % 10;
  int thirdMode = (readMemory(pointer) / 10000) % 10;

  long long firstParameter = getParameter(firstMode, 1);
  long long secondParameter = getParameter(secondMode, 2);

  long long result = firstParameter + secondParameter;

  if (thirdMode == 0) {
    writeMemory(readMemory(pointer + 3), result);
  } else if (thirdMode == 2) {
    writeMemory(currentRelativeBase + readMemory(pointer + 3), result);
  } else {
    throw runtime_error("Unmapped mode for mode: " + to_string(thirdMode));
  }
}

void HeapIntcodeProgram::multiply() {
  int firstMode = (readMemory(pointer) / 100) % 10;
  int secondMode = (readMemory(pointer) / 1000) % 10;
  int thirdMode = (readMemory(pointer) / 10000) % 10;

  long long firstParameter = getParameter(firstMode, 1);
  long long secondParameter = getParameter(secondMode, 2);

  long long result = firstParameter * secondParameter;

  if (thirdMode == 0) {
    writeMemory(readMemory(pointer + 3), result);
  } else if (thirdMode == 2) {
    writeMemory(currentRelativeBase + readMemory(pointer + 3), result);
  } else {
    throw runtime_error("Unmapped mode for mode: " + to_string(thirdMode));
  }
}

void HeapIntcodeProgram::output() {
  int mode = readMemory(pointer) / 100 % 10;
  currentOutput.push_back(getParameter(mode, 1));
  currentState = State::OUTPUT_AVAILABLE;
}

void HeapIntcodeProgram::jumpIfTrue() {
  int firstMode = (readMemory(pointer) / 100) % 10;
  int secondMode = (readMemory(pointer) / 1000) % 10;

  long long valueToTest = getParameter(firstMode, 1);

  if (valueToTest != 0) {
    pointer = getParameter(secondMode, 2);
  } else {
    pointer += 3;
  }
}

void HeapIntcodeProgram::jumpIfFalse() {
  int firstMode = (readMemory(pointer) / 100) % 10;
  int secondMode = (readMemory(pointer) / 1000) % 10;

  long long valueToTest = getParameter(firstMode, 1);

  if (valueToTest == 0) {
    pointer = getParameter(secondMode, 2);
  } else {
    pointer += 3;
  }
}

void HeapIntcodeProgram::lessThan() {
  int firstMode = (readMemory(pointer) / 100) % 10;
  int secondMode = (readMemory(pointer) / 1000) % 10;
  int thirdMode = (readMemory(pointer) / 10000) % 10;

  long long firstParameter = getParameter(firstMode, 1);
  long long secondParameter = getParameter(secondMode, 2);

  long long result = firstParameter < secondParameter ? 1 : 0;

  if (thirdMode == 0) {
    writeMemory(readMemory(pointer + 3), result);
  } else if (thirdMode == 2) {
    writeMemory(currentRelativeBase + readMemory(pointer + 3), result);
  } else {
    throw runtime_error("Unmapped mode for mode: " + to_string(thirdMode));
  }
}

void HeapIntcodeProgram::equals() {
  int firstMode = (readMemory(pointer) / 100) % 10;
  int secondMode = (readMemory(pointer) / 1000) % 10;
  int thirdMode = (readMemory(pointer) / 10000) % 10;

  long long firstParameter = getParameter(firstMode, 1);
  long long secondParameter = getParameter(secondMode, 2);

  long long result = firstParameter == secondParameter ? 1 : 0;

  if (thirdMode == 0) {
    writeMemory(readMemory(pointer + 3), result);
  } else if (thirdMode == 2) {
    writeMemory(currentRelativeBase + readMemory(pointer + 3), result);
  } else {
    throw runtime_error("Unmapped mode for mode: " + to_string(thirdMode));
  }
}

void HeapIntcodeProgram::adjustBase() {
  int mode = (readMemory(pointer) / 100) % 10;

  switch (mode) {
    case 0:
      currentRelativeBase += readMemory(readMemory(pointer + 1));
      break;
    case 1:
      currentRelativeBase += readMemory(pointer + 1);
      break;
    case 2: {
      long long value = readMemory(pointer + 1);
      currentRelativeBase += readMemory(currentRelativeBase + value);
    }
      break;
    default:
      throw runtime_error("Unknown parameter mode");
  }
}

long long HeapIntcodeProgram::readMemory(long position) {
  if (position < memory.size()) {
    return memory[position];
  } else {
    auto heapValue = heap.find(position);
    return heapValue != heap.end() ? heapValue->second : 0;
  }
}

void HeapIntcodeProgram::writeMemory(long position, long long value) {
  if (position < memory.size()) {
    memory[position] = value;
  } else {
    heap[position] = value;
  }
}

std::vector<long long> HeapIntcodeProgram::getOutputAndReset() {
  vector<long long> outputToReturn = currentOutput;
  currentOutput = {};
  return outputToReturn;
}
