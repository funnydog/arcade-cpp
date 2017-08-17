#ifndef EVENTS_H
#define EVENTS_H

#include <SDL.h>

class Events
{
public:
	Events();

	enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE, QUIT, };

	void poll();

	bool checkInput(enum KEYS key) { return inputState[key]; }
	bool checkTrigger(enum KEYS key) { return triggerState[key]; }

	bool resize;
	bool quit;

private:
	void changeState(enum KEYS key, bool state);

	bool inputState[6];
	bool triggerState[6];
};

#endif
