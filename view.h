#ifndef VIEW_H
#define VIEW_H

#include "phi.h"

class View
{
protected:
	View() {}

public:
	virtual ~View() {}

	virtual int enter(Phi *phi) = 0;
	virtual void exit(Phi *phi) = 0;

	virtual int update(Phi *phi) = 0;
	virtual void render(Phi *phi, float dt) = 0;
};

#endif
