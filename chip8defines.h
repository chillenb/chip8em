#ifndef CHIP8DEFINES_H
#define CHIP8DEFINES_H
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;

enum Chip8ErrorCode {
  C8_INVALID_INSTRUCTION = 1
};


extern u8 chip8_fontset[80];

#endif