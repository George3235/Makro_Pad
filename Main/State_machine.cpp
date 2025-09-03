#include "State_machine.h"

StateMachine::StateMachine() {}

void StateMachine::begin() {
  set(State::NUMS);
}

void StateMachine::set(State s) {
  if (currentState == s) return;
  currentState = s;
}

// Byter state mellan NUMS och SHORT_CUTS
void StateMachine::toggle() {
  currentState = (currentState == State::NUMS) ? State::SHORT_CUTS : State::NUMS; 
}

// Retunerar nuvarande State
State StateMachine::current() {
  return currentState;
}
