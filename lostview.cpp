#include "menuview.h"
#include "lostview.h"

LostView::LostView() :
	image(nullptr)
{
}

LostView::~LostView()
{
	SDL_DestroyTexture(image);
}

LostView&
LostView::getInstance()
{
	static LostView lostView;
	return lostView;
}

int
LostView::enter(Phi *phi)
{
	image = phi->loadTexture("../data/Shooter_Lose.png");
	if (image == nullptr)
		return -1;

	return 0;
}

void
LostView::exit(Phi *phi)
{
	SDL_DestroyTexture(image);
	image = nullptr;
}

int
LostView::update(Phi *phi)
{
	if (phi->events.checkTrigger(Events::SPACE)) {
		return phi->changeView(&MenuView::getInstance());
	}

	if (phi->events.checkTrigger(Events::QUIT))
		phi->quit();

	return 0;
}

void
LostView::render(Phi *phi, float dt)
{
	SDL_Renderer *ren = phi->getRenderer();

	SDL_RenderClear(ren);

	SDL_Rect dst;
	dst.x = dst.y = 0;
	if (image != nullptr) {
		SDL_QueryTexture(image, NULL, NULL, &dst.w, &dst.h);
		SDL_RenderCopy(ren, image, NULL, &dst);
	}
}
