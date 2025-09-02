#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"
#include <EEPROM.h>

#include "Config.h"        // pinnar, keymap, LED_PIN, EEPROM_SIZE
#include "State_machine.h"
#include "Encoder.h"
#include "Shortcuts.h"

// --- Skapa HID-objekten först ---
USBHIDKeyboard Keyboard;
USBHIDConsumerControl Consumer;

// --- Shortcuts-hjälpare (Windows-only) ---
Shortcuts shortcuts(Keyboard, &Consumer);

// --- Encoder (äger din StateMachine inuti sig) ---
EncoderHandler encoder(Consumer);

// --- 3x3 mapping för SHORT_CUTS-läget ---
static const Shortcut shortcutsMap[3][3] = {
  { SC_COPY,       SC_UNDO,      SC_SCREENSHOT        },  // rad 0
  { SC_PASTE,       SC_SELECT_ALL, SC_RENAME    },  // rad 1
  { SC_CUT, SC_NEW_TAB,       SC_PAUS       }   // rad 2
};

// --- Hjälpfunktion: skicka rätt sak beroende på nuvarande State ---
static inline void handleKeyPress(uint8_t r, uint8_t c) {
  State st = encoder.stateMachine.current();
  if (st == State::NUMS) {
    // Siffror via keymap
    Keyboard.print(keymap[r][c]);
  } else {
    // Genvägar (Windows): copy/paste/screenshot/URL/play-pause osv.
    shortcuts.send(shortcutsMap[r][c]);
  }
}

void setup() {
  Serial.begin(115200);
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM init failed!");
  }

  // Starta USB HID
  USB.begin();
  Keyboard.begin();
  Consumer.begin();   // viktigt för Consumer Control (play/pause m.m.)

  // Initiera matrispinnar
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(cols[i], OUTPUT);
    digitalWrite(cols[i], HIGH);     // kolumner inaktiva = HIGH
    pinMode(rows[i], INPUT_PULLUP);  // rader input med pullup
  }

  // Initiera encoder (inkl. stateMachine.begin())
  encoder.begin();

  // LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  delay(500);  // ge USB lite tid att enumerera
}

void loop() {
  // Hantera encoder (vridning + knapp)
  encoder.update();

  // Skanna tangentmatrisen
  bool anyPressed = false;

  for (uint8_t c = 0; c < 3; c++) {
    // Aktivera kolumn (LOW) en i taget
    digitalWrite(cols[c], LOW);

    for (uint8_t r = 0; r < 3; r++) {
      if (digitalRead(rows[r]) == LOW) {  // knappen tryckt (aktiv låg)
        anyPressed = true;
        digitalWrite(LED_PIN, HIGH);

        // Skicka beroende på State (NUMS -> siffra, annars -> genväg)
        handleKeyPress(r, c);

        // Enkel debounce
        delay(200);

        // Vänta på release (så vi inte spammar vid hållning)
        while (digitalRead(rows[r]) == LOW) {
          delay(2);
          encoder.update();  // låt encoder funka under hållning
        }
      }
    }

    // Inaktivera kolumn
    digitalWrite(cols[c], HIGH);
  }

  if (!anyPressed) {
    digitalWrite(LED_PIN, LOW);
  }

  delay(2); // liten paus för att inte maxa CPU
}
