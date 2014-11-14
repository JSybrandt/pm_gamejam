#include "ghost.h"
#include"freezeframe.h"

Ghost::Ghost():Actor(){
	edge.top = -32;
	edge.bottom = 32;
	edge.left = -12;
	edge.right = 12;
	collisionType = COLLISION_TYPE::BOX;
	colorFilter = GhostNS::COLOR;
	setActive(false);
	target = false;
	shoot = false;
}
Ghost::~Ghost(){}

bool Ghost::initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *textureM)
{
	game = g;
	return Actor::initialize(game,width,height,ncols,textureM);
}


void Ghost::update(float frameTime)
{
	if(getActive())
	{
		if(health <= 0) {
			setActive(false);
			audio->playCue(KILL1_CUE);
		}

		VECTOR2 endLoc = getCenter()+(getVelocity()*GhostNS::SPEED*frameTime);
		endLoc = game->getRealEndLoc(getCenter(),endLoc);
		setCenter(endLoc);
		VECTOR2 aim(game->getPlayerLoc().x - endLoc.x,game->getPlayerLoc().y - endLoc.y);
		float aimDir = atan2(aim.y,aim.x);

		if(velocity != VECTOR2(0,0)) {
			setRadians(atan2(velocity.y,velocity.x));
			Actor::update(frameTime);
		}

		if(shoot && weaponCooldown <= 0){
			//because we dont want to use the angle form player center
			VECTOR2 bulletLoc = getCenter()+utilityNS::rotateVector(GhostNS::bulletDisplacement,aimDir); //
			VECTOR2 bulletPath = jiggleVector(game->getPlayerLoc()) - bulletLoc;
			float bulletAngle = atan2(bulletPath.y,bulletPath.x);

			game->spawnBullet(bulletLoc,bulletAngle,getColorFilter(),false);
			setRadians(bulletAngle);
			weaponCooldown  = GhostNS::WEAPON_COOLDOWN;
			recoilCooldown = GhostNS::RECOIL_TIME;

			animComplete = false;
			setCurrentFrame(0);
			audio->playCue(PISTOL_CUE);
		}
		else
			setRadians(aimDir);

		weaponCooldown -= frameTime;
		if(weaponCooldown < 0) weaponCooldown =0;

		recoilCooldown -= frameTime;
		if(recoilCooldown < 0)recoilCooldown = 0;
	}

}

void Ghost::evade(float frameTime)
{
	VECTOR2 disp = targetEntity.getCenter()-getCenter();
	D3DXVec2Normalize(&disp,&disp);
	setVelocity(-disp);
}

void Ghost::deltaTrack(float frametime)
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
void Ghost::vectorTrack(float frametime)
{
	VECTOR2 disp = targetEntity.getCenter()-getCenter();
	D3DXVec2Normalize(&disp,&disp);
	setVelocity(disp);
}

void Ghost::ai(float time, Actor &t)
{ 
	if(active) {
		VECTOR2 toPlayer = game->getPlayerLoc() - getCenter();
		float distSqrdToPlayer = D3DXVec2LengthSq(&toPlayer);

		if(distSqrdToPlayer > GhostNS::LOSE_DISTANCE_SQRD) {
			target = false;
			shoot = false;
			setVelocity(VECTOR2(0,0));
		}
		else if(distSqrdToPlayer < personalChaseDistanceSQRD) {
			target = true;
			//shoot = false;
		}

		if(target && distSqrdToPlayer < GhostNS::LOSE_DISTANCE_SQRD && distSqrdToPlayer > personalEngageDistanceSQRD) {
			shoot = true;
			targetEntity = t;
			vectorTrack(time);
			//setVelocity(VECTOR2(0,0));
		}
		else if(target && distSqrdToPlayer < GhostNS::LOSE_DISTANCE_SQRD) {
			shoot = true;
			setVelocity(VECTOR2(0,0));
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