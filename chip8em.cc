#include "chip8em.h"
#include "chip8defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <random>
#include <thread>
#include <chrono>
#include <bitset>
using std::bitset;

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
  std::mt19937 e1(c8_rd());
  c8_rng = e1;
  c8display.c8d_initialize();
}

void Chip8::load_program(u8 *code, int length) {
  if(length + 0x200 > 4096) {
    errhandler(C8_PROGRAM_TOO_LARGE);
  }
  for(int i = 0; i < length; i++) {
    memory[i+0x200] = code[i];
  }
}

void Chip8::run() {
  using namespace std::chrono_literals;
  int i = 0;
  while(!c8display.quit) {
    cycle();
    c8display.c8d_loop_tasks();
    if(!i)
      c8display.c8d_updateDisplay(gfx);
    std::this_thread::sleep_for(2ms);
    i = (i + 1) % 8;
  }
  c8display.c8d_cleanup();
}


void Chip8::errhandler(Chip8ErrorCode errcode) {
  int num_err = static_cast<int>(errcode);
  fprintf(stderr, "Address %#04X, opcode %#04X\n", pc, opcode);
  switch(errcode) {
    case C8_INVALID_INSTRUCTION:
      fprintf(stderr, "Invalid instruction: %#04X\n", opcode);
      break;
    case C8_PROGRAM_TOO_LARGE:
      fprintf(stderr, "Program size too large.\n");
      break;
    case C8_STACK_EMPTY:
      fprintf(stderr, "Tried to pop from empty stack\n");
      break;
    case C8_STACK_OVERFLOW:
      fprintf(stderr, "Stack overflow\n");
      break;
    case C8_GFX_OVERFLOW:
      fprintf(stderr, "Graphics overflow\n");
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
      c8display.c8d_beep();
      sound_timer--;
    }
  }

}

void Chip8::clear_screen() {
  for(auto& line : gfx)
    line = 0ULL;
  //c8display.c8d_updateDisplay(gfx);
  return;
}


void Chip8::ins0() {
  switch(opcode) {
    case 0x00E0:
      clear_screen();
      break;
    case 0x00EE:
      // return from subroutine
      if(sp==0) {
        errhandler(C8_STACK_EMPTY);
      } else {
        pc = stack[sp];
        sp--;
        pc += 2;
      }
      break;
    default:
      errhandler(C8_INVALID_INSTRUCTION);
      break;
  }
}

void Chip8::ins1() {
  u16 addr = opcode & 0x0FFF;
  pc = addr;
}

void Chip8::ins2() {
  if(sp==15)
    errhandler(C8_STACK_OVERFLOW);
  u16 addr = opcode & 0x0FFF;
  sp++;
  stack[sp] = pc;
  pc = addr;
}

void Chip8::ins3() {
  u8 byte = opcode & 0xFF;
  u8 reg = (opcode >> 8) & 0x0F;
  if(V[reg] == byte) {
    pc += 2;
  }
  pc += 2;
}

void Chip8::ins4() {
  u8 byte = opcode & 0xFF;
  u8 reg = (opcode >> 8) & 0x0F;
  if(V[reg] != byte) {
    pc += 2;
  }
  pc += 2;
}

void Chip8::ins5() {
  if(opcode & 0x0F)
    errhandler(C8_INVALID_INSTRUCTION);
  u8 regY = (opcode >> 4) & 0x0F;
  u8 regX = (opcode >> 8) & 0x0F;
  if(V[regY] == V[regX]) {
    pc += 2;
  }
  pc += 2;
}

void Chip8::ins6() {
  u8 byte = opcode & 0xFF;
  u8 reg = (opcode >> 8) & 0x0F;
  V[reg] = byte;
  pc += 2;
}

void Chip8::ins7() {
  u8 byte = opcode & 0xFF;
  u8 reg = (opcode >> 8) & 0x0F;
  V[reg] += byte;
  V[reg] &= 0xFF;
  pc += 2;
}

