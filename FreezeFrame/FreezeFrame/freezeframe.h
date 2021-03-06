// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 spacewar.h v1.0

#ifndef _REDSPACE_H             // prevent multiple definitions if this 
#define _REDSPACE_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include <string>
using std::string;
#include <ctime>


#include "Controls.h"
#include "Actor.h"
#include "Player.h"
#include "Ghost.h"
#include "Particle.h"
#include "Wall.h"
#include "patternStep.h"

namespace freezeFrameNS
{
	const int MAX_GhostS = 100;
	const int MAX_TURRETS = 100;
	const int MAX_PLAYER_BULLETS = 100;
	const int MAX_ENEMY_BULLETS = 1000;
	const int MAX_PARTICLES = 10000;
	const int MAX_SCENERY = 1000;
	const int MAX_WALLS = 100;
	const int MAX_MINES = 100;
	const int MAX_ITEMS = 100;

	const int NUM_MENU_OPTIONS = 4;
	const float MENU_ITEM_SPEED = 300;
	const float MENU_ITEM_DELAY = 0.25;

	const int NUM_PARTICLES_IN_CLOUD_EFFECT = 200;
	const int NUM_PARTICLES_IN_CONE_EFFECT = 100;

	const float TIME_UNTIL_RESET = 1;

	const UCHAR PAUSE_BUTTON = 'P';

	const float TIME_TEXT_ON_SCREEN = 3;

	const int STARTING_LIVES = 3;

	const float INTRO_MUSIC_COUNTDOWN = 6;

	const int MAX_DOTS = 1000;
}


using namespace freezeFrameNS;
using namespace utilityNS;

//=============================================================================
// Create game class
//=============================================================================
class FreezeFrame : public Game
{
private:

	enum GameState{
	TitleScreen,
	Level1,
	Level2,
	Level3,
	FeelingLucky,
	RestartScreen,
	SIZE //THIS MUST BE THE LAST ELEMENT
};

    // variables
	TextureManager backgroundTex;   
	TextureManager baseTex;
	TextureManager manTex;
	
	TextureManager particleTex;
	
	TextureManager wallTex;
	
	TextureManager ghostTex;
	TextureManager dotTex;
	TextureManager winTex;
	TextureManager loseTex;

	Image win,lose;

	TextDX infoText;

	
	Image   background;         // backdrop image

	Ghost Ghosts[MAX_GhostS];
	Actor dots[MAX_DOTS];
	Particle particles[MAX_PARTICLES];
	Wall walls[MAX_WALLS];
	Actor home;

	PatternStep pattern1[8];
	PatternStep pattern2[8];
	PatternStep pattern3[2];

	Ghost ghost1;
	Ghost ghost2;
	Ghost ghost3;

	Controls P1Controls;
	Player player;

	float worldFrameTime;
	float playerDeathCountdown;
	int ps1;
	int ps2;
	int ps3;
	

	VECTOR2 screenLoc;

	VECTOR2 * worldSizes; //array of sizes per level

	GameState currentState;

	bool paused;
	bool showCredits;

	float textCooldown;
	string statusString;

	int numLives;

	float introMusicCoutdown; 

	int score; 

	bool gameOn;

	int numDots;

public:
	// Constructor
    FreezeFrame();

    // Destructor
    virtual ~FreezeFrame();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();


	//places the screen so the selected location is in the middle area (might not center)
	void updateScreen(VECTOR2 center); 

	Actor& getHome() {return home;}	
	Wall* spawnWall(VECTOR2 loc, VECTOR2 size);
	Actor* spawnDot(VECTOR2 loc);
	Ghost* spawnGhost(VECTOR2 loc);//TODO: add stuff for AI
	void spawnParticleCloud(VECTOR2 loc, COLOR_ARGB c);
	void spawnParticleCone(VECTOR2 loc, float dir, COLOR_ARGB c);
	Particle* spawnParticle(VECTOR2 loc,VECTOR2 vel, COLOR_ARGB c);


	void level1Load();
	void level2Load();
	void level3Load();

	void levelsUpdate();
	void levelsRender();


	void deactivateAll();

	VECTOR2 getMouseInWorld(){
		VECTOR2 mouse(input->getMouseX(),input->getMouseY());
		return screenLoc + mouse;
	}

	VECTOR2 getPlayerLoc(){
		return player.getCenter();
	}

	VECTOR2 getCurrentWorldSize(){return worldSizes[currentState];}

	bool getRealEndLoc(VECTOR2 startLoc, VECTOR2 endLoc,Actor* a);

	void onPlayerDeath();



	bool l2pCheat;
	bool infAmmoCheat;
};

#endif
