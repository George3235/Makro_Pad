#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "Config.h"
#include "USBHIDConsumerControl.h"  // innehåller TinyUSB HID-usage enum via beroenden
#include "State_machine.h"

class EncoderHandler {
public:
    explicit EncoderHandler(USBHIDConsumerControl& consumer);

    void begin();   // initiera pinnar
    void update();  // körs i loop()

    void onClick();       // singelklick
    void onDoubleClick(); // dubbelklick
    void onLongPress();   // långtryck

    StateMachine stateMachine;   // Variabel av state machine 

private:
    USBHIDConsumerControl& _consumer;

    void volumeUp();
    void volumeDown();

    int lastClk = HIGH;
    uint32_t lastChangeMs = 0;
    bool isMuted = false;

    // knapp-state
    int buttonState = HIGH;
    uint32_t lastButtonPress = 0;
    uint32_t pressStart = 0;
    bool longPressFired = false;
};

#endif
