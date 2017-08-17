#include "sprite.h"

Sprite::Sprite() :
	texture(nullptr),
	originX(0),
	originY(0)
{
}

void
Sprite::init(SDL_Texture *texture, int x, int y, int w, int h)
{
	Sprite::texture = texture;
	srcRect.x = originX = x;
	srcRect.y = originY = y;
	SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);
	if (w != 0) srcRect.w = w;
	if (h != 0) srcRect.h = h;
}

void
Sprite::setFrame(int fx, int fy)
{
	srcRect.x = originX + fx * srcRect.w;
	srcRect.y = originY + fy * srcRect.h;
}
