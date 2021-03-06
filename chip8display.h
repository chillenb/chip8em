#ifndef CHIP8DISP_H
#define CHIP8DISP_H

#include "chip8defines.h"
#include <SDL2/SDL.h>
#include <array>
#include <bitset>
#include <stdint.h>

using std::array;
using std::bitset;

class Chip8Display {
public:
  bool isinitialized = false;
  bool quit = false;

  virtual void c8d_initialize() = 0;
  virtual void c8d_updateDisplay(bitset<64> gfx[]) = 0;
  virtual void c8d_beep() = 0;
  virtual u8 c8d_getchar() = 0;
};

static constexpr int c8d_win_width = 64;
static constexpr int c8d_win_height = 32;
static constexpr int pixsize = 10;
static constexpr int margin = 20;

static constexpr int sc_width = c8d_win_width * pixsize + 2 * margin;
static constexpr int sc_height = c8d_win_height * pixsize + 2 * margin;

/*
class Chip8TerminalDisplay : public Chip8Display {
    public:
    Chip8TerminalDisplay();
    ~Chip8TerminalDisplay();

    void c8d_initialize();
    void c8d_updateDisplay(bitset<64> gfx[]);
    void c8d_beep();
    u8 c8d_getchar();


    private:

    WINDOW *win;
};
*/

class Chip8SDL : public Chip8Display {
public:
  Chip8SDL();
  ~Chip8SDL();

  void c8d_initialize();
  void c8d_updateDisplay(bitset<64> gfx[]);
  void c8d_beep();
  bool c8d_check_if_key(u8 key);
  u8 c8d_getchar();
  void c8d_cleanup();

  void c8d_loop_tasks();

private:
  float screen[64 * 32] = {0};
  u8 screentimer[64 * 32] = {0};

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Event e;
  const u8 *keystate;
};

#endif