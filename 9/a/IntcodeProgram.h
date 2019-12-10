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

class IntcodeProgram {
public:
  static IntcodeProgram loadProgram(std::vector<long long> &memory) {
    return IntcodeProgram(memory);
  }

  void run(const std::vector<long long> &inputValues);

  long long getLastOutput();

  State state() {
    return currentState;
  }

private:
  explicit IntcodeProgram(std::vector<long long> memory) :
    memory(std::move(memory)), pointer(0), performedOperations(0), currentRelativeBase(0), currentOutput(-1),
    currentState(State::WAITING_FOR_INPUT) {}

  std::vector<long long> memory;

  long long pointer;

  long long currentRelativeBase;

  long long currentOutput;

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
