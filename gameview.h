#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <list>

#include "view.h"
#include "gameobject.h"
#include "spaceship.h"

class GameView : public View
{
private:
	// assets
	SDL_Texture *shipImage;
	SDL_Texture *bulletImage;
	SDL_Texture *cometImage;
	SDL_Texture *explImage;
	SDL_Texture *bgImage;
	SDL_Texture *mgImage;
	SDL_Texture *fgImage;

	TTF_Font *font18;

	Mix_Music *music;
	Mix_Chunk *bulletSound;
	Mix_Chunk *explosionSound;

	// entity list
	std::list<GameObject *> objects;

	// player's ship
	SpaceShip ship;

	int loadAssets(Phi *phi);
	void destroyAssets();

	GameView();
public:
	static GameView &getInstance();
	virtual ~GameView();

	virtual int enter(Phi *phi) override;
	virtual void exit(Phi *phi) override;

	virtual int update(Phi *phi) override;
	virtual void render(Phi *phi, float lag) override;
};

#endif
