#include "gameview.h"
#include "menuview.h"

MenuAction::MenuAction(View *view, SDL_Texture *idle, SDL_Texture *hover) :
	view(view)
{
	MenuAction::idle.init(idle);
	MenuAction::hover.init(hover);
}

MenuView::MenuView() :
	selected(0),
	music(nullptr)
{
}

MenuView::~MenuView()
{
	for (auto &action: actions) {
		SDL_DestroyTexture(action.idle.getTexture());
		SDL_DestroyTexture(action.hover.getTexture());
	}
	actions.clear();
}

MenuView&
MenuView::getInstance()
{
	static MenuView menuView;
	return menuView;
}

int
MenuView::addAction(Phi *phi, const std::string &name, View *view)
{
	SDL_Color idle = { 150, 150, 150, 255 };
	SDL_Color hover = { 255, 255, 255, 255 };

	TTF_Font *font32 = phi->loadFont("../data/belligerent.ttf", 32);
	TTF_Font *font38 = phi->loadFont("../data/belligerent.ttf", 38);
	if (font32 == nullptr || font38 == nullptr)
		return -1;

	SDL_Texture *idleTex = phi->strToTexture(name, font32, idle);
	SDL_Texture *hoverTex = phi->strToTexture(name, font38, hover);
	TTF_CloseFont(font38);
	TTF_CloseFont(font32);

	if (idleTex == nullptr || hoverTex == nullptr) {
		SDL_DestroyTexture(hoverTex);
		SDL_DestroyTexture(idleTex);
		return -1;
	}

	actions.push_back(MenuAction(view, idleTex, hoverTex));
	return 0;
}

int
MenuView::enter(Phi *phi)
{
	music = Mix_LoadMUS("../data/soundtrack.mp3");
	if (music == nullptr)
		return -1;

	if (addAction(phi, "Start Game", &GameView::getInstance()) != 0
	    || addAction(phi, "Settings", nullptr) != 0
	    || addAction(phi, "Quit", nullptr) != 0)
		return -1;

	selected = 0;

	phi->playMusic(music);
	return 0;
}

void
MenuView::exit(Phi *phi)
{
	phi->playMusic(nullptr);

	Mix_FreeMusic(music);
	music = nullptr;

	for (auto &action: actions) {
		SDL_DestroyTexture(action.idle.getTexture());
		SDL_DestroyTexture(action.hover.getTexture());
	}
	actions.clear();
}

int
MenuView::update(Phi *phi)
{
	if (phi->events.checkTrigger(Events::QUIT)) {
		phi->quit();
		return 0;
	}

	if (phi->events.checkTrigger(Events::SPACE)) {
		if (actions[selected].view == nullptr) {
			phi->quit();
			return 0;
		} else {
			return phi->changeView(actions[selected].view);
		}
	}

	if (phi->events.checkTrigger(Events::UP) && selected > 0)
		selected--;

	if (phi->events.checkTrigger(Events::DOWN) && selected + 1 < (int)actions.size())
		selected++;

	return 0;
}

void
MenuView::render(Phi *phi, float dt)
{
	SDL_Renderer *ren = phi->getRenderer();

	int screenWidth, screenHeight;
	phi->getSize(&screenWidth, &screenHeight);

	const int boxWidth = 360;
	int boxHeight = actions.size() * 50;
	const int borderWidth = 3;
	const int marginHeight = 10;

	SDL_Rect fillRect = {
		(screenWidth - boxWidth) / 2 - borderWidth,
		(screenHeight - boxHeight) / 2 - marginHeight - borderWidth,
		boxWidth + borderWidth * 2,
		boxHeight + borderWidth * 2 + marginHeight * 2
	};

	// clear
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);

	// render the box
	SDL_SetRenderDrawColor(ren, 70, 15, 70, 255);
	SDL_RenderFillRect(ren, &fillRect);

	// render the labels
	int i = 0;
	for (auto& action : actions) {
		Sprite *s = (i == selected) ? &action.hover : &action.idle;
		SDL_Rect srcRect = s->getSourceRect();

		int x = (screenWidth - srcRect.w) / 2.0;
		int y = (screenHeight - boxHeight + 50 - srcRect.h) / 2 + 50 * i;

		phi->renderSprite(s, x, y);
		i++;
	}
}
