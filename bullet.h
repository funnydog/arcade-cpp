#ifndef BULLET_H
#define BULLET_H

#include "command.h"
#include "gameobject.h"

class Bullet : public GameObject
{
private:
	const Command *scorePoint;

public:
	Bullet(float x, float y, SDL_Texture *image, const Command *scorePoint);
	virtual ~Bullet();

	void update(Phi *phi) override;
	void render(Phi *phi, float dt) override;
	void hasCollided(int objectID) override;
};

#endif
