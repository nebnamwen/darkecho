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

void move_player(int ticks) {
  float ds = MOVE_RATE * ticks / TICKS_PER_SEC;
  if (ds > PLAYER_RADIUS * 0.5) { ds = PLAYER_RADIUS; }

  if (CONTROL_LEFT) {
    player_dir -= TURN_RATE * ticks/ TICKS_PER_SEC;
    if (player_dir < 0) { player_dir += M_PI * 2; }
  }

  if (CONTROL_RIGHT) {
    player_dir += TURN_RATE * ticks / TICKS_PER_SEC;
    if (player_dir > M_PI * 2) { player_dir -= M_PI * 2; }
  }

  if (CONTROL_UP) {
    player_pos.x += cos(player_dir) * ds;
    player_pos.y += sin(player_dir) * ds;
  }

  if (CONTROL_DOWN) {
    player_pos.x -= cos(player_dir) * ds;
    player_pos.y -= sin(player_dir) * ds;
  }
}

void naive_bounds_check() {
  if (player_pos.x < 0.5) { player_pos.x = 0.5; }
  if (player_pos.y < 0.5) { player_pos.y = 0.5; }
  if (player_pos.x > MAP_WIDTH - 0.5) { player_pos.x = MAP_WIDTH - 0.5; }
  if (player_pos.y > MAP_HEIGHT - 0.5) { player_pos.y = MAP_HEIGHT - 0.5; }
}

typedef struct {
  float x;
  float y;
  float d;
} collision_t;

collision_t collide_point(float x, float y) {
  collision_t collision = { 0, 0, 0 };
  float r = sqrt((player_pos.x - x)*(player_pos.x - x) + (player_pos.y - y)*(player_pos.y - y));
  if (r > 0 && r < PLAYER_RADIUS) {
    collision.d = PLAYER_RADIUS - r;
    collision.x = (player_pos.x - x) * collision.d / r;
    collision.y = (player_pos.y - y) * collision.d / r;
  }
  return collision;
}

collision_t collide_line(float dx, float dy, float x1, float y1, float x2, float y2) {
  collision_t collision = { 0, 0, 0 };
  if (dx && dy) { dx = dx / sqrt(2); dy = dy / sqrt(2); }
  float r = (player_pos.x - x1) * dx + (player_pos.y - y1) * dy;
  if (r > 0 && r < PLAYER_RADIUS &&
      // check that player position is between the ends of this wall segment
      ((player_pos.x - x1) * dy - (player_pos.y - y1) * dx) *
      ((player_pos.x - x2) * dy - (player_pos.y - y2) * dx) < 0) {
    collision.d = PLAYER_RADIUS - r;
    collision.x = dx * collision.d;
    collision.y = dy * collision.d;
  }
  return collision;
}

#define TEST_COLLIDE if (next_collision.d > best_collision.d) { best_collision = next_collision; }

bool collide_walls() {
  bool collided = false;

  collision_t no_collision = { 0, 0, 0 };
  collision_t best_collision;
  collision_t next_collision;

  do {
    best_collision = next_collision = no_collision;
  
    // iterate over cells player overlaps
    for (int X = (int)(player_pos.x - PLAYER_RADIUS);
	 X <= (int)(player_pos.x + PLAYER_RADIUS);
	 X++) { 
      for (int Y = (int)(player_pos.y - PLAYER_RADIUS);
	   Y <= (int)(player_pos.y + PLAYER_RADIUS);
	   Y++) {
	char shape = map[X][Y].shape;

	// collide with vertices in this cell
	if (shape & (TRI_NORTH | TRI_WEST)) { next_collision = collide_point(X,Y); TEST_COLLIDE; }
	if (shape & (TRI_NORTH | TRI_EAST)) { next_collision = collide_point(X+1,Y); TEST_COLLIDE; }
	if (shape & (TRI_SOUTH | TRI_WEST)) { next_collision = collide_point(X,Y+1); TEST_COLLIDE; }
	if (shape & (TRI_SOUTH | TRI_EAST)) { next_collision = collide_point(X+1,Y+1); TEST_COLLIDE; }

	// collide with orthogonal edges in this cell
       	if (shape & TRI_NORTH) { next_collision = collide_line(0,-1,X,Y,X+1,Y); TEST_COLLIDE; }
	if (shape & TRI_SOUTH) { next_collision = collide_line(0,1,X,Y+1,X+1,Y+1); TEST_COLLIDE; }
	if (shape & TRI_WEST) { next_collision = collide_line(-1,0,X,Y,X,Y+1); TEST_COLLIDE; }
	if (shape & TRI_EAST) { next_collision = collide_line(1,0,X+1,Y,X+1,Y+1); TEST_COLLIDE; }
	
	// collide with diagonal edges in this cell
	if (shape == SHAPE_NW) { next_collision = collide_line(1,1,X+1,Y,X,Y+1); TEST_COLLIDE; }
	if (shape == SHAPE_NE) { next_collision = collide_line(-1,1,X,Y,X+1,Y+1); TEST_COLLIDE; }
	if (shape == SHAPE_SE) { next_collision = collide_line(-1,-1,X+1,Y,X,Y+1); TEST_COLLIDE; }
	if (shape == SHAPE_SW) { next_collision = collide_line(1,-1,X,Y,X+1,Y+1); TEST_COLLIDE; }
      }
    }

    player_pos.x += best_collision.x;
    player_pos.y += best_collision.y;
  } while ((best_collision.d > 0.001) && (collided = true));

  return collided;
}

void update(int ticks) {
  move_player(ticks);
  naive_bounds_check();
  collide_walls();
}

