#pragma once

#include "Actor.h"

class FreezeFrame;

namespace turretNS{
	const float ROTATION_SPEED = PI/4;
	const float ENGAGE_DISTANCE_SQRD = pow(512,2); // replace this with "line of sight"
	const float ROTATION_WIDTH = PI/2;
	const float ROT_EPSILON = 0.001;
	const float FIRE_RATE = 0.75;
	const float REBOOT_TIME = 2;
	const VECTOR2 BULLET1_OFFSET(30,-15);
	const VECTOR2 BULLET2_OFFSET(30,15);
	const float TURRET_RADIUS = 32;
};

class Turret:public Actor
{
private:
	FreezeFrame * game;
	Actor base;
	float minDir, maxDir,rotVel;
	float weaponCooldown;
	float rebootCooldown;
public:
	Turret();

	bool initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *turretTM, TextureManager *baseTM);

	void update(float frameTime);

	void animate(float frameTime);

	void draw(VECTOR2 screenLoc);

	void create(VECTOR2 loc, float dir);

	void hit();
};