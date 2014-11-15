#include "ghost.h"
#include"freezeframe.h"

Ghost::Ghost():Actor(){
	radius = 32;
	timer = 0;
	ghosty = 0;
	collisionType = COLLISION_TYPE::CIRCLE;
	colorFilter = GhostNS::COLOR;
	setActive(false);
	target = false;
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
	if(D3DXVec2LengthSq(&disp) > GhostNS::VEC_TRACK_EPS)
	{
		D3DXVec2Normalize(&disp,&disp);
		setVelocity(disp);
	}
	else
		setVelocity(VECTOR2(0,0));
}

void Ghost::ai(float time, Actor &t)
{ 
	if(active) {

		if(target) {
			targetEntity = t;
			ghosty += time;
			if(ghosty > 4) {
				audio->playCue(GHOST_CUE);
				ghosty = 0;
			}
		}
		else {
			targetEntity = game->getHome();
			ghosty += time;
		}
		if(pattern == false) {
			vectorTrack();
			timer += time;

			if(timer > 10) {
				target = !target;
				timer = 0;
			}
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