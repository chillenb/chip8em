#include "chip8em.h"
#include "chip8defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <functional>

u8 chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


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

void Chip8::errhandler(Chip8ErrorCode errcode) {
  int num_err = static_cast<int>(errcode);
  switch(errcode) {
    case C8_INVALID_INSTRUCTION:
      fprintf(stderr, "Invalid instruction: %#04X\n", opcode);
      break;
    default:
      fprintf(stderr, "Unknown error %d!\n", num_err);
      break;
  }
  exit(1<<errcode);
}

void Chip8::cycle() {
  opcode = memory[pc] << 8 | memory[pc+1];
  u8 opcode_type = opcode >> 12;
  std::invoke(instructions[opcode_type], this);
  if(delay_timer > 0)
    delay_timer--;
  
  if(sound_timer > 0) {
    if(sound_timer == 1) {
      c8terminaldisplay.c8_beep();
      sound_timer--;
    }
  }

}

void Chip8::clear_screen() {
  return;
}


void Chip8::ins0() {
  switch(opcode) {
    case 0x00E0:
      clear_screen();
      break;
    case 0x00EE:
      // TODO:
      // return from subroutine
      break;
    default:
      errhandler(C8_INVALID_INSTRUCTION);
      break;
  }
}

void Chip8::ins1() {

}

void Chip8::ins2() {
  
}

void Chip8::ins3() {
  
}

void Chip8::ins4() {
  
}

void Chip8::ins5() {
  
}

void Chip8::ins6() {
  
}

void Chip8::ins7() {
  
}

void Chip8::insA() {
  
}

void Chip8::insD() {
  
}

void Chip8::c8_test_beep() {
  c8terminaldisplay.c8_beep();
}