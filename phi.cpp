#include <iostream>

#include <SDL_image.h>
#include <SDL_mixer.h>

#include "phi.h"
#include "view.h"

using namespace std;

static void logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " error: " << SDL_GetError() << std::endl;
}

Phi::Phi() :
	renderer(nullptr),
	window(nullptr),
	curView(nullptr),
	exitLoop(false),
	soundChannels(32)
{
}

Phi::~Phi()
{
	// exit any left views
	while (!views.empty()) {
		views.back()->exit(this);
		views.pop_back();
	}
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	SDL_DestroyWindow(window);
	window = nullptr;

	quitLibs();
}

int
Phi::initLibs()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logError(SDL_GetError());
		return -1;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logError(IMG_GetError());
		SDL_Quit();
		return -1;
	}

	if (TTF_Init() != 0) {
		logError(TTF_GetError());
		IMG_Quit();
		SDL_Quit();
		return -1;
	}

	unsigned flags = MIX_INIT_MP3 | MIX_INIT_OGG;
	if ((Mix_Init(flags) & flags) != flags) {
		logError(Mix_GetError());
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		return -1;
	}

	if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 1024) < 0
	    || Mix_AllocateChannels(soundChannels) < 0) {
		logError(Mix_GetError());
		quitLibs();
		return -1;
	}

	return 0;
}

void
Phi::quitLibs()
{
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
int
Phi::createWindow(const std::string &title, int width, int height)
{
	if (window != nullptr || renderer != nullptr || initLibs() < 0)
		return -1;

	window = SDL_CreateWindow(
		title.c_str(),
		100, 100, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logError(SDL_GetError());
		quitLibs();
		return -1;
	}

	renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logError(SDL_GetError());
		SDL_DestroyWindow(window);
		window = nullptr;
		quitLibs();
		return -1;
	}

	return 0;
}

int
Phi::getSize(int *w, int *h)
{
	return SDL_GetRendererOutputSize(renderer, w, h);
}


SDL_Texture *
Phi::loadTexture(const char *path)
{
	SDL_Texture *texture = IMG_LoadTexture(renderer, path);
	if (texture == nullptr)
		logSDLError(std::cerr, "IMG_LoadTexture");

	return texture;
}

SDL_Texture *
Phi::loadTexture(const char *path, SDL_Color key)
{
	SDL_Surface *image = IMG_Load(path);
	if (image == nullptr) {
		logSDLError(std::cerr, "IMG_Load");
		return nullptr;
	}

	Uint32 colorkey = SDL_MapRGB(image->format, key.r, key.g, key.b);
	SDL_SetColorKey(image, SDL_TRUE, colorkey);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	if (texture == nullptr)
		logSDLError(std::cerr, "SDL_CreateTextureFromSurface");

	return texture;
}

SDL_Texture *
Phi::strToTexture(const std::string &text, TTF_Font *font, SDL_Color col)
{
	SDL_Surface *surf = TTF_RenderText_Blended(font, text.c_str(), col);
	if (surf == nullptr) {
		logSDLError(std::cerr, "TTF_RenderText");
		return nullptr;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	return texture;
}

TTF_Font *
Phi::loadFont(const char *path, int size)
{
	return TTF_OpenFont(path, size);
}

void
Phi::renderText(const std::string &text, int x, int y, TTF_Font *font, SDL_Color color)
{
	SDL_Texture *texture = strToTexture(text, font, color);
	if (texture) {
		SDL_Rect dst;
		dst.x = x;
		dst.y = y;
		SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
		SDL_RenderCopy(renderer, texture, NULL, &dst);
		SDL_DestroyTexture(texture);
	}
}

void
Phi::renderSprite(const Sprite *s, int x, int y)
{
	SDL_Rect src = s->getSourceRect();
	SDL_Rect dst = src;
	dst.x = x;
	dst.y = y;
	SDL_RenderCopy(renderer, s->getTexture(), &src, &dst);
}

void
Phi::playSound(Mix_Chunk *chunk)
{
	if (Mix_PlayChannel(-1, chunk, 0) < 0) {
		soundChannels *= 2;
		if (Mix_AllocateChannels(soundChannels) < 0)
			return;

		playSound(chunk);
	}
}

void
Phi::playMusic(Mix_Music *music)
{
	Mix_PlayMusic(music, -1);
}

int
Phi::pushView(View *view)
{
	curView = view;
	views.push_back(curView);
	return curView->enter(this);
}

void
Phi::popView()
{
	if (!views.empty()) {
		curView->exit(this);
		views.pop_back();
		curView = views.empty() ? nullptr : views.back();
	}
}

int
Phi::changeView(View *view)
{
	popView();
	return pushView(view);
}

int
Phi::update()
{
	if (!curView)
		return -1;

	return curView->update(this);
}

void
Phi::render(float dt)
{
	if (curView) {
		curView->render(this, dt);
		SDL_RenderPresent(renderer);
	}
}

bool
Phi::shouldQuit()
{
	return curView == nullptr || exitLoop || events.quit;
}

void
Phi::quit()
{
	exitLoop = true;
}

void
Phi::logInfo(const std::string &msg)
{
	std::cerr << "Info: " << msg << std::endl;
}

void
Phi::logWarning(const std::string &msg)
{
	std::cerr << "Warning: " << msg << std::endl;
}

void
Phi::logError(const std::string &msg)
{
	std::cerr << "Error: " << msg << std::endl;
}
