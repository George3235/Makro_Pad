#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <Arduino.h>
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

// Namn matchar din karta
enum Shortcut : uint8_t {
  SC_COPY,       SC_PASTE,       SC_CUT,
  SC_UNDO,       SC_REDO,        SC_SCREENSHOT,
  SC_SELECT_ALL, SC_NEW_TAB,     SC_CLOSE_TAB,
  SC_RENAME,    // F2 (REN)
  SC_PAUS     // media play/pause
};

class Shortcuts {
public:
  // cc får gärna pekas in för mediaknappar (play/pause); annars funkar fallback (mellanslag)
  explicit Shortcuts(USBHIDKeyboard& kb, USBHIDConsumerControl* cc = nullptr);

  void send(Shortcut s) const;

  // Om du vill byta URL utan att kompilera om all kod kan du sätta den här
  void setChatUrl(const char* url);

private:
  USBHIDKeyboard& _kb;
  USBHIDConsumerControl* _cc;
  const char* _chatUrl =
    "https://chatgpt.com/g/g-p-67eafc43a19881919767b3c78709ed2e-george/project";

  // Hjälpare
  inline void press2(uint8_t k1, uint8_t k2) const {
    _kb.press(k1); _kb.press(k2); delay(8); _kb.releaseAll();
  }
  inline void press3(uint8_t k1, uint8_t k2, uint8_t k3) const {
    _kb.press(k1); _kb.press(k2); _kb.press(k3); delay(8); _kb.releaseAll();
  }

  void openURLWindows(const char* url) const; // Win+R → URL → Enter
};

#endif
