#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "stdbool.h"

#include "SDL.h"

#define SAMPLES_PER_SECOND 44100
uint32_t RunningSampleIndex = 0;

SDL_Window *sdl_win;
SDL_Renderer *sdl_ren;

#include "audio.c"
#include "control.c"
#include "level.c"
#include "map_file.c"
#include "graphics.c"

void setup_sdl_stuff() {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  sdl_win = SDL_CreateWindow("Dark Echo map test", 0, 0, TILE_SIZE * MAP_WIDTH, TILE_SIZE * MAP_HEIGHT, 0);
  sdl_ren = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);

  init_audio();
}

void teardown_sdl_stuff() {
  SDL_CloseAudio();
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
      int dT = queue_audio();
      update(dT);
      draw_all();

      SDL_Delay(500 * SamplesPerFrame / SAMPLES_PER_SECOND);
    }
  }

  teardown_sdl_stuff();
}
