#pragma once

#include "Actor.h"

class FreezeFrame;

namespace GhostNS
{
	const float SPEED=100;
	const float LOSE_DISTANCE_SQRD = pow(600,2);
	const float CHASE_DISTANCE_SQRD = pow(500,2);
	const float ENGAGE_DISTANCE_SQRD = pow(200,2);
	const float VEC_TRACK_EPS = pow(5,2);
	const float WEAPON_COOLDOWN = 1;
	const float RECOIL_TIME = 0.25;
	const COLOR_ARGB COLOR = graphicsNS::RED;
	const VECTOR2 bulletDisplacement(28,18);
	const VECTOR2 home(640,800);

};

class Ghost:public Actor
{
public:
	FreezeFrame *game;
	Actor targetEntity;
	bool target, shoot;
	int numBullets;
	float weaponCooldown;
	float recoilCooldown;
	bool pattern;

	float timer;
	float ghosty;
	float personalChaseDistanceSQRD;
	float personalEngageDistanceSQRD;

public:
	Ghost();
	~Ghost();

	bool initialize(FreezeFrame * game, int width, int height, int ncols, TextureManager *textureM, bool pattern = false);

	void update(float frametime);

	void ai(float time, Actor &t);

	void create(VECTOR2 loc);

	bool getHunting() {return target;}

	void setTarget(bool t) {target = t;}

	void vectorTrack();
	void deltaTrack();
	void evade();

};