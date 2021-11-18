#ifndef CHIP8EM_H
#define CHIP8EM_H

#include <stdint.h>
#include "chip8defines.h"
#include "chip8display.h"



class Chip8 {
  typedef void (Chip8::*Chip8Instruction)();
  public:
  void initialize();
  void cycle();
  void errhandler(Chip8ErrorCode);
  void clear_screen();
  void c8_test_beep();

  //private:
  u8 memory[4096] = {0};
  u8 V[16]; // register file
  u16 I; // index register
  u16 pc; // program counter
  u16 sp; // stack pointer
  u64 gfx[32] = {0}; // screen, 64 x 32
  u16 opcode;
  u8 delay_timer, sound_timer;
  u16 stack[16]; // stack

  void ins0();
  void ins1();
  void ins2();
  void ins3();
  void ins4();
  void ins5();
  void ins6();

  void ins7();
  void insA();
  void insD();


  Chip8Instruction instructions[16] = {
    &Chip8::ins0, // 0
    &Chip8::ins1, // 1
    &Chip8::ins2, // 2
    &Chip8::ins3, // 3
    &Chip8::ins4, // 4
    &Chip8::ins5, // 5
    &Chip8::ins6, // 6
    &Chip8::ins7, // 7
    nullptr, // 8
    nullptr, // 9
    &Chip8::insA, // A
    nullptr, // B
    nullptr, // C
    &Chip8::insD, // D
    nullptr, // E
    nullptr  // F
  };

  Chip8TerminalDisplay c8terminaldisplay;

};

#endif