#include "bullet.h"

Bullet::Bullet(float x, float y, SDL_Texture *image, const Command *scorePoint)
{
	GameObject::init(x, y, 10, 0, 1, 0, 0, 0);
	setID(BULLET);
	frameWidth = frameHeight = 8;
	sprite.init(image, 211, 285, frameWidth, frameHeight);
	Bullet::scorePoint = scorePoint;
}

Bullet::~Bullet()
{
}

void Bullet::update(Phi *phi)
{
	GameObject::update(phi);

	if (x > SCREEN_WIDTH)
		hasCollided(BORDER);
}

void Bullet::render(Phi *phi, float dt)
{
	phi->renderSprite(&sprite, x - frameWidth / 2, y - frameHeight / 2);
}

void Bullet::hasCollided(int objectID)
{
	if (objectID == ENEMY || objectID == BORDER) {
		setAlive(false);
	}

	if (objectID == ENEMY) {
		scorePoint->execute();
	}
}
