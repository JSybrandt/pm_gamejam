#pragma once

#include"Actor.h"

class FreezeFrame;

namespace bulletNS{
	const float SPEED = 600;
};

class Bullet: public Actor
{
private:
	FreezeFrame* game;
	VECTOR2 firedLocation;
	
	Image line;

public:

	Bullet();

	bool initialize(FreezeFrame* g, int width, int height, int ncols,TextureManager *bulletTM, TextureManager *lineTM);

	void create(VECTOR2 loc, float dir,COLOR_ARGB c);

	void update(float frametime);

	void draw(VECTOR2 screenLoc);

};