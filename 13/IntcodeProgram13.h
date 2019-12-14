#ifndef ADVENTOFCODE_INTCODEPROGRAM_H
#define ADVENTOFCODE_INTCODEPROGRAM_H

#include <string>
#include <vector>

enum class State {
  WAITING_FOR_INPUT,
  PROCESSING,
  OUTPUT_AVAILABLE,
  HALTED
};

class IntcodeProgram13 {
public:
  static IntcodeProgram13 loadProgram(std::vector<long long> &memory) {
    return IntcodeProgram13(memory);
  }

  void run(const std::vector<long long> &inputValues);

  std::vector<long long> getOutputAndReset();

  State state() {
    return currentState;
  }

private:
  explicit IntcodeProgram13(std::vector<long long> memory) :
    memory(std::move(memory)), pointer(0), performedOperations(0), currentRelativeBase(0), currentOutput({}),
    currentState(State::WAITING_FOR_INPUT) {}

  std::vector<long long> memory;

  long long pointer;

  long long currentRelativeBase;

  std::vector<long long> currentOutput;

  State currentState;

  long long performedOperations;

  long long getParameter(int mode, int offset);

  void add();

  void multiply();

  void output();

  void jumpIfTrue();

  void jumpIfFalse();

  void lessThan();

  void equals();

  void adjustBase();
};

#endif //ADVENTOFCODE_INTCODEPROGRAM_H
