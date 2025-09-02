#include "Encoder.h"

EncoderHandler::EncoderHandler(USBHIDConsumerControl& consumer)
  : _consumer(consumer) {}

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
            // Om man trycker ner knappen längre än 1.5 Sekunder 
            if (now - lastButtonPress >= 1500) { 
                onDoubleClick();
                lastButtonPress = 0; // reset fönstret
            } else {
                onClick();
                lastButtonPress = now;
            }
        }
    }
    buttonState = sw;
}

void EncoderHandler::volumeUp() {
    _consumer.press(HID_USAGE_CONSUMER_VOLUME_INCREMENT); // från TinyUSB enum
    delay(2);
    _consumer.release();
}

void EncoderHandler::volumeDown() {
    _consumer.press(HID_USAGE_CONSUMER_VOLUME_DECREMENT); // från TinyUSB enum
    delay(2);
    _consumer.release();
}

// --- callback-stubs ---
void EncoderHandler::onClick() {
  _consumer.press(HID_USAGE_CONSUMER_MUTE); 
  delay(2); 
  _consumer.release();
}

// Om användaren trycker två gånger efter varandra
// Ska växla mellan States: NUMS och SHORT_CUTS
void EncoderHandler::onDoubleClick() {
    stateMachine.toggle();
}

// Om användaren trycker en längre period 
// Byter direkt till State Applications
void EncoderHandler::onLongPress() {
    stateMachine.setApplications();
}
