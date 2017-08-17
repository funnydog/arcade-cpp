#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "gameobject.h"

class Background : public GameObject
{
private:
public:
	Background(SDL_Texture *image, float velX);
	virtual ~Background() {}

	void update(Phi *phi) override;
	void render(Phi *phi, float dt) override;
};

#endif
