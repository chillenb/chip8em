#include "chip8em.h"

void Chip8::initialize() {
  pc = 0x200;
  I = 0;
  sp = 0;
  delay_timer = 0;
  sound_timer = 0;

  for(int i = 0; i < 80; i++) {
    memory[i] = chip8_fontset[i];
  }
}


void Chip8::cycle() {
  
}