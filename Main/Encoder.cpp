#include "Encoder.h"

EncoderHandler::EncoderHandler(USBHIDConsumerControl& consumer)
  : _consumer(consumer) {}

// Funktion som startar allting som har med Encoder att göra
void EncoderHandler::begin() {
    pinMode(ENC_DT,  INPUT_PULLUP);
    pinMode(ENC_CLK, INPUT_PULLUP);
    pinMode(ENC_SW,  INPUT_PULLUP);

    lastClk        = digitalRead(ENC_CLK);
    buttonState    = digitalRead(ENC_SW);
    lastButtonPress = 0;
    longPressFired  = false;

    // Initiera stateMachine 
    stateMachine.begin();
}

// Funktion för att uppdatera Encodern's state och egenskaper
void EncoderHandler::update() {
    // --- Rotary ---
    int clkState = digitalRead(ENC_CLK);
    if (clkState != lastClk) {
        uint32_t now = millis();
        if (now - lastChangeMs > 1) { // enkel debounce
            if (digitalRead(ENC_DT) != clkState) {
                volumeUp();
            } else {
                volumeDown();
            }
            lastChangeMs = now;
        }
        lastClk = clkState;
    }

    // --- Button (single/double/long) ---
    int sw = digitalRead(ENC_SW);

    // edge: HIGH -> LOW = press
    if (buttonState == HIGH && sw == LOW) {
        pressStart = millis();
        longPressFired = false;
    }

    // håll-koll: long press
    if (sw == LOW && !longPressFired) {
        if (millis() - pressStart > 800) { // 800 ms long-press
            onLongPress();
            longPressFired = true;
        }
    }

    // edge: LOW -> HIGH = release
    if (buttonState == LOW && sw == HIGH) {
        uint32_t now = millis();
        if (!longPressFired) {
            onClick();
        }
    }
    buttonState = sw;
}

// Om man vrider åt höger -> höjer volymen
void EncoderHandler::volumeUp() {
    _consumer.press(HID_USAGE_CONSUMER_VOLUME_INCREMENT); // från TinyUSB enum
    delay(2);
    _consumer.release();
}

// Om man vrider åt vänster -> Sänker volymen
void EncoderHandler::volumeDown() {
    _consumer.press(HID_USAGE_CONSUMER_VOLUME_DECREMENT); // från TinyUSB enum
    delay(2);
    _consumer.release();
}

// Om man trycker på Encodern's knapp snabbt -> Mute / Unmute
void EncoderHandler::onClick() {
  _consumer.press(HID_USAGE_CONSUMER_MUTE); 
  delay(2); 
  _consumer.release();
}


// Om användaren trycker en längre period ska den byta layer
void EncoderHandler::onLongPress() {
    stateMachine.toggle();
}
