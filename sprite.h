#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

class Sprite
{
private:
	SDL_Texture *texture;
	SDL_Rect srcRect;
	int originX;
	int originY;

public:
	Sprite();

	void init(SDL_Texture *texture, int x = 0, int y = 0, int w = 0, int h = 0);
	void setFrame(int fx, int fy);

	SDL_Texture *getTexture() const { return texture; }
	SDL_Rect getSourceRect() const { return srcRect; }
};

#endif
