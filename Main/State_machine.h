#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "stdint.h"

// Enum with the states

enum class State : uint8_t {
  NUMS = 1,                        // Numbers 1-9
  SHORT_CUTS = 2,                  // Short Cuts
  APPLICATIONS = 3                
};

class StateMachine {
  public: 
    using ChangeCallBack = void (*) (State newState);

    StateMachine();

    // Initiera med ett startläge.
    void begin(State initial = State::NUMS);

    // Sätt läge explicit 
    void set (State s); 

    // Växla mellan lägen 
    void toggle ();

    // Läsa av nuvarande state
    State current(); 

  private: 
    State _state = State::NUMS;
    ChangeCallBack _onChange = nullptr; 

    // Anropa denna i set() när läget faktiskt ändrats
    void notify();
};
#endid