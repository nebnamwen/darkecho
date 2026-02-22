#define TILE_SIZE 30

void draw_circle(int x, int y, int radius, int r, int g, int b) {
  for (int dx = -1; dx <= 1; dx += 2) {
    for (int dy = -1; dy <= 1; dy += 2) {
      SDL_Vertex verts[3] = {
        // Position {x, y}, Color {r, g, b, a}, TexCoord {u, v}                                     
        {{x, y}, {r, g, b, 255}, {0, 0}},
        {{x + dx * radius, y}, {r, g, b, 255}, {0, 0}},
        {{x + dx * (int)(radius / sqrt(2)), y + dy * (int)(radius / sqrt(2))}, {r, g, b, 255}, {0, 0}}
      };
      SDL_RenderGeometry(sdl_ren, NULL, verts, 3, NULL, 0);

      verts[1].position.x = x; verts[1].position.y = y + dy * radius;
      SDL_RenderGeometry(sdl_ren, NULL, verts, 3, NULL, 0);
    }
  }
}

void draw_map() {
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

void draw_player() {
  SDL_SetRenderDrawColor(sdl_ren, 255, 255, 255, 255);
  int x = (int)(player_pos.x * TILE_SIZE);
  int y = (int)(player_pos.y * TILE_SIZE);
  int r = (int)(PLAYER_RADIUS * TILE_SIZE);
  draw_circle(x, y, r, 255, 255, 255);
  SDL_RenderDrawLine(sdl_ren, x, y, x + (int)(2 * r * cos(player_dir)), y + (int)(2 * r * sin(player_dir)));
}

void draw_all() {
  SDL_SetRenderDrawColor(sdl_ren, 0, 0, 0, 255);
  SDL_RenderClear(sdl_ren);

  draw_map();
  draw_player();

  SDL_RenderPresent(sdl_ren);
}
