bool quit = false;
bool next_level = false;

#define NUM_CONTROLS 16

bool controls[NUM_CONTROLS];

#define CONTROL_LEFT controls[1]
#define CONTROL_RIGHT controls[2]
#define CONTROL_UP controls[3]
#define CONTROL_DOWN controls[4]

void process_input_events() {
  SDL_Event e;

  while (SDL_PollEvent(&e)){
    switch (e.type) {
    case SDL_QUIT:
      quit = true;
      break;

    case SDL_KEYDOWN:
      if (e.key.repeat == 0) {
	switch (e.key.keysym.sym) {
	case SDLK_ESCAPE:
	  quit = true;
	  break;

	case SDLK_TAB:
	  next_level = true;
	  break;

	case SDLK_LEFT:
	case SDLK_a:
	  CONTROL_LEFT = true;
	  break;

	case SDLK_RIGHT:
	case SDLK_d:
	  CONTROL_RIGHT = true;
	  break;

	case SDLK_UP:
	case SDLK_w:
	  CONTROL_UP = true;
	  break;

	case SDLK_DOWN:
	case SDLK_s:
	  CONTROL_DOWN = true;
	  break;
	}
      }
      break;

    case SDL_KEYUP:
      switch (e.key.keysym.sym) {
      case SDLK_LEFT:
      case SDLK_a:
	CONTROL_LEFT = false;
	break;

      case SDLK_RIGHT:
      case SDLK_d:
	CONTROL_RIGHT = false;
	break;

      case SDLK_UP:
      case SDLK_w:
	CONTROL_UP = false;
	break;

      case SDLK_DOWN:
      case SDLK_s:
	CONTROL_DOWN = false;
	break;
      }
      break;
    }
  }
}
