#include "Particle.h"

Particle::Particle():Actor()
{
	setActive(false);
	setRadians(utilityNS::rand01()*2*PI);
}

void Particle::update(float frametime)
{
	if(getActive())
	{
		age += frametime;
		if(age>=particleNS::MAX_LIFETIME) active = false;
		setCenter(getCenter()+frametime*velocity);

	}
}

void Particle::draw(VECTOR2 screenLoc)
{
	if(getActive())
	{
		COLOR_ARGB c = Graphics::calculateAlpha(1-(age/particleNS::MAX_LIFETIME))&colorFilter;
		Actor::draw(screenLoc,c);
	}
}


void Particle::create(VECTOR2 loc,VECTOR2 vel, COLOR_ARGB c)
{

	setActive(true);
	age = utilityNS::rand01()*particleNS::MAX_LIFETIME;
	setCenter(loc);
	setVelocity(vel);
	setColorFilter(c);


}