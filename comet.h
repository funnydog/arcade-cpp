#ifndef COMET_H
#define COMET_H

#include "command.h"
#include "gameobject.h"

class Comet : public GameObject
{
private:
	const Command *takeLife;

public:
	Comet(float x, float y, SDL_Texture *image, const Command *takeLife);
	virtual ~Comet();

	void update(Phi *phi) override;
	void render(Phi *phi, float dt) override;
	void hasCollided(int objectID) override;
};

#endif
