# 3×3 MacroPad för ESP32-S3 (Waveshare ESP32-S3 Zero)

Volymratt (rotary), två layers (Numbers & Shortcuts) och HID-tangenter


## Översikt

Detta är ett kompakt 3×3 MacroPad med ESP32-S3 som agerar USB-HID-tangentbord samt Consumer Control (volym m.m.).
En rotary encoder (KY-040) styr volym upp/ner; knapptryck på encodern ger mute/unmute, dubbeltryck växlar layer (Numbers ↔ Shortcuts), och långtryck är reserverat för ett tredje läge (Applications, för framtida funktioner).

Projektet är gjort för Windows-genvägar i Shortcut-lagret, men koden är enkel att ändra för andra OS.

## Funktioner

### Två layers

- Numbers: siffertangenter 1–9 i klassisk telefonpad-ordning

- Shortcuts (Windows): Copy, Paste, Cut, Undo, Select All, New Tab, Screenshot (Win+Shift+S), Rename (F2), Play/Pause (media)

### Rotary encoder

- Vrid höger/vänster: Volym +/–

- Klick: Mute/Unmute

- Långtryck: Växla layer (Numbers ↔ Shortcuts)

#### Anti-ghosting via 1N4148-dioder på varje switch i matrisen

#### Minimal extern hårdvara (använder interna pull-ups)

---------------------------------------------------------------

## Komponenter (BOM)
| Description | Amount |
|-------------| -------|
| Waveshare ESP32-S3 Zero (Esp-S3-WROOM-1/-1U-modul) | 1 |
| KY-040 Rotary Encoder (med tryckknapp)             | 1 |
| Mekaniska brytare (MX-kompatibla)                  | 9 |
| Diod 1n4148 (anti-ghosting, en per brytare)        | 9 |
| Case (3D printad)                                  | 1 |
| Skruva med längd 8 mm                              | 4 |

-------------------------------------------------------------------

## Dator-setup

### Val av IDE
- Arduino IDE (enkelt att komma igång)
- PlatformIO (mer avancerat/repovänligt, valfritt)

### Snabbguide 
1. Installera ESP32 stöd i Arduino IDE (Boards Manager, Espressif Systems, v3.x)
2. Välj kort Waveshare ESP32-S3-Zero
3. USB-inställningar i ***Tools***:
    - `USB CDC On Boot: Enable` 
    - `USB Mode: Default / TinyUSB`
    - `PSRAM: enligt din modul`
    - `Upload Speed: 921600` (ev. 460800 om det strular)
4. Anslut med USB-C, kompilera & ladda upp.
> [!WARNING]
> ***Ingen port?*** Håll ***BOOT***, tryck ***RESET***, släpp >***BOOT*** (download-mode). Välj rätt COM-port och försök igen.
-------------------------------------------------------------------

## Kopplingar (pins & dioder)
### Matris

Koden skannar ***kolumner aktiva låga*** (sätter kolumn ***LOW*** en i taget) och läser rader med `INPUT_PULLUP`.
| Funktion                 | ESP32-S3      | Kommentar |
| -------------------------| --------------| ----------|
| Col 0                    |        1      | Output (normal HIGH, aktiv LOW) |
| Col 1 |  2 | Output (normal HIGH, aktiv LOW) |
| Col 2 | 12 | Output (normal HIGH, aktiv LOW) |
| Row 0 | 3  | Input Pull-Up |
| Row 1 | 4  | Input Pull-Up |
| Row 2 | 5  | Input Pull-Up |

> [!VIKTIG]
> ***Diod-riktning***: Rader är höga via pull-up och kolumnen dras  ***LOW*** när den skannas. <br>
> Strömmen ska går rad -> kolumn när knappen trycks. <br>
> Sätt anod rad, katod (ringen) mot kolumn. 
> Koppling: `Row (anod) ->|-(katod) -> Switch -> Col`.

### Encoder
| Funktion               |  ESP32-S3 GPIO |
| ---------------------- | -------------- |
| ENC_DT                 |        6       |
| ENC_CLK                |        7       |
| ENC_SW (knapp)         |        8       |


## Keymaps (layout per layer)
### Numbers
``` 
7 8 9 
4 5 6 
1 2 3 
 ```
Koden skickar vanliga tecken `1–9` enligt matrisens `[rad][kolumn]`.

### Shortcuts (Windows)
```
Copy        Paste       Cut
Undo        Select All  New Tab
Screenshot  Rename(F2)  Play/Pause
```


> OBS: Genvägarna är anpassade för Windows. På macOS/Linux behöver du troligen mappa om tangenterna (i Shortcuts::send).

## Encoder-kontroller

- Vrid medurs: Volym upp

- Vrid moturs: Volym ned

- Klick: Mute/Unmute

- Långtryck (~800 ms): Växla layer (Numbers ↔ Shortcuts)


## Firmwarestruktur
```
/src eller / (beroende på IDE)
│
├─ Config.h            → pinnar, matris, keymap, LED, EEPROM_SIZE
├─ HID_usages.h        → HID-usage IDs för volym ± (säkerställs i koden)
├─ State_machine.h/.cpp→ enum State { NUMS, SHORT_CUTS, APPLICATIONS }, set toggle/current
├─ Shortcuts.h/.cpp    → Windows-genvägar, samt Consumer Control Play/Pause
├─ Encoder.h/.cpp      → Volym via vridning, klick/dubbelklick/långtryck
└─ main .ino/.cpp      → init USB HID, skanna matris, ropa på rätt funktion per layer
```

- `USBHIDKeyboard` och `USBHIDConsumerControl` (TinyUSB) används för tangent- och mediaknappar.

- `EEPROM` är initierad; storlek 9 (kan användas senare för att spara t.ex. valt layer).

----------------------------------------------------------------------------------

## Bygga & flasha (Arduino IDE)

1. Installera ESP32-stöd i Boards Manager (Espressif Systems, v3.x rekommenderas).

2. Välj kort: ESP32S3 Dev Module (eller Waveshare ESP32-S3-Zero om du har den installerad).

3. Viktiga menyalternativ (Tools):

    - `USB CDC On Boot: Enabled`

    - `USB Mode: Default / TinyUSB`

    - `PSRAM: enligt ditt modul-utförande (ofta “OPI PSRAM” på S3 Zero)`

    - `Upload Speed: 921600 (eller 460800 om du har strul)`

4. Kablaget: Anslut S3 via USB-C direkt till datorn.

5. Kompilera & ladda upp. Efter flash ska en ny HID-enhet dyka upp (tangentbord + consumer control).

> Om porten inte syns: håll nere BOOT och tryck RESET, släpp RESET, släpp BOOT (sätter S3 i download-mode). Välj rätt COM-port och försök igen.