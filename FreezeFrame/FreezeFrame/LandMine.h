#pragma once

#include "Actor.h"
#include "textDX.h"
#include <string>

class FreezeFrame;

namespace landmineNS{
	const float TIME_UNTIL_EXPLOSION = 1;
	const float EXPLOSION_LIFETIME = 0.25;
	const float MINE_RADIUS = 64;
	const float EXPLOSION_RADIUS = 128;
	const VECTOR2 TEXT_DISPLACEMENT(-12,-32);
};

class LandMine: public Actor
{
private:
	bool primed,exploding;
	float timeRemaining, beeper;
	TextDX * text;
	FreezeFrame * game;
	Image dangerZone;
	
public:
	LandMine();
	void update(float frametime);
	void draw(VECTOR2 screenLoc);
	void create(VECTOR2 loc);
	void wasSteppedOn();//call when the mine gets stepped on
	bool initialize(FreezeFrame * g, TextDX * text,int width, int height, int ncols, TextureManager *textureM, TextureManager *dangerZoneM);
	bool getDangerous(){return exploding&&getActive();}
	bool getPrimed(){return primed;}
};