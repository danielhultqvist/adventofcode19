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
  static IntcodeProgram loadProgram(std::vector<int> &memory) {
    return IntcodeProgram(memory);
  }

  void run(const std::vector<int> &input);

  int getLastOutput();

  State state() {
    return currentState;
  }

private:
  explicit IntcodeProgram(std::vector<int> memory) :
    memory(std::move(memory)), pointer(0), currentOutput(-1), currentState(State::WAITING_FOR_INPUT) {}

  std::vector<int> memory;

  int pointer;

  int currentOutput;

  State currentState;

  void add();

  void multiply();

  void output();

  void jumpIfTrue();

  void jumpIfFalse();

  void lessThan();

  void equals();
};

#endif //ADVENTOFCODE_INTCODEPROGRAM_H

/*








 */