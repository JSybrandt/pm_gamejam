#include"Player.h"
#include"freezeframe.h"

using namespace playerNS;

Player::Player():Actor(){
	//TODO: tighter collision
	/*edge.top = -32;
	edge.bottom = 32;
	edge.left = -12;
	edge.right = 12;
	collisionType = BOX;*/

	radius = 32;
	collisionType = CIRCLE;

	currentTimeMultiplier = 1;
	desiredTimeMultiplier = STANDING_TM;

	timeUntilWallCollision = 1;

	showUseMessage = false;

	mouseDown = false;

};
Player::~Player(){};

bool Player::initialize(FreezeFrame * g, Controls c,TextDX* t, int width, int height, int ncols, TextureManager *playerTM)
{
	game = g;
	controls = c;
	text = t;

	velocity = VECTOR2(0,0);


	bool result =  Actor::initialize(g,width,height,ncols,playerTM);
	return result;
}

void Player::draw(VECTOR2 screenLoc)
{
	if(getActive())
	{

		
		Actor::draw(screenLoc);

		
		
	}
}

void Player::update(float &frametime)
{
	if(getActive())
	{
		

		VECTOR2 inputDir(0,0);

		if(input->isKeyDown(controls.up))
			inputDir.y=-1;
		if(input->isKeyDown(controls.down))
			inputDir.y=1;
		if(input->isKeyDown(controls.left))
			inputDir.x=-1;
		if(input->isKeyDown(controls.right))
			inputDir.x=1;
		
		D3DXVec2Normalize(&inputDir,&inputDir);

		if(inputDir!= VECTOR2(0,0)&&!(inputDir.x!=0&&inputDir.y!=0))
		{
			velocity = inputDir * RUN_SPEED;
			setRadians(atan2(inputDir.y,inputDir.x));
		}

		
		
		VECTOR2 intendedEnd = getCenter()+velocity*frametime;

		if(game->getRealEndLoc(getCenter(),intendedEnd,this))
			velocity = VECTOR2(0,0);
		

		Image::update(frametime);
			
	}
}

