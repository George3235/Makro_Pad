#include "State_machine.h"
// Instance 
StateMachine::StateMachine() {}

// Sätter igång State machine med start läge NUMS 
void StateMachine::begin() {
  set(State::NUMS);
}

// Funktion för att ändra på State
void StateMachine::set(State s) {
  if (currentState == s) return;
  currentState = s;
}

// Byter state mellan NUMS och SHORT_CUTS
void StateMachine::toggle() {
  currentState = (currentState == State::NUMS) ? State::SHORT_CUTS : State::NUMS; 
}

// Hjälp funktion för att ta reda på nuvarande State
// Retunerar nuvarande State
State StateMachine::current() {
  return currentState;
}
