A simple door lock using keypad and LCD screen.

this project consists of 5 drivers and 2 ECUs.
ECUS:
  First ECU: keypad and LCD are attached to it.
  Second ECU: EEPROM and motor are attached to it.
Drivers :
  1) i2c to store password at EEPROM.
  2) UART to commuincate between two ECUs.
  3) motor driver to control door.
  4) keypad driver to transform key strokes to numbers.
  5) LCD to display messages on screen.
