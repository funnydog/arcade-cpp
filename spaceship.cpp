#include "spaceship.h"

class TakeLife : public Command
{
private:
	SpaceShip *ship;
public:
	explicit TakeLife(SpaceShip *ship) : ship(ship) {}

	void execute() const override { ship->loseLife(); }
};

class ScorePoint : public Command
{
private:
	SpaceShip *ship;
public:
	explicit ScorePoint(SpaceShip *ship) : ship(ship) {}

	void execute() const override { ship->addPoint(); }
};

SpaceShip::SpaceShip() :
	takeLife(new class TakeLife(this)),
	scorePoint(new class ScorePoint(this))
{
}

SpaceShip::~SpaceShip()
{
	delete scorePoint;
	delete takeLife;
}

void SpaceShip::init(SDL_Texture *image)
{
	GameObject::init(20, 200, 6, 6, 0, 0, 10, 12);
	setID(PLAYER);

	setAlive(true);
	lives = 3;
	score = 0;

	maxFrame = 3;
	curFrame = 0;
	frameWidth = 46;
	frameHeight = 41;
	animationRow = 1;

	if (image != nullptr) {
		sprite.init(image, 0, 0, frameWidth, frameHeight);
	}
}

void SpaceShip::update(Phi *phi)
{
	if (phi->events.checkInput(Events::UP)) {
		moveUp();
	} else if (phi->events.checkInput(Events::DOWN)) {
		moveDown();
	} else {
		resetAnimation(1);
	}

	if (phi->events.checkInput(Events::LEFT)) {
		moveLeft();
	} else if (phi->events.checkInput(Events::RIGHT)) {
		moveRight();
	} else {
		resetAnimation(0);
	}

	GameObject::update(phi);
	if (x < 0)
		x = 0;
	else if (x > SCREEN_WIDTH)
		x = SCREEN_WIDTH;

	if (y < 0)
		y = 0;
	else if (y > SCREEN_HEIGHT)
		y = SCREEN_HEIGHT;
}

void SpaceShip::render(Phi *phi, float dt)
{
	GameObject::render(phi, dt);

	sprite.setFrame(curFrame, animationRow);
	phi->renderSprite(&sprite, x - frameWidth / 2, y - frameHeight / 2);
}

void SpaceShip::moveUp()
{
	animationRow = 0;
	dirY = -1;
}

void SpaceShip::moveDown()
{
	animationRow = 2;
	dirY = 1;
}

void SpaceShip::moveLeft()
{
	curFrame = 2;
	dirX = -1;
}

void SpaceShip::moveRight()
{
	curFrame = 1;
	dirX = 1;
}

void SpaceShip::resetAnimation(int position)
{
	if (position == 1) {
		animationRow = 1;
		dirY = 0;
	} else {
		curFrame = 0;
		dirX = 0;
	}
}

void SpaceShip::hasCollided(int objectID)
{
	if (objectID == ENEMY) {
		lives--;
	}
}
