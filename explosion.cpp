#include "explosion.h"

Explosion::Explosion(float x, float y, SDL_Texture *image)
{
	GameObject::init(x, y, 0, 0, 0, 0, 0, 0);
	setID(EXPLOSION);
	setCollidable(false);

	maxFrame = 20;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 0;
	frameWidth = 96;
	frameHeight = 96;
	animationColumns = 5;
	animationDirection = 1;

	sprite.init(image, 0, 0, 96, 96);
}

void Explosion::update(Phi *phi)
{
	if (++frameCount >= frameDelay) {
		frameCount = 0;
		curFrame += animationDirection;
		if (curFrame >= maxFrame)
			setAlive(false);

		sprite.setFrame(
			curFrame % animationColumns,
			curFrame / animationColumns
			);
	}
}

void Explosion::render(Phi *phi, float dt)
{
	phi->renderSprite(&sprite, x - frameWidth / 2, y - frameHeight / 2);
}
