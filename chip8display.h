#ifndef CHIP8DISP_H
#define CHIP8DISP_H

#include <stdint.h>
#include "chip8defines.h"

#include <ncurses.h>

class Chip8Display {
    public:

    virtual void c8_updateDisplay(u64* gfx) = 0;
    virtual void c8_beep() = 0;
};


static const int c8_win_width = 64;
static const int c8_win_height = 32;


class Chip8TerminalDisplay : public Chip8Display {
    public:
    Chip8TerminalDisplay();
    ~Chip8TerminalDisplay();

    void c8_updateDisplay(u64* gfx);
    void c8_beep();

    private:

    WINDOW *win;
};

#endif