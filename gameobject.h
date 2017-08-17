#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <SDL.h>

#include "globals.h"
#include "phi.h"

class GameObject
{
private:
	int ID;
	bool alive;
	bool collidable;

protected:
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;
	int boundX;
	int boundY;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	Sprite sprite;

public:
	GameObject();
	virtual ~GameObject();

	void init(float x, float y, float velX, float velY, int dirX, int dirY, int boundX, int boundY);
	virtual void update(Phi *phi);
	virtual void render(Phi *phi, float dt);

	float getX() { return x; }
	float getY() { return y; }

	void setX(float x) { GameObject::x = x; }
	void setY(float y) { GameObject::y = y; }

	int getBoundX() { return boundX; }
	int getBoundY() { return boundY; }

	int getID() { return ID; }
	void setID(int ID) { GameObject::ID = ID; }

	bool getAlive() { return alive; }
	void setAlive(bool alive) { GameObject::alive = alive; }

	bool getCollidable() { return collidable; }
	void setCollidable(bool collidable) { GameObject::collidable = collidable; }

	bool checkCollision(GameObject *otherObject);
	virtual void hasCollided(int objectID);

	bool isCollidable() { return alive && collidable; }
};

#endif
