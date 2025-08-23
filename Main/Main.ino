#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"
#include <EEPROM.h>

#include "Config.h"        // pinnar, keymap, LED_PIN, EEPROM_SIZE
//#include "hid_usages.h"    // HID Usage-konstanter
#include "Encoder.h"       // EncoderHandler-klass

USBHIDKeyboard Keyboard;
USBHIDConsumerControl Consumer;
EncoderHandler encoder(Consumer);

void setup() {
  Serial.begin(115200);
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM init failed!");
  }

  // Starta USB HID
  USB.begin();
  Keyboard.begin();
  Consumer.begin();   // viktigt för Consumer Control (volym/mute m.m.)

  // Initiera matrispinnar
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(cols[i], OUTPUT);
    digitalWrite(cols[i], HIGH);     // kolumner inaktiva = HIGH
    pinMode(rows[i], INPUT_PULLUP);  // rader input med pullup
  }

  // Initiera encoder
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

        char key = keymap[r][c];
        digitalWrite(LED_PIN, HIGH);

        // Sänd tecknet via USB HID-tangentbord
        Keyboard.print(key);

        // Enkel debounce så vi inte spammar
        delay(200);

        // Vänta på release (valfritt men brukar ge bättre känsla)
        while (digitalRead(rows[r]) == LOW) {
          delay(2);
          encoder.update();  // låt encoder fortsätta fungera under hållning
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
