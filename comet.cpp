#include "comet.h"

Comet::Comet(float x, float y, SDL_Texture *image, const Command *takeLife)
{
	GameObject::init(x, y, 5, 0, -1, 0, 35, 35);
	setID(ENEMY);

	maxFrame = 143;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 2;
	frameWidth = 96;
	frameHeight = 96;
	animationColumns = 21;

	if (rand() % 2)
		animationDirection = 1;
	else
		animationDirection = -1;

	sprite.init(image, 0, 0, frameWidth, frameHeight);
	Comet::takeLife = takeLife;
}

Comet::~Comet()
{
}

void Comet::update(Phi *phi)
{
	GameObject::update(phi);

	if (++frameCount >= frameDelay) {
		curFrame += animationDirection;
		if (curFrame >= maxFrame)
			curFrame = 0;
		else if (curFrame < 0)
			curFrame = maxFrame - 1;

		frameCount = 0;

		sprite.setFrame(
			curFrame % animationColumns,
			curFrame / animationColumns
			);
	}

	if (x + frameWidth < 0) {
		hasCollided(BORDER);
	}
}

void Comet::render(Phi *phi, float dt)
{
	GameObject::render(phi, dt);
	phi->renderSprite(&sprite, x - frameWidth / 2, y - frameHeight / 2);
}

void Comet::hasCollided(int objectID)
{
	if (objectID == BORDER) {
		takeLife->execute();
	}

	if (objectID != ENEMY) {
		setAlive(false);
	}
}
