#include <ncurses.h>
#include <unistd.h>
#include "chip8defines.h"
#include "chip8display.h"
#include <wchar.h>
#include <locale.h>



Chip8TerminalDisplay::Chip8TerminalDisplay() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    int startx = (COLS - c8_win_width*2) / 2;
    int starty = (LINES - c8_win_height) / 2;
    
    startx = (startx < 0) ? 0 : startx;
    starty = (starty < 0) ? 0 : starty;

    win = newwin(c8_win_height, c8_win_width*2, starty, startx);
    getch();
}


Chip8TerminalDisplay::~Chip8TerminalDisplay() {
    getch();
    endwin();
}

void Chip8TerminalDisplay::c8_updateDisplay(u64* gfx) {
    werase(win);
    wchar_t linebuf[2*c8_win_width+1];
    linebuf[2*c8_win_width] = L'\0';
    for(int i = 0; i < c8_win_height; i++) {
        u64 line = gfx[i];
        for(int j = 0; j < c8_win_width; j++) {
            if(line & (1UL<<j)) {
                linebuf[2*j] = L'█';
                linebuf[2*j+1] = L'█';
            } else {
                linebuf[2*j] = L' ';
                linebuf[2*j+1] = L' ';
            }
        }
        mvwaddwstr(win, i, 0, linebuf);
    }
    wrefresh(win);
}

void Chip8TerminalDisplay::c8_beep() {
    beep();
}