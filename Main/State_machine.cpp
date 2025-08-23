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
  switch (currentState) {
    case State::NUMS:
      set(State::SHORT_CUTS);
      break;
    default:
      set(State::NUMS);
      break;
  }
}

// Retunerar nuvarande State
State StateMachine::current() {
  return currentState;
}

// Byter till APPLICATIONS state
void StateMachine::setApplications() {
  set(State::APPLICATIONS);
}