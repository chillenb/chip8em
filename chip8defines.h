#ifndef CHIP8DEFINES_H
#define CHIP8DEFINES_H
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;

typedef int16_t i16;

enum Chip8ErrorCode {
  C8_INVALID_INSTRUCTION = 1,
  C8_STACK_EMPTY = 2,
  C8_STACK_OVERFLOW = 3,
  C8_GFX_OVERFLOW = 4,
  C8_PROGRAM_TOO_LARGE = 5
};


extern u8 chip8_fontset[80];

#endif