#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "stdint.h"

// Enum with the states

enum class State : uint8_t {
  NUMS,                        // Numbers 1-9
  SHORT_CUTS,                  // Short Cuts
};

class StateMachine {
  public: 
    StateMachine();

    // Initiera med ett startläge.
    void begin();

    // Sätt läge explicit 
    void set (State s); 

    // Växla mellan lägen 
    void toggle ();

    // Läsa av nuvarande state
    State current(); 

  private: 
    State currentState = State::NUMS; 
};
#endif