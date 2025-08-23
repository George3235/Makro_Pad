#include "State_machine.h"

StateMachine::StateMachine() {}

void StateMachine::begin(State initial) {
  _state = initial;
}

void StateMachine::set(State s) {
  if (_state == s) return;
  _state = s;
  notify();
}

void StateMachine::toggle() {
  switch (_state) {
    case State::NUMS:
      set(State::SHORT_CUTS);
      break;
    case State::SHORT_CUTS:
      set(State::APPLICATIONS);
      break;
    default:
      set(State::NUMS);
      break;
  }
}

// Retunerar nuvarande State
State StateMachine::current() {
  return _state;
}

void StateMachine::notify() {
  if(_onChange) {
    _onChange(_state);
  }
}