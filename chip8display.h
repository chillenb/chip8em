#ifndef CHIP8DISP_H
#define CHIP8DISP_H

#include <stdint.h>
#include "chip8defines.h"
#include <array>
#include <ncurses.h>
#include <bitset>

using std::bitset;
using std::array;

class Chip8Display {
    public:

    virtual void c8d_initialize() = 0;
    virtual void c8d_updateDisplay(bitset<64> gfx[]) = 0;
    virtual void c8d_beep() = 0;
    virtual u8 c8d_getchar() = 0;

};


static const int c8d_win_width = 64;
static const int c8d_win_height = 32;


class Chip8TerminalDisplay : public Chip8Display {
    public:
    Chip8TerminalDisplay();
    ~Chip8TerminalDisplay();

    bool isinitialized = false;

    void c8d_initialize();
    void c8d_updateDisplay(bitset<64> gfx[]);
    void c8d_beep();
    u8 c8d_getchar();


    private:

    WINDOW *win;
};

#endif