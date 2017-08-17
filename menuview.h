#ifndef MENUVIEW_H
#define MENUVIEW_H

#include <deque>

#include "phi.h"
#include "sprite.h"
#include "view.h"

struct MenuAction
{
	View *view;
	Sprite idle;
	Sprite hover;

	explicit MenuAction(View *view, SDL_Texture *idle, SDL_Texture *hover);
};

class MenuView : public View
{
public:
	static MenuView& getInstance();
	virtual ~MenuView();

	virtual int enter(Phi *phi) override;
	virtual void exit(Phi *phi) override;

	virtual int update(Phi *phi) override;
	virtual void render(Phi *phi, float lag) override;

private:
	MenuView();
	int addAction(Phi *phi, const std::string &name, View *view);

	std::deque<MenuAction> actions;
	int selected;
	Mix_Music *music;
};

#endif
