#include "patternStep.h"

PatternStep::PatternStep()
{
    active = false;        
	timeInStep = 0;
	entity = NULL;  
	timeForStep = 0;
	action = NONE;
}

void PatternStep::initialize(Player *e)
{
	entity = e;
}

void PatternStep::update(float frameTime)
{
	if (!active) return;
	timeInStep += frameTime;
	if (timeInStep > timeForStep)
	{
		timeInStep = 0;
		active = false;
	}
	switch (action)
	{
	case NONE:
		entity->setVelocity(D3DXVECTOR2(0, 0));
		break;
	case UP:
		entity->setVelocity(D3DXVECTOR2(0, -1));
		break;
	case DOWN:
		entity->setVelocity(D3DXVECTOR2(0, 1));
		break;
	case RIGHT:
		entity->setVelocity(D3DXVECTOR2(1,0));
		break;
	case LEFT:
		entity->setVelocity(D3DXVECTOR2(-1,0));
		break;
	case UPPERLEFT:
		entity->setVelocity(D3DXVECTOR2(-.5,-.5));
		break;
	case TRACK:
		entity->vectorTrack();
		break;
	case DELTA:
		entity->deltaTrack();
		break;
	case EVADE:
		entity->evade();
		break;
	}
}