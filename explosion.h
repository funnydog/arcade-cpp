#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "gameobject.h"

class Explosion : public GameObject
{
private:
public:
	Explosion(float x, float y, SDL_Texture *image);
	virtual ~Explosion() {}

	void update(Phi *phi) override;
	void render(Phi *phi, float dt) override;
};

#endif
