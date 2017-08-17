#include <sstream>

#include "background.h"
#include "bullet.h"
#include "comet.h"
#include "explosion.h"
#include "gameview.h"
#include "menuview.h"
#include "lostview.h"

GameView::GameView() :
	shipImage(nullptr),
	bulletImage(nullptr),
	cometImage(nullptr),
	explImage(nullptr),
	bgImage(nullptr),
	mgImage(nullptr),
	fgImage(nullptr),
	font18(nullptr),
	music(nullptr),
	bulletSound(nullptr),
	explosionSound(nullptr)
{
}

GameView::~GameView()
{
	for (auto iter = objects.begin(); iter != objects.end(); ++iter) {
		if ((*iter) != &ship)
			delete (*iter);
	}
	objects.clear();
	destroyAssets();
}

GameView&
GameView::getInstance()
{
	static GameView myGameView;
	return myGameView;
}

int
GameView::enter(Phi *phi)
{
	if (loadAssets(phi) < 0)
		return -1;

	ship.init(shipImage);
	objects.push_back(new Background(bgImage, 1));
	objects.push_back(new Background(mgImage, 2));
	objects.push_back(new Background(fgImage, 3));
	objects.push_back(&ship);

	phi->playMusic(music);
	return 0;
}

void
GameView::exit(Phi *phi)
{
	phi->playMusic(nullptr);
	for (auto iter = objects.begin(); iter != objects.end(); ++iter) {
		if ((*iter) != &ship)
			delete (*iter);
	}
	objects.clear();
	destroyAssets();
}

int
GameView::update(Phi *phi)
{
	// input update
	if (phi->events.checkTrigger(Events::QUIT)) {
		phi->changeView(&MenuView::getInstance());
		return 0;
	}

	if (phi->events.checkTrigger(Events::SPACE)) {
		Bullet *bullet = new Bullet(
			ship.getX() + 17,
			ship.getY(),
			bulletImage,
			ship.getScorePoint());
		objects.push_back(bullet);
		phi->playSound(bulletSound);
	}

	if (rand() % 100 == 0) {
		Comet *comet = new Comet(
			SCREEN_WIDTH + 96/2,
			30 + rand() % (SCREEN_HEIGHT - 60),
			cometImage,
			ship.getTakeLife());
		objects.push_back(comet);
	}

	// world update
	for (auto iter = objects.begin(); iter != objects.end(); ++iter) {
		(*iter)->update(phi);
	}

	// collisions
	for (auto iter = objects.begin(); iter != objects.end(); ++iter) {
		if (!(*iter)->isCollidable())
			continue;

		for (auto iter2 = iter; iter2 != objects.end(); ++iter2) {
			if (! (*iter2)->isCollidable())
				continue;

			if ((*iter)->getID() == (*iter2)->getID())
				continue;

			if ((*iter)->checkCollision(*iter2)) {
				(*iter)->hasCollided((*iter2)->getID());
				(*iter2)->hasCollided((*iter)->getID());
				Explosion *explosion = new Explosion(
					((*iter)->getX() + (*iter2)->getX()) / 2,
					((*iter)->getY() + (*iter2)->getY()) / 2,
					explImage);
				objects.push_back(explosion);
				phi->playSound(explosionSound);
			}
		}
	}

	if (ship.getLives() <= 0)
		return phi->changeView(&LostView::getInstance());

	// cull the dead
	for (auto iter = objects.begin(); iter != objects.end();) {
		if (!(*iter)->getAlive()) {
			delete (*iter);
			iter = objects.erase(iter);
		} else {
			++iter;
		}
	}

	return 0;
}

void
GameView::render(Phi *phi, float dt)
{
	SDL_Renderer *ren = phi->getRenderer();

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);

	for (auto &obj: objects)
		obj->render(phi, dt);

	SDL_Color magenta = { 255, 0, 255, 255 };
	std::ostringstream sb;
	sb << "Player has " << ship.getLives()
	   << " lives left. Player has destroyed " << ship.getScore()
	   << " objects.";
	phi->renderText(sb.str(), 5, 5, font18, magenta);
}

int
GameView::loadAssets(Phi *phi)
{
	SDL_Color magenta = {255, 0, 255, 255};

	font18 = phi->loadFont("../data/belligerent.ttf", 18);
	if (font18 == nullptr)
		return -1;

	// game entities
	shipImage = phi->loadTexture("../data/spaceship_by_arboris.png", magenta);
	bulletImage = phi->loadTexture("../data/spaceship_sprites_by_arboris.png", magenta);
	cometImage = phi->loadTexture("../data/asteroid.png", magenta);
	explImage = phi->loadTexture("../data/explosion.png", magenta);
	if (shipImage == nullptr || bulletImage == nullptr || cometImage == nullptr || explImage == nullptr)
		return -1;

	// background
	bgImage = phi->loadTexture("../data/starBG.png", magenta);
	mgImage = phi->loadTexture("../data/starMG.png", magenta);
	fgImage = phi->loadTexture("../data/starFG.png", magenta);
	if (bgImage == nullptr || mgImage == nullptr || fgImage == nullptr)
		return -1;

	// music
	music = Mix_LoadMUS("../data/mdk_phoenix_orchestral.ogg");
	if (music == nullptr)
		return -1;

	// sounds
	bulletSound = Mix_LoadWAV("../data/bullet.ogg");
	explosionSound = Mix_LoadWAV("../data/explosion.ogg");
	if (bulletSound == nullptr || explosionSound == nullptr)
		return -1;

	return 0;
}

void
GameView::destroyAssets()
{
	Mix_FreeChunk(bulletSound);
	Mix_FreeChunk(explosionSound);
	bulletSound = explosionSound = nullptr;

	Mix_FreeMusic(music);
	music = nullptr;

	SDL_DestroyTexture(fgImage);
	SDL_DestroyTexture(mgImage);
	SDL_DestroyTexture(bgImage);
	SDL_DestroyTexture(explImage);
	SDL_DestroyTexture(cometImage);
	SDL_DestroyTexture(bulletImage);
	SDL_DestroyTexture(shipImage);

	shipImage = bulletImage = cometImage = nullptr;
	explImage = bgImage = mgImage = fgImage = nullptr;

	TTF_CloseFont(font18);
	font18 = nullptr;
}
