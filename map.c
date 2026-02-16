// all map files must be exactly this size for now
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

#define MAT_NOTHING 0
#define MAT_STONE 1
#define MAT_CRYSTAL 2

typedef struct {
  char shape;
  char material;
} map_tile_t;

map_tile_t map[MAP_WIDTH][MAP_HEIGHT];

void load_map(const char* file) {
#define LINE_LEN MAP_WIDTH*3 + 5
  char line[LINE_LEN] = { 0 };

  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s: Unable to read map file\n", file);
    exit(1);
  }

  int y = 0;
  while (!feof(fp)) {
    memset(line, 0, LINE_LEN);
    fgets(line, LINE_LEN, fp);

    if (y < MAP_HEIGHT) {
      if (strlen(line) <= MAP_WIDTH * 3) {
	fprintf(stderr, "%s %d: Map line too short\n", file, y);
	exit(1);
      }

      for (int x = 0; x < MAP_WIDTH; x++) {

#define TILE_ERROR { fprintf(stderr, "%s %d %d: Unable to interpret tile '%s'\n", file, y, x, tile); exit(1); }

	char tile[4] = { 0 };
	char matkey = ' ';
	char material = MAT_NOTHING;
	char shape = SHAPE_EMPTY;
	strncpy(tile, line+(x*3), 3);

	// empty tile
	if (strcmp(tile, "   ") == 0) { }

	// full tile
	else if (tile[0] == '[' && tile[2] == ']') {
	  matkey = tile[1];
	  shape = SHAPE_FULL;
	}

	// slanted tile
	else if (tile[1] == '/' || tile[1] == '\\') {
	  if (tile[0] == ' ') {
	    if (tile[2] == ' ') { TILE_ERROR; }

	    matkey = tile[2];
	    if (tile[1] == '/') { shape = SHAPE_SE; }
	    else { shape = SHAPE_NE; }
	  }
	  else if (tile[2] == ' ') {
	    matkey = tile[0];
	    if (tile[1] == '/') { shape = SHAPE_NW; }
	    else { shape = SHAPE_SW; }
	  }
	  else { TILE_ERROR; }
	}

	else { TILE_ERROR; }

	switch (matkey) {
	case ' ':
	  material = MAT_NOTHING;
	  break;

	case 's':
	  material = MAT_STONE;
	  break;

	case 'c':
	  material = MAT_CRYSTAL;
	  break;

	default:
	  TILE_ERROR;
	}

	map[x][y].shape = shape;
	map[x][y].material = material;
      }

      y++;
    }
    else {
      if (strlen(line)) {
	fprintf(stderr, "%s %d: More map lines than expected\n", file, y);
	exit(1);
      }
    }
  }

  if (y < MAP_HEIGHT) {
    fprintf(stderr, "%s %d: Fewer map lines than expected\n", file, y);
    exit(1);
  }

  fclose(fp);
}
