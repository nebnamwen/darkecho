// all maps must be exactly this size for now
#define MAP_HEIGHT 20
#define MAP_WIDTH 20

#define TRI_NORTH 1
#define TRI_EAST 2
#define TRI_SOUTH 4
#define TRI_WEST 8

#define SHAPE_EMPTY 0
#define SHAPE_FULL (TRI_NORTH | TRI_EAST | TRI_SOUTH | TRI_WEST)
#define SHAPE_NW (TRI_NORTH | TRI_WEST)
#define SHAPE_NE (TRI_NORTH | TRI_EAST)
#define SHAPE_SE (TRI_SOUTH | TRI_EAST)
#define SHAPE_SW (TRI_SOUTH | TRI_WEST)

#include "materials_inc/defines.c"

typedef struct {
  float x;
  float y;
} vec2_t;

vec2_t player_pos = { -1, -1 };
float player_dir = 0;
#define PLAYER_RADIUS sqrt(2) / 4

typedef struct {
  char shape;
  char material;
} map_tile_t;

map_tile_t map[MAP_WIDTH][MAP_HEIGHT];

#define TICKS_PER_SEC 1000

#define TURN_RATE 1.0
#define MOVE_RATE 1.0

void update(int ticks) {
  if (CONTROL_LEFT) {
    player_dir -= TURN_RATE * ticks / TICKS_PER_SEC;
    if (player_dir < 0) { player_dir += M_PI * 2; }
  }

  if (CONTROL_RIGHT) {
    player_dir += TURN_RATE * ticks / TICKS_PER_SEC;
    if (player_dir > M_PI * 2) { player_dir -= M_PI * 2; }
  }

  if (CONTROL_UP) {
    player_pos.x += cos(player_dir) * MOVE_RATE * ticks / TICKS_PER_SEC;
    player_pos.y += sin(player_dir) * MOVE_RATE * ticks / TICKS_PER_SEC;
  }

  if (CONTROL_DOWN) {
    player_pos.x -= cos(player_dir) * MOVE_RATE * ticks / TICKS_PER_SEC;
    player_pos.y -= sin(player_dir) * MOVE_RATE * ticks / TICKS_PER_SEC;
  }
}
