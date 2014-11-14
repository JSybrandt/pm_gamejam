#include "LandMine.h"
using namespace landmineNS;
using std::string;
using std::to_string;

#include "freezeframe.h"


LandMine::LandMine()
{
	radius = MINE_RADIUS;
	setActive(false);
	primed = false;
	exploding = false;
	timeRemaining = 0;
}
void LandMine::update(float frametime)
{
	if(getActive())
	{
		timeRemaining -= frametime;
		beeper -= frametime;
		if(timeRemaining < 0) 
		{
			if(primed)
			{
				timeRemaining = 0;
				primed = false;
				exploding = true;
				radius = EXPLOSION_RADIUS;
				game->spawnParticleCloud(getCenter(),graphicsNS::RED);
				game->spawnParticleCloud(getCenter(),graphicsNS::ORANGE);
				
			}
			else if(exploding)
			{
				timeRemaining = 0;
				exploding = false;
				audio->playCue(EXP3_CUE);
				setActive(false);
			}
		}
		if(beeper <= 0 && timeRemaining > 0) {
			audio->playCue(BEEP_CUE);
			beeper = timeRemaining/3.0;
		}
		Image::update(frametime);
	}
}
void LandMine::draw(VECTOR2 screenLoc)
{
	if(getActive())
	{
		
		if(primed)
		{
			dangerZone.setColorFilter(Graphics::calculateAlpha(0.5-(timeRemaining/TIME_UNTIL_EXPLOSION)/2));
			dangerZone.draw(screenLoc);

			Actor::draw(screenLoc);
			int whole = (int)timeRemaining;
			int dec = (timeRemaining-whole)*10;
			string time = to_string(whole)+"."+to_string(dec);
			VECTOR2 textLoc = getCenter()+TEXT_DISPLACEMENT - screenLoc;
			text->print(time,textLoc.x,textLoc.y);

		}
	}
}

void LandMine::create(VECTOR2 loc)
{
	setCenter(loc);
	setActive(true);
	primed = false;
	exploding = false;
	radius = MINE_RADIUS;
	dangerZone.setCenter(loc);
}

void LandMine::wasSteppedOn()
{
	if(!primed && !exploding)
	{
		primed = true;
		timeRemaining = TIME_UNTIL_EXPLOSION;
	}
}

bool LandMine::initialize(FreezeFrame * g, TextDX * t,int width, int height, int ncols, TextureManager *textureM, TextureManager *dangerZoneM)
{
	game = g;
	text = t;
	bool result = dangerZone.initialize(g->getGraphics(),0,0,0,dangerZoneM);
	return result && Actor::initialize(g,width,height,ncols,textureM);
}