// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine Part 1
// Chapter 4 RedSpace.cpp v1.0
// RedSpace is the class we create.

#include "freezeframe.h"

//=============================================================================
// Constructor
//=============================================================================
FreezeFrame::FreezeFrame()
{
	srand(time(0));

	P1Controls = Controls(VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT,'C','E');

	worldSizes = new VECTOR2[GameState::SIZE];
	worldSizes[GameState::TitleScreen] = VECTOR2(GAME_WIDTH,GAME_HEIGHT);
	worldSizes[GameState::Level1] = VECTOR2(GAME_WIDTH,2*GAME_HEIGHT);
	worldSizes[GameState::Level2] = VECTOR2(2048,4096);
	worldSizes[GameState::Level3] = VECTOR2(1300,2048);
	worldSizes[GameState::FeelingLucky] = VECTOR2(GAME_WIDTH,GAME_HEIGHT);

	score = 0;
	currentState = Level1;

	paused = false;

	l2pCheat=false;
	infAmmoCheat=false;
	showCredits = false;
	textCooldown = 0;
	ps1 = 0;
	ps2 = 0;
	ps3 = 0;

	gameOn = true;
	numLives = STARTING_LIVES;
}

//=============================================================================
// Destructor
//=============================================================================
FreezeFrame::~FreezeFrame()
{
	releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void FreezeFrame::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError


	if(!backgroundTex.initialize(graphics,BACKGROUND_IMAGE))
		throw GameError(1,"Failed to init background tex");

	if(!manTex.initialize(graphics,PLAYER_IMAGE))
		throw GameError(2,"Failed to init man tex");
	if(!ghostTex.initialize(graphics,GHOST_IMAGE))
		throw GameError(2,"Failed to init man tex");
	if(!dotTex.initialize(graphics,DOT_IMAGE))
		throw GameError(2,"Failed to init man tex");

	if(!particleTex.initialize(graphics,PARTICLE_IMAGE))
		throw GameError(6,"Failed to init particle tex");
	if(!wallTex.initialize(graphics,WALL_IMAGE))
		throw GameError(10,"Failed to init wall tex");

	if(!dotTex.initialize(graphics,DOT_IMAGE))
		throw GameError(10,"Failed to init dot tex");
	if(!winTex.initialize(graphics,WIN_IMAGE))
		throw GameError(10,"Failed to init win tex");
	if(!loseTex.initialize(graphics,LOSE_IMAGE))
		throw GameError(10,"Failed to init lose tex");

	if(!win.initialize(graphics,0,0,0,&winTex))
		throw GameError(15,"Failed to init win image");
	if(!lose.initialize(graphics,0,0,0,&loseTex))
		throw GameError(15,"Failed to init lose image");

	for(int i = 0 ; i < MAX_DOTS; i++)
	{
		if(!dots[i].initialize(this,0,0,0,&dotTex))
			throw GameError(10,"Failed to init dot");
		dots[i].setCollisionRadius(8);
	}

	if(!infoText.initialize(graphics,20,true,false,"Verdana"))
		throw GameError(9,"Failed to init mine text");

	infoText.setFontColor(graphicsNS::BLUE);

	if(!background.initialize(graphics,0,0,0,&backgroundTex))
		throw GameError(15,"Failed to init background image");

	if(!player.initialize(this,P1Controls,&infoText,0,0,0,&manTex))
		throw GameError(24,"Failed to init player");


	for(int i = 0; i < MAX_GhostS; i++)
	{
		if(!Ghosts[i].initialize(this,0,0,0,&ghostTex))
			throw GameError(-1*i,"FAILED TO MAKE DUDE!");
		Ghosts[i].setFrames(0, 6);   // animation frames
		Ghosts[i].setCurrentFrame(0);     // starting frame
		Ghosts[i].setFrameDelay(0.05f); //0.08 seems appriopriate
	}


	for(int i = 0 ; i < MAX_PARTICLES; i++)
	{
		if(!particles[i].initialize(this,0,0,0,&particleTex))
			throw GameError(-1*i,"FAILED TO MAKE particle!");
	}


	for(int i = 0 ; i < MAX_WALLS; i++)
	{
		if(!walls[i].initialize(this,0,0,0,&wallTex))
			throw GameError(-1*i,"FAILED TO MAKE wall!");
	}

	audio->playCue(OPENING_CUE);
	introMusicCoutdown = INTRO_MUSIC_COUNTDOWN;

	level1Load();

	if(!ghost1.initialize(this,0,0,0,&ghostTex, true))
		throw GameError(-1,"FAILED TO MAKE DUDE!");
	if(!ghost2.initialize(this,0,0,0,&ghostTex, true))
		throw GameError(-1,"FAILED TO MAKE DUDE!");
	if(!ghost3.initialize(this,0,0,0,&ghostTex))
		throw GameError(-1,"FAILED TO MAKE DUDE!");

	ghost1.setColorFilter(graphicsNS::RED);
	ghost2.setColorFilter(graphicsNS::BLUE);
	ghost3.setColorFilter(graphicsNS::GREEN);

	for (int i = 0; i< 8; i++)
	{
		pattern1[i].initialize(&ghost1);
		pattern1[i].setActive();
	}
	pattern1[0].setAction(UP);
	pattern1[0].setTimeForStep(5);
	pattern1[1].setAction(RIGHT);
	pattern1[1].setTimeForStep(3);
	pattern1[2].setAction(UP);
	pattern1[2].setTimeForStep(3);
	pattern1[3].setAction(LEFT);
	pattern1[3].setTimeForStep(6);
	pattern1[4].setAction(DOWN);
	pattern1[4].setTimeForStep(3);
	pattern1[5].setAction(RIGHT);
	pattern1[5].setTimeForStep(3);
	pattern1[6].setAction(DOWN);
	pattern1[6].setTimeForStep(5);
	pattern1[7].setAction(NA);
	pattern1[7].setTimeForStep(5);

	for (int i = 0; i< 8; i++)
	{
		pattern2[i].initialize(&ghost2);
		pattern2[i].setActive();
	}

	pattern2[0].setAction(DOWN);
	pattern2[0].setTimeForStep(4);
	pattern2[1].setAction(RIGHT);
	pattern2[1].setTimeForStep(2);
	pattern2[2].setAction(DOWN);
	pattern2[2].setTimeForStep(2);
	pattern2[3].setAction(LEFT);
	pattern2[3].setTimeForStep(5);
	pattern2[4].setAction(UP);
	pattern2[4].setTimeForStep(6);
	pattern2[5].setAction(RIGHT);
	pattern2[5].setTimeForStep(3);
	pattern2[6].setAction(DOWN);
	pattern2[6].setTimeForStep(0);
	pattern2[7].setAction(NA);
	pattern2[7].setTimeForStep(7);

	home.initialize(this, 0,0,0,&ghostTex);
	home.setCenter(VECTOR2(worldSizes[currentState].x/2,worldSizes[currentState].y/2));

	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void FreezeFrame::update()
{
	if(introMusicCoutdown > 0)
		introMusicCoutdown-=frameTime;
	if(introMusicCoutdown < 0)
	{
		introMusicCoutdown = 0;
		audio->playCue(MAIN_LOOP_CUE);
	}


	levelsUpdate();

}



void FreezeFrame::levelsUpdate()
{
	if(input->wasKeyPressed(PAUSE_BUTTON))
	{
		paused = !paused;
	}

	if(input->wasKeyPressed('R'))
		level1Load();

	for(int i = 0 ; i < MAX_PARTICLES; i++)
	{
		particles[i].update(worldFrameTime);
	}

	
	ghost1.update(worldFrameTime);
	ghost2.update(worldFrameTime);
	ghost3.update(worldFrameTime);

	if(!gameOn) return;


	worldFrameTime = frameTime;
	player.update(worldFrameTime);
	updateScreen(player.getCenter());



	
	
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void FreezeFrame::ai()
{
	ghost1.ai(frameTime, player);
	ghost2.ai(frameTime, player);
	ghost3.ai(frameTime, player);

	if (ps1 == 8) {
		ps1 = 0;
	}
	if (ps2 == 8) {
		ps2 = 0;
	}

	if (pattern1[ps1].isFinished()) {
		pattern1[ps1].setActive();
		ps1++;
	}
	if (pattern2[ps2].isFinished()) {
		pattern2[ps2].setActive();
		ps2++;
	}

	pattern1[ps1].update(frameTime);
	pattern2[ps2].update(frameTime);
}

//=============================================================================
// Handle collisions
//=============================================================================
void FreezeFrame::collisions()
{
	if(gameOn) 
	{
		VECTOR2 v;
		for(int i = 0; i < MAX_DOTS;i++)
		{
			if(player.collidesWith(dots[i],v))
			{
				dots[i].setActive(false);
				score++;
				spawnParticleCloud(player.getCenter(),graphicsNS::YELLOW);

				if(score == numDots)gameOn = false;

			}

		}

		if(player.collidesWith(ghost1,v))
		{
			onPlayerDeath();
			numLives--;
			spawnParticleCloud(player.getCenter(),ghost1.getColorFilter());
		}
		
		if(player.collidesWith(ghost2,v))
		{
			onPlayerDeath();
			numLives--;
			spawnParticleCloud(player.getCenter(),ghost2.getColorFilter());
		}
		if(player.collidesWith(ghost3,v))
		{
			onPlayerDeath();
			numLives--;
			spawnParticleCloud(player.getCenter(),ghost3.getColorFilter());
		}
	}
}

//=============================================================================
// Render game items
//=============================================================================
void FreezeFrame::render()
{
	graphics->spriteBegin();                // begin drawing sprites

		levelsRender();

	graphics->spriteEnd();                  // end drawing sprites


}


void FreezeFrame::levelsRender()
{
	background.draw(screenLoc);

	for(int i = 0 ; i < MAX_DOTS; i++)
		dots[i].draw(screenLoc);
	
	ghost1.draw(screenLoc);
	ghost2.draw(screenLoc);
	ghost3.draw(screenLoc);

	for(int i = 0; i < MAX_WALLS; i++)
	{
		walls[i].draw(screenLoc);
	}

	player.draw(screenLoc);

	for(int i = 0 ; i < MAX_PARTICLES; i++)
	{
		particles[i].draw(screenLoc);
	}

	infoText.print("SCORE:"+std::to_string(score),GAME_WIDTH*0.2,0);

	if(!gameOn)
	{
		if(score == numDots)
			win.draw(VECTOR2(0,0));
		else
			lose.draw(VECTOR2(0,0));
	}
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void FreezeFrame::releaseAll()
{
	Game::releaseAll();
	return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void FreezeFrame::resetAll()
{
	Game::resetAll();
	return;
}



void FreezeFrame::level1Load()
{
	currentState = Level1;
	deactivateAll();

	score = 0;
	gameOn = true;

	ghost1.setActive(true);
	ghost1.setCenter(VECTOR2(worldSizes[currentState].x/2,worldSizes[currentState].y/2));

	ghost2.setActive(true);
	ghost2.setCenter(VECTOR2(worldSizes[currentState].x/2,worldSizes[currentState].y/2));

	ghost3.setActive(true);
	ghost3.setCenter(VECTOR2(worldSizes[currentState].x/2,worldSizes[currentState].y/2));

	player.setCenter(getCurrentWorldSize()/4);

	numDots = 0;

	for(int i = 25 ; i < getCurrentWorldSize().x-25;i+=50)
		for(int h= 25 ; h < getCurrentWorldSize().y-25;h+=50)
		{
			spawnDot(VECTOR2(i,h));
			numDots++;
		}

	spawnWall(VECTOR2(0,0),VECTOR2(250,300));
	spawnWall(VECTOR2(getCurrentWorldSize().x-200,0),VECTOR2(200,400));

	spawnWall(VECTOR2(getCurrentWorldSize().x-200,getCurrentWorldSize().y-400),VECTOR2(200,400));
	spawnWall(VECTOR2(0,getCurrentWorldSize().y-300),VECTOR2(250,300));

	spawnWall(VECTOR2(100,500),VECTOR2(500,100));
	spawnWall(VECTOR2(getCurrentWorldSize().x-500,1000),VECTOR2(500,100));

	spawnWall(VECTOR2(100,1500),VECTOR2(500,100));
	spawnWall(VECTOR2(getCurrentWorldSize().x-500,2000),VECTOR2(500,100));

	VECTOR2 v;
	for(int i = 0 ; i < MAX_DOTS;i++)
		for(int j = 0 ; j < MAX_WALLS; j++)
			if(walls[j].collidesWith(dots[i],v))
			{
				dots[i].setActive(false);
				numDots--;
			}
}

void FreezeFrame::level2Load()
{
	currentState = Level2;
	deactivateAll();

	player.setCenter(VECTOR2(100,100));


	spawnWall(VECTOR2(300,0),VECTOR2(250,600));
	spawnWall(VECTOR2(950,0),VECTOR2(600,180));
	spawnWall(VECTOR2(1150,0),VECTOR2(300,300));
	spawnWall(VECTOR2(550,450),VECTOR2(1100,150));
	spawnWall(VECTOR2(1450,600),VECTOR2(200,800));
	spawnWall(VECTOR2(550,0),VECTOR2(600,50));

	spawnWall(VECTOR2(0,900),VECTOR2(300,700));
	spawnWall(VECTOR2(300,1300),VECTOR2(250,300));
	spawnWall(VECTOR2(0,1600),VECTOR2(1650,200));

	spawnWall(VECTOR2(worldSizes[currentState].x - 1650,2450),VECTOR2(1800,200));
	spawnWall(VECTOR2(650,2650),VECTOR2(1000,400));
	spawnWall(VECTOR2(650,3050),VECTOR2(200,850));

	spawnWall(worldSizes[currentState]-VECTOR2(550,550),VECTOR2(550,550));


	spawnGhost(VECTOR2(1400,2000));
	spawnGhost(VECTOR2(1300,2100));
	//spawnGhost(VECTOR2(1350,2200));
	spawnGhost(VECTOR2(1450,2350));

	spawnGhost(VECTOR2(600,2000));
	spawnGhost(VECTOR2(650,2100));
	spawnGhost(VECTOR2(475,2375));

	spawnGhost(VECTOR2(1000,3200));
	spawnGhost(VECTOR2(1875,3000));
	spawnGhost(VECTOR2(1400,4000));

}

void FreezeFrame::level3Load()
{
	
	currentState = Level3;
	deactivateAll();

	
}




void FreezeFrame::updateScreen(VECTOR2 center)
{
	//TODO: nicer screen centering
	screenLoc = VECTOR2(center.x - GAME_WIDTH/2,center.y-GAME_HEIGHT/2);

	if(screenLoc.x<0)screenLoc.x=0;
	if(screenLoc.y<0)screenLoc.y=0;
	if(screenLoc.x+GAME_WIDTH>worldSizes[currentState].x)screenLoc.x=worldSizes[currentState].x-GAME_WIDTH;
	if(screenLoc.y+GAME_HEIGHT>worldSizes[currentState].y)screenLoc.y=worldSizes[currentState].y-GAME_HEIGHT;

}


Particle* FreezeFrame::spawnParticle(VECTOR2 loc,VECTOR2 vel, COLOR_ARGB c)
{
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		if(!particles[i].getActive())
		{
			particles[i].create(loc,vel,c);
			return &particles[i];
		}
	}

	return nullptr;
}


Ghost* FreezeFrame::spawnGhost(VECTOR2 loc)
{
	for(int i = 0; i < MAX_GhostS; i++)
	{
		if(!Ghosts[i].getActive())
		{
			Ghosts[i].create(loc);
			return &Ghosts[i];
		}
	}

	return nullptr;
}

Wall* FreezeFrame::spawnWall(VECTOR2 loc, VECTOR2 size)
{
	for(int i = 0; i < MAX_WALLS; i++)
	{
		if(!walls[i].getActive())
		{
			walls[i].create(loc,size);
			return &walls[i];
		}
	}
	return nullptr;
}

Actor* FreezeFrame::spawnDot(VECTOR2 loc)
{
	for(int i = 0; i < MAX_DOTS; i++)
	{
		if(!dots[i].getActive())
		{
			dots[i].setCenter(loc);
			dots[i].setActive(true);
			return &dots[i];
		}
	}
	return nullptr;
}




void FreezeFrame::spawnParticleCloud(VECTOR2 loc, COLOR_ARGB c)
{
	float dir,spd;
	VECTOR2 v(1,0);
	for(int i = 0; i < NUM_PARTICLES_IN_CLOUD_EFFECT; i++)
	{
		dir = rand01()*2*PI;
		spd = rand01()*particleNS::CLOUD_VEL;
		spawnParticle(loc,rotateVector(v,dir)*spd,c);
	}
}

void FreezeFrame::spawnParticleCone(VECTOR2 loc,float dir, COLOR_ARGB c)
{
	float currentdir,spd;
	VECTOR2 v(1,0);
	for(int i = 0; i < NUM_PARTICLES_IN_CONE_EFFECT; i++)
	{
		currentdir = rand01()*PI/4-PI/8+dir;
		spd = (rand01()/2+0.5)*particleNS::CONE_VEL;
		spawnParticle(loc,rotateVector(v,currentdir)*spd,c);
	}
}



void FreezeFrame::deactivateAll()
{
	for(int i = 0 ; i < MAX_GhostS; i++)
		Ghosts[i].setActive(false);
	for(int i = 0 ; i < MAX_PARTICLES; i++)
		particles[i].setActive(false);
	for(int i = 0; i < MAX_WALLS; i++)
		walls[i].setActive(false);
	for(int i = 0; i < MAX_DOTS; i++)
		dots[i].setActive(false);
}



bool FreezeFrame::getRealEndLoc(VECTOR2 startLoc, VECTOR2 endLoc,Actor*a)
{
	VECTOR2 init = endLoc;
	a->setCenter(endLoc);
	VECTOR2 v;
	for(int i = 0; i < MAX_WALLS; i++)
	{
		if(walls[i].getActive())
		{
			
			if(a->collidesWith(walls[i],v))
			{
				if(startLoc.x<= walls[i].getX())
				{
					endLoc.x = walls[i].getX() - a->getRadius();
				}
				else if(startLoc.y <= walls[i].getY())
				{
					endLoc.y =  walls[i].getY() - a->getRadius();
				}
				else if(startLoc.x >= walls[i].getX()+walls[i].getWidth())
				{
					endLoc.x =  walls[i].getX()+walls[i].getWidth()+ a->getRadius();
				}
				else if(startLoc.y >= walls[i].getY()+walls[i].getHeight())
				{
					endLoc.y = walls[i].getY()+walls[i].getHeight() + a->getRadius();
				}
				break;
			}
		}
	}

	if(endLoc.x < a->getRadius()) endLoc.x = a->getRadius();
	if(endLoc.x > getCurrentWorldSize().x-a->getRadius()) endLoc.x=getCurrentWorldSize().x-a->getRadius();
	if(endLoc.y < a->getRadius()) endLoc.y=a->getRadius();
	if(endLoc.y > getCurrentWorldSize().y-a->getRadius()) endLoc.y=getCurrentWorldSize().y-a->getRadius();

	a->setCenter(endLoc);

	return endLoc != init;
}

void FreezeFrame::onPlayerDeath()
{
		spawnParticleCloud(player.getCenter(),graphicsNS::RED);
		playerDeathCountdown = TIME_UNTIL_RESET;
		audio->playCue(KILL3_CUE);
		numLives--;
		gameOn = false;
		score = 0;
}
