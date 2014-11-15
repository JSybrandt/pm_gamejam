#include "ghost.h"
#include"freezeframe.h"

Ghost::Ghost():Actor(){
	radius = 32;
	collisionType = COLLISION_TYPE::CIRCLE;
	colorFilter = GhostNS::COLOR;
	setActive(false);
	target = true;
	shoot = false;
	pattern = false;
}
Ghost::~Ghost(){}

bool Ghost::initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *textureM, bool pattern)
{
	this->pattern = pattern;
	game = g;
	return Actor::initialize(game,width,height,ncols,textureM);
}


void Ghost::update(float frameTime)
{
	if(getActive())
	{

		VECTOR2 endLoc = getCenter()+(getVelocity()*GhostNS::SPEED*frameTime);
		game->getRealEndLoc(getCenter(),endLoc,this);

	}

}

void Ghost::evade()
{
	VECTOR2 disp = targetEntity.getCenter()-getCenter();
	D3DXVec2Normalize(&disp,&disp);
	setVelocity(-disp);
}

void Ghost::deltaTrack()
{

	VECTOR2 v(0,0);

	if(targetEntity.getCenterX() < getCenterX())
		v.x = -1;
	if(targetEntity.getCenterX() > getCenterX())
		v.x = 1;

	if(targetEntity.getCenterY() < getCenterY())
		v.y = -1;
	if(targetEntity.getCenterY() > getCenterY())
		v.y = 1;

	D3DXVec2Normalize(&v, &v);
	setVelocity(v);

}
void Ghost::vectorTrack()
{
	VECTOR2 disp = targetEntity.getCenter()-getCenter();
	D3DXVec2Normalize(&disp,&disp);
	setVelocity(disp);
}

void Ghost::ai(float time, Actor &t)
{ 
	if(active) {
		if(pattern == false) {
			targetEntity = t;
			vectorTrack();
		}
		
	}
	return;
}

void Ghost::create(VECTOR2 loc)
{
	target = false;
	shoot = false;
	velocity = VECTOR2(0,0);
	setActive(true);
	setCenter(loc);
	setHealth(100);


	personalChaseDistanceSQRD = (rand01()+0.5) * GhostNS::CHASE_DISTANCE_SQRD;
	personalEngageDistanceSQRD = (rand01()+0.5) * GhostNS::ENGAGE_DISTANCE_SQRD;
}