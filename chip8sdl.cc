
#include "chip8defines.h"
#include "chip8display.h"

#include <array>
#include <bitset>
using std::bitset;
using std::array;



Chip8SDL::Chip8SDL() {

}


Chip8SDL::~Chip8SDL() {

}

void Chip8SDL::c8d_loop_tasks() {
  while(SDL_PollEvent(&e) != 0) {
    if(e.type == SDL_QUIT)
      quit = true;
  }
  keystate = reinterpret_cast<const u8*>(SDL_GetKeyboardState(NULL));
}

void Chip8SDL::c8d_updateDisplay(bitset<64> gfx[]) {
    const u8 delay = 0;
    SDL_Rect rect;
    rect.w = pixsize;
    rect.h = pixsize;
    for(int i = 0; i < c8d_win_height; i++) {
        bitset<64> line = gfx[i];
        for(int j = 0; j < c8d_win_width; j++) {
          if(line.test(j)) {
            screentimer[i*64+j] = delay;
            screen[i*64 + j] = 2.5f;
          } else {
            if(screentimer[i*64+j])
              screentimer[i*64+j]--;
            else {
              screen[i*64 + j] = 0.65 * screen[i*64+j];
            }
          }

          u8 rcolor = std::min(255.0f*screen[i*64+j], 255.0f);
          SDL_SetRenderDrawColor(renderer, rcolor, rcolor, rcolor, 0xFF);

          rect.y = margin + i * pixsize;
          rect.x = margin + j * pixsize;
          SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

void Chip8SDL::c8d_beep() {
    //beep();
}

void Chip8SDL::c8d_initialize() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL failed to initialize: error %s\n", SDL_GetError());
    exit(-1);
  }
  window = SDL_CreateWindow("chip8em",
           SDL_WINDOWPOS_UNDEFINED,
           SDL_WINDOWPOS_UNDEFINED,
           sc_width,
           sc_height,
           SDL_WINDOW_SHOWN);
  if(window == NULL) {
    fprintf(stderr, "Could not create window: error %s\n", SDL_GetError());
    SDL_Quit();
    exit(-1);
  }
  //surface = SDL_GetWindowSurface(window);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == NULL) {
    fprintf(stderr, "Could not create renderer: error %s\n", SDL_GetError());
  }
}

void Chip8SDL::c8d_cleanup() {
  if(isinitialized){
    SDL_DestroyWindow(window);
    SDL_Quit();
  }
}


static const SDL_Scancode keys[16] = {
  SDL_SCANCODE_X,
  SDL_SCANCODE_1,
  SDL_SCANCODE_2,
  SDL_SCANCODE_3,
  SDL_SCANCODE_Q,
  SDL_SCANCODE_W,
  SDL_SCANCODE_E,
  SDL_SCANCODE_A,
  SDL_SCANCODE_S,
  SDL_SCANCODE_D,
  SDL_SCANCODE_Z,
  SDL_SCANCODE_C,
  SDL_SCANCODE_4,
  SDL_SCANCODE_R,
  SDL_SCANCODE_F,
  SDL_SCANCODE_V
};

bool Chip8SDL::c8d_check_if_key(u8 key) {
  return keystate[keys[key]];
}

u8 Chip8SDL::c8d_getchar() {
  while(!quit) {
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT) {
        quit = true;
      } else if(e.type == SDL_KEYDOWN) {
        SDL_Scancode sc = e.key.keysym.scancode;
        for(int k = 0; k < 16; k++) {
          if(keys[k] == sc) {
            return k;
          }
        }
      }
    }
  }
  return 0;
}