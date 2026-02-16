#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "time.h"
#include "math.h"
#include "stdbool.h"

#include "SDL.h"

#include "map.c"

#define TILE_SIZE 30

SDL_Window *sdl_win;
SDL_Renderer *sdl_ren;

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

void draw_map() {
  SDL_SetRenderDrawColor(sdl_ren, 0, 0, 0, 255);
  SDL_RenderClear(sdl_ren);

  for (int x = 0; x < MAP_WIDTH; x++) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
      int r, g, b;
      switch(map[x][y].material) {

#include "materials_inc/colors.c"

      }

      int X = x * TILE_SIZE;
      int Y = y * TILE_SIZE;
   
      SDL_Vertex verts[3] = {
        // Position {x, y}, Color {r, g, b, a}, TexCoord {u, v}
        {{X, Y}, {r, g, b, 255}, {0, 0}},
        {{X + TILE_SIZE, Y}, {r, g, b, 255}, {0, 0}},
        {{X + TILE_SIZE/2, Y+TILE_SIZE/2}, {r, g, b, 255}, {0, 0}}
      };
	
      char shape = map[x][y].shape;

      if (shape & TRI_NORTH) { SDL_RenderGeometry(sdl_ren, NULL, verts, 3, NULL, 0); }

      verts[0].position.x = X + TILE_SIZE; verts[0].position.y = Y + TILE_SIZE;

      if (shape & TRI_EAST) { SDL_RenderGeometry(sdl_ren, NULL, verts, 3, NULL, 0); }

      verts[1].position.x = X; verts[1].position.y = Y + TILE_SIZE;

      if (shape & TRI_SOUTH) { SDL_RenderGeometry(sdl_ren, NULL, verts, 3, NULL, 0); }

      verts[0].position.x = X; verts[0].position.y = Y;

      if (shape & TRI_WEST) { SDL_RenderGeometry(sdl_ren, NULL, verts, 3, NULL, 0); }
    }
  }
}

int main(int argc, char *argv[]) {
  setup_sdl_stuff();

  bool quit = 0;

  for (int i = 1; i < argc && !quit; i++) {
    load_map(argv[i]);

    bool next = 0;
    SDL_Event e;

    while (!next) {
      while (SDL_PollEvent(&e)) {
	if (e.type == SDL_QUIT) {
	  next = true;
	  quit = true;
	}
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB) {
	  next = true;
	}
      }

      draw_map();

      SDL_RenderPresent(sdl_ren);
    }
  }

  teardown_sdl_stuff();
}
