#pragma once

#include"Actor.h"
#include"Controls.h"
#include"constants.h"

class FreezeFrame;

namespace playerNS{
	const float RUN_SPEED = 300;
	const float WALK_SPEED = 150;
	const float WEAPON_COOLDOWN = 1;
	const VECTOR2 bulletDisplacement(28,-18);
	const float RECOIL_TIME = 0.25;

	const float D_TIME_PER_FRAME = 0.045;
	const float RUN_TM = 1;
	const float WALK_TM = 0.3;
	const float STANDING_TM = 0.01;
	const float RECOIL_TM = 1;
	const float DEATH_TM = 0.5;

	const float TIME_EPSILON = 0.03;
	const float POS_EPSILON_SQRD = 144;
	const float ROTATION_EPSILON = 0.01;

	const int CYLINDER_MAX_IMG_INDEX = 6;
	const float TIME_FOR_GUN_RAISING = 0.5;
	const float GUN_BULLET_CAPACITY = 6;
	const float CYLINDER_ROTATION_SPEED = PI/3*WEAPON_COOLDOWN;
	const float CYLINDER_VERTICAL_SPEED = 600;

	const float CYLINDER_WIDTH = 128;
	const float CYLINDER_HEIGHT = 128;
	const float CYLINDER_COLS = 4;
	
	const VECTOR2 CYLINDER_OFFSCREEN(64,GAME_HEIGHT*-0.2);
	const VECTOR2 CYLINDER_ONSCREEN(64,72);

	const VECTOR2 TEXT_DISPLACEMENT(-32,-50);

}


class Player: public Actor
{
private:
	Controls controls;
	FreezeFrame* game;
	Actor feet;
	Image cylinder;
	float weaponCooldown;
	float recoilCooldown;
	float currentTimeMultiplier;
	float desiredTimeMultiplier;

	float timeUntilWallCollision;

	int numBullets;
	bool gunAnimation;
	float cylinderDesiredDir;
	
	bool showUseMessage;

	TextDX * text;

	bool mouseDown;

public:
	Player();
	~Player();

	bool initialize(FreezeFrame * game, Controls c, TextDX* t, int width, int height, int ncols, TextureManager *playerTM, TextureManager *feetTM, TextureManager *cylinderTM);

	void draw(VECTOR2 screenLoc);

	void update(float &frameTime);

	void setTimeOfWallCollision(float time){timeUntilWallCollision=time;};

	bool alive;

	void pickUpGun();

	void set(VECTOR2 loc);

	bool readyToFire(){return weaponCooldown <= 0;};

	void setBullets(int n){numBullets = n;}
};