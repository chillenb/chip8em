#include <ncurses.h>
#include <unistd.h>
#include "chip8defines.h"
#include "chip8display.h"
#include <wchar.h>
#include <locale.h>
#include <array>
#include <bitset>
using std::bitset;
using std::array;



Chip8TerminalDisplay::Chip8TerminalDisplay() {

}


Chip8TerminalDisplay::~Chip8TerminalDisplay() {
    if(isinitialized){
        getch();
        endwin();
    }
}

void Chip8TerminalDisplay::c8d_updateDisplay(bitset<64> gfx[]) {
    werase(win);
    wchar_t linebuf[2*c8d_win_width+1];
    linebuf[2*c8d_win_width] = L'\0';
    for(int i = 0; i < c8d_win_height; i++) {
        bitset<64> line = gfx[i];
        for(int j = 0; j < c8d_win_width; j++) {
            if(line.test(j)) {
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

void Chip8TerminalDisplay::c8d_beep() {
    beep();
}

void Chip8TerminalDisplay::c8d_initialize() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    nodelay(win, TRUE);
    curs_set(0);
    int startx = (COLS - c8d_win_width*2) / 2;
    int starty = (LINES - c8d_win_height) / 2;
    
    startx = (startx < 0) ? 0 : startx;
    starty = (starty < 0) ? 0 : starty;

    win = newwin(c8d_win_height, c8d_win_width*2, starty, startx);
}

int c8_key_from_keyboard(int c) {
    switch(c) {
        case '1':
          return 1;
        case '2':
          return 2;
        case '3':
          return 3;
        case '4':
          return 0xC;
        case 'q':
          return 0x4;
        case 'w':
          return 0x5;
        case 'e':
          return 0x6;
        case 'r':
          return 0xD;
        case 'a':
          return 0x7;
        case 's':
          return 0x8;
        case 'd':
          return 0x9;
        case 'f':
          return 0xE;
        case 'z':
          return 0xA;
        case 'x':
          return 0x0;
        case 'c':
          return 0xB;
        case 'v':
          return 0xF;
        default:
          return -1;
    }
}


u8 Chip8TerminalDisplay::c8d_getchar() {
    int c = c8_key_from_keyboard(getch());
    while(c == -1) {
        c = c8_key_from_keyboard(getch());
    }
    fprintf(stderr, "Got character %d\n", c);
    return u8(c);
}