#ifndef CHIP8EM_H
#define CHIP8EM_H

#include "chip8defines.h"
#include "chip8display.h"
#include <array>
#include <bitset>
#include <random>
#include <stdint.h>

using std::array;
using std::bitset;

class Chip8 {
  typedef void (Chip8::*Chip8Instruction)();

public:
  void initialize();
  void load_program(u8 *code, int length);
  void run();
  void cycle();
  void errhandler(Chip8ErrorCode);
  void clear_screen();
  void c8_test_beep();

  // private:
  array<u8, 4096> memory = {0};
  array<i16, 16> V = {0};   // register file
  u16 I;                    // index register
  u16 pc;                   // program counter
  u16 sp;                   // stack pointer
  bitset<64> gfx[32] = {0}; // screen, 64 x 32
  u16 opcode;
  u8 delay_timer, sound_timer;
  array<u16, 16> stack; // stack

  std::random_device c8_rd;
  std::mt19937 c8_rng;
  std::uniform_int_distribution<> c8_unif_dist{0, 0xFF};

  void ins0();
  void ins1();
  void ins2();
  void ins3();
  void ins4();
  void ins5();
  void ins6();
  void ins7();
  void ins8();
  void ins9();
  void insA();
  void insB();
  void insC();
  void insD();
  void insE();
  void insF();

  Chip8Instruction instructions[16] = {
      &Chip8::ins0, // 0
      &Chip8::ins1, // 1
      &Chip8::ins2, // 2
      &Chip8::ins3, // 3
      &Chip8::ins4, // 4
      &Chip8::ins5, // 5
      &Chip8::ins6, // 6
      &Chip8::ins7, // 7
      &Chip8::ins8, // 8
      &Chip8::ins9, // 9
      &Chip8::insA, // A
      &Chip8::insB, // B
      &Chip8::insC, // C
      &Chip8::insD, // D
      &Chip8::insE, // E
      &Chip8::insF  // F
  };

  Chip8SDL c8display;
};

#endif