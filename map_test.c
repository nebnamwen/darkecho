#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "stdbool.h"

#include "SDL.h"

#include "control.c"
#include "level.c"
#include "map_file.c"

#define FRAME_MS 25

SDL_Window *sdl_win;
SDL_Renderer *sdl_ren;

#include "graphics.c"

void setup_sdl_stuff() {
  SDL_Init(SDL_INIT_VIDEO);

  sdl_win = SDL_CreateWindow("Dark Echo map test", 0, 0, TILE_SIZE * MAP_WIDTH, TILE_SIZE * MAP_HEIGHT, 0);
  sdl_ren = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);
}

void teardown_sdl_stuff() {
  SDL_DestroyRenderer(sdl_ren);
  SDL_DestroyWindow(sdl_win);
  SDL_Quit();
}

int main(int argc, char *argv[]) {
  setup_sdl_stuff();

  for (int i = 1; i < argc && !quit; i++) {
    load_map(argv[i]);

    next_level = false;
    SDL_Event e;

    while (!next_level && !quit) {
      process_input_events();
      update(FRAME_MS);
      draw_all();

      SDL_Delay(FRAME_MS);
    }
  }

  teardown_sdl_stuff();
}
