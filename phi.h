#ifndef PHI_H
#define PHI_H

#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>

#include "events.h"
#include "sprite.h"

class View;

class Phi
{
public:
	Phi();
	~Phi();

	int createWindow(const std::string &title, int width, int height);

	int getSize(int *w, int *h);

	SDL_Texture *loadTexture(const char *path);
	SDL_Texture *loadTexture(const char *path, SDL_Color colorKey);
	SDL_Texture *strToTexture(const std::string &text, TTF_Font *font, SDL_Color color);

	TTF_Font *loadFont(const char *path, int size);

	void renderSprite(const Sprite *sprite, int x, int y);
	void renderText(const std::string &msg, int x, int y, TTF_Font *font, SDL_Color color);
	SDL_Renderer *getRenderer() { return renderer; }

	void playSound(Mix_Chunk *chunk);
	void playMusic(Mix_Music *music);

	Events events;

	// view management
	int changeView(View *view);
	int pushView(View *view);
	void popView();

	int update();
	void render(float dt);

	bool shouldQuit();
	void quit();

	void logInfo(const std::string &msg);
	void logWarning(const std::string &msg);
	void logError(const std::string &msg);

private:
	int initLibs();
	void quitLibs();

	SDL_Renderer *renderer;
	SDL_Window *window;

	std::vector<View *> views;
	View *curView;
	bool exitLoop;

	int soundChannels;
};

#endif