void Chip8::ins8() {
  u8 regY = (opcode >> 4) & 0x0F;
  u8 regX = (opcode >> 8) & 0x0F;
  i16 sum, diff;

  switch(opcode & 0x0F) {
    case 0:
      V[regX] = V[regY];
      break;
    case 1:
      V[regX] |= V[regY];
      break;
    case 2:
      V[regX] &= V[regY];
      break;
    case 3:
      V[regX] ^= V[regY];
      break;
    case 4:
      sum = V[regX] + V[regY];
      V[regX] = sum & 0xFF;
      if(sum > 0xFF)
        V[0xF] = 1;
      else
        V[0xF] = 0;
      break;
    case 5:
      diff = V[regX] - V[regY];
      V[regX] = diff & 0xFF;
      if(diff < 0)
        V[0xF] = 0;
      else
        V[0xF] = 1;
      break;
    case 6:
      V[0xF] = V[regX] & 0x01;
      V[regX] >>= 1;
      break;
    case 7:
      diff = V[regY] - V[regX];
      V[regX] = diff & 0xFF;
      if(diff<0)
        V[0xF] = 0;
      else
        V[0xF] = 1;
      break;
    case 0xE:
      V[0xF] = (V[regX] >> 7) & 0x01;
      V[regX] <<= 1;
      break;
    default:
      errhandler(C8_INVALID_INSTRUCTION);
      break;
  }
  pc += 2;
}

void Chip8::ins9() {
  if(opcode & 0x0F)
    errhandler(C8_INVALID_INSTRUCTION);
  u8 regY = (opcode >> 4) & 0x0F;
  u8 regX = (opcode >> 8) & 0x0F;
  if(V[regY] != V[regX]) {
    pc += 2;
  }
  pc += 2;
}

void Chip8::insA() {
  I = opcode & 0xFFF;
  pc += 2;
}

void Chip8::insB() {
  pc = V[0] + (opcode & 0xFFF);
}

void Chip8::insC() {
  u8 regX = (opcode >> 8) & 0x0F;
  u8 byte = opcode & 0xFF;
  u8 random = c8_unif_dist(c8_rng);
  V[regX] = random & byte;
  pc += 2;
}

void Chip8::insD() {
  u8 regY = (opcode >> 4) & 0x0F;
  u8 regX = (opcode >> 8) & 0x0F;
  int coordx = V[regX];
  int coordy = V[regY];
  u8 height = opcode & 0x0F;
  V[0xF] = 0;
  for(int row = 0; row < height; row++) {
    u8 pixels = memory[(I+row)&0x0FFF];
    bitset<8> bpixels(pixels);
    int row_to_use = (row + coordy) & 63;
    for(int col = 0; col < 8; col++) {
      int col_to_use = (col + coordx) & 63;
      bool existing = gfx[row_to_use].test(col_to_use);
      if(existing && bpixels.test(7-col))
        V[0xF] = 1;
      gfx[row_to_use][col_to_use] = existing != bpixels.test(7-col);
    }
  }
  //c8display.c8d_updateDisplay(gfx);
  pc += 2;
}

void Chip8::insE() {
  u8 regX = (opcode >> 8) & 0x0F;
  switch(opcode & 0xFF) {
    case 0x9E:
      if(c8display.c8d_check_if_key(V[regX]))
        pc += 2;
      break;
    case 0xA1:
      if(!c8display.c8d_check_if_key(V[regX]))
        pc += 2;
      break;
    default:
      errhandler(C8_INVALID_INSTRUCTION);
      break;
  }
  pc += 2;
}

void Chip8::insF() {
  u8 regX = (opcode >> 8) & 0x0F;
  u8 code = opcode & 0xFF;
  u8 div, byte;
  switch(code) {
    case 0x7:
      V[regX] = delay_timer & 0xFF;
      break;
    case 0xA:
      V[regX] = c8display.c8d_getchar();
      break;
    case 0x15:
      delay_timer = V[regX];
      break;
    case 0x18:
      sound_timer = V[regX];
      break;
    case 0x1E:
      I += V[regX];
      break;
    case 0x29:
      I = V[regX] * 5;
      break;
    case 0x33:
      byte = V[regX];
      div = byte / 10;
      memory[(I+2)&0x0FFF] = byte - div*10;
      byte = div / 10;
      memory[(I+1)&0x0FFF] = div - byte * 10;
      memory[I] = byte;
      break;
    case 0x55:
      for(int j = 0; j <= regX; j++) {
        memory[(I+j)&0x0FFF] = V[j];
      }
      break;
    case 0x65:
      for(int j = 0; j <= regX; j++) {
        V[j] = memory[(I+j)&0x0FFF];
      }
      break;
    default:
      errhandler(C8_INVALID_INSTRUCTION);
      break;
  }
  pc += 2;
}


void Chip8::c8_test_beep() {
  c8display.c8d_beep();
}