#include "background.h"

Background::Background(SDL_Texture *image, float velX)
{
	GameObject::init(0, 0, velX, 0, -1, 0, 0, 0);

	setID(MISC);
	setCollidable(false);

	sprite.init(image);
	SDL_QueryTexture(image, NULL, NULL, &frameWidth, &frameHeight);
}

void Background::update(Phi *phi)
{
	GameObject::update(phi);
	if (x + frameWidth <= 0) {
		x = 0;
	}
}

void Background::render(Phi *phi, float dt)
{
	GameObject::render(phi, dt);
	phi->renderSprite(&sprite, x, y);
	if (x + frameWidth < SCREEN_WIDTH)
		phi->renderSprite(&sprite, x + frameWidth, y);
}
