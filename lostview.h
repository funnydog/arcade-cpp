#ifndef LOSTVIEW_H
#define LOSTVIEW_H

#include "view.h"

class LostView : public View
{
private:
	SDL_Texture *image;

	LostView();
public:
	static LostView& getInstance();
	virtual ~LostView();

	virtual int enter(Phi *phi) override;
	virtual void exit(Phi *phi) override;

	virtual int update(Phi *phi) override;
	virtual void render(Phi *phi, float dt) override;
};

#endif
