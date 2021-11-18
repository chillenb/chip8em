#include <iostream>
#include "chip8em.h"

int main() {
  Chip8 myc8;
  myc8.c8_test_beep();
  for(int i = 0; i < 32; i++) {
  myc8.gfx[i] = (i&0x01) ? 0x5555555555555555 : 0xAAAAAAAAAAAAAAAA;
  }
  myc8.c8terminaldisplay.c8_updateDisplay(myc8.gfx);
  return 0;
}