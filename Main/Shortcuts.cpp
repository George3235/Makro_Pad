#include "esp32-hal.h"
#include "Shortcuts.h"

Shortcuts::Shortcuts(USBHIDKeyboard& kb, USBHIDConsumerControl* cc)
  : _kb(kb), _cc(cc) {}

void Shortcuts::setChatUrl(const char* url) { _chatUrl = url; }

void Shortcuts::openURLWindows(const char* url) const {
  // Öppna Kör-fönstret, skriv URL och tryck Enter
  press2(KEY_LEFT_GUI, 'r');      // Win + R
  delay(150);
  _kb.print(url);
  _kb.write('\n');                // Enter
}

void Shortcuts::send(Shortcut s) const {
  switch (s) {
    case SC_COPY:        press2(KEY_LEFT_CTRL, 'c'); break;
    case SC_PASTE:       press2(KEY_LEFT_CTRL, 'v'); break;
    case SC_CUT:         press2(KEY_LEFT_CTRL, 'x'); break;
    case SC_UNDO:        press2(KEY_LEFT_CTRL, 'z'); break;
    case SC_REDO:        press2(KEY_LEFT_CTRL, 'y'); break;   // alt: CTRL+SHIFT+'z'
    case SC_SELECT_ALL:  press2(KEY_LEFT_CTRL, 'a'); break;
    case SC_NEW_TAB:     press2(KEY_LEFT_CTRL, 't'); break;
    case SC_CLOSE_TAB:   press2(KEY_LEFT_CTRL, 'w'); break;

    case SC_SCREENSHOT:  press3(KEY_LEFT_GUI, KEY_LEFT_SHIFT, 's'); break; // Snipping Tool
    case SC_RENAME:      _kb.press(KEY_F2); delay(8); _kb.releaseAll(); break;

    case SC_PAUS:
      if (_cc) { _cc->press(CONSUMER_CONTROL_PLAY_PAUSE); delay(2); _cc->release(); }
      else     { _kb.write(' '); } // fallback (t.ex. YouTube)
      break;
  }
}
