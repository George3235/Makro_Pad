#ifndef CONFIG_H
#define CONFIG_H

/* ───── ROTARY ENCODER ─────────────────────────────── */
#define ENC_DT   6   // GP6
#define ENC_CLK  7   // GP7
#define ENC_SW   8   // GP8

/* ───── MATRIX ─────────────────────────────────────── */
const uint8_t cols[3] = {1, 2, 12};   // GP1, GP2, GP12
const uint8_t rows[3] = {3, 4, 5};    // GP3, GP4, GP5

// Keymap (ändra inte)
const char keymap[3][3] = {
  { '7', '4', '1' },   // rad 0
  { '8', '5', '2' },   // rad 1
  { '9', '6', '3' }    // rad 2
};

/* ───── ANNAT ─────────────────────────────────────── */
#define EEPROM_SIZE  9
#define LED_PIN      13   // indikator-LED

#endif
