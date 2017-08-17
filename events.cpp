#include "events.h"

Events::Events() :
	resize(false),
	quit(false),
	inputState{false, false, false, false, false, false},
	triggerState{false, false, false, false, false, false}
{
}

void
Events::changeState(enum KEYS key, bool state)
{
	triggerState[key] = !inputState[key] && state;
	inputState[key] = state;
}

void
Events::poll()
{
	// reset the trigger events
	for (auto &v : triggerState)
		v = false;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_WINDOWEVENT) {
			resize = true;
		} else if (event.type == SDL_QUIT) {
			quit = true;
		} else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			bool state = event.type == SDL_KEYDOWN;
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				changeState(QUIT, state);
				break;
			case SDLK_LEFT:
				changeState(LEFT, state);
				break;
			case SDLK_RIGHT:
				changeState(RIGHT, state);
				break;
			case SDLK_UP:
				changeState(UP, state);
				break;
			case SDLK_DOWN:
				changeState(DOWN, state);
				break;
			case SDLK_SPACE:
				changeState(SPACE, state);
				break;
			default:
				break;
			}
		}
	}
}
