#include <SDL.h>
#include <SDL_image.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>

#include "globals.h"
#include "phi.h"
#include "menuview.h"

using namespace std;

const Uint64 targetFPS = 60;

// timeAfter() - check if time a is after time b
// @a: time a
// @b: time b
//
// Return: 1 if a > b, 0 otherwise
static int timeAfter(Uint64 a, Uint64 b)
{
	return (Sint64)b - (Sint64)a < 0;
}

int main(int argc, char *argv[])
{
	Phi phi;

	if (phi.createWindow("SDL_Example", SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
		return 1;

	if (phi.pushView(&MenuView::getInstance()) < 0) {
		phi.logError("Cannot initialize the View");
		return 1;
	}

	// game loop
	Uint64 frameTicks = SDL_GetPerformanceFrequency() / targetFPS;
	Uint64 nextTick = SDL_GetPerformanceCounter();
	while (!phi.shouldQuit()) {
		// update at constant frameTicks intervals
		while (!timeAfter(nextTick, SDL_GetPerformanceCounter())) {
			// process input
			phi.events.poll();

			// update the view
			if (phi.update() < 0) {
				phi.logError("Troubles in a state transition");
				goto out;
			}
			nextTick += frameTicks;
		}

		// render at variable intervals, skipping frames
		// with interpolation dt = [0.0:1.0[
		float dt = (float)(SDL_GetPerformanceCounter()  + frameTicks - nextTick) / frameTicks;
		phi.render(dt);
	}
out:
	return 0;
}
