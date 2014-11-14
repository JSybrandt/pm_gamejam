#pragma once
#include"Actor.h"

class FreezeFrame;

namespace cursorNS{
	const float ROTATION_SPEED = 2*PI;
};

class Cursor: public Actor
{
private:
	FreezeFrame * game;
public:
	bool initialize(FreezeFrame * game, int width, int height, int ncols, TextureManager *textureM);
	void update(float frametime);
};