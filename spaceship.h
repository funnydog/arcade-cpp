#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "command.h"
#include "gameobject.h"

class SpaceShip : public GameObject
{
private:
	int lives;
	int score;
	int animationRow;

	Command *takeLife;
	Command *scorePoint;

public:
	SpaceShip();
	~SpaceShip();

	void init(SDL_Texture *image = NULL);

	void update(Phi *phi) override;
	void render(Phi *phi, float dt) override;
	void hasCollided(int objectID) override;

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void resetAnimation(int position);

	int getLives() { return lives; }
	int getScore() { return score; }

	const Command *getTakeLife() { return takeLife; }
	const Command *getScorePoint() { return scorePoint; }

	void loseLife() { lives--; }
	void addPoint() { score++; }
};

#endif
