// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly with modifications by Caelan Mayberry and Justin Sybrandt
// Chapter 6 Actor.h v1.0

#ifndef _Actor_H               // Prevent multiple definitions if this 
#define _Actor_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "game.h"
#include "Utility.h"

namespace ActorNS
{
    enum COLLISION_TYPE {NONE, CIRCLE, BOX, ROTATED_BOX};
    const float GRAVITY = 6.67428e-11f;         // gravitational constant
}

class Actor : public Image
{
    // Actor properties
  protected:
	Image img;
    COLLISION_TYPE collisionType;
    VECTOR2 center;         // center of Actor
    float   radius;         // radius of collision circle
	float	perimeter;
    VECTOR2 distSquared;    // used for calculating circle collision
    float   sumRadiiSquared;
    // edge specifies the collision box relative to the center of the Actor.
    // left and top are typically negative numbers
    RECT    edge;           // for BOX and ROTATED_BOX collision detection
    VECTOR2 corners[4];     // for ROTATED_BOX collision detection
    VECTOR2 edge01,edge03;  // edges used for projection
    float   edge01Min, edge01Max, edge03Min, edge03Max; // min and max projections
    VECTOR2 velocity;       // velocity
    VECTOR2 deltaV;         // added to velocity during next call to update()
    float   mass;           // Mass of Actor
	float	resistance;		// Resistance to movement to help will planets
    float   health;         // health 0 to 100
    float   rr;             // Radius squared variable
    float   force;          // Force of gravity
    float   gravity;        // gravitational constant of the game universe
    Input   *input;         // pointer to the input system
    Audio   *audio;         // pointer to audio system
    HRESULT hr;             // standard return type
    bool    active;         // only active entities may collide
    bool    rotatedBoxReady;    // true when rotated collision box is ready

    // --- The following functions are protected because they are not intended to be
    // --- called from outside the class.
    // Circular collision detection 
    // Pre: &ent = Other Actor
    // Post: &collisionVector contains collision vector
    virtual bool collideCircle(Actor &ent, VECTOR2 &collisionVector);
    // Axis aligned box collision detection
    // Pre: &ent = Other Actor
    // Post: &collisionVector contains collision vector
    virtual bool collideBox(Actor &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection between boxes
    // Pre: &ent = Other Actor
    // Post: &collisionVector contains collision vector
    virtual bool collideRotatedBox(Actor &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection between box and circle
    // Pre: &ent = Other Actor
    // Post: &collisionVector contains collision vector
    virtual bool collideRotatedBoxCircle(Actor &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection helper functions
    void computeRotatedBox();
    bool projectionsOverlap(Actor &ent);
    bool collideCornerCircle(VECTOR2 corner, Actor &ent, VECTOR2 &collisionVector);

  public:
    // Constructor
    Actor();
    // Destructor
    virtual ~Actor() {}

    ////////////////////////////////////////
    //           Get functions            //
    ////////////////////////////////////////

    // Return center of scaled Actor as screen x,y.
    virtual const VECTOR2 getCenter()   
    {
        return VECTOR2(getCenterX(),getCenterY());
    }

    // Return radius of collision circle.
    virtual float getRadius() const     {return radius;}

    // Return RECT structure used for BOX and ROTATED_BOX collision detection.
    virtual const RECT& getEdge() const {return edge;}

    // Return corner c of ROTATED_BOX
    virtual const VECTOR2* getCorner(UINT c) const {
        if(c>=4) 
            c=0;
        return &corners[c]; 
    }

    // Return velocity vector.
    virtual const VECTOR2 getVelocity() const {return velocity;}

    // Return active.
    virtual bool  getActive()         const {return active;}

    // Return mass.
    virtual float getMass()           const {return mass;}

    // Return gravitational constant.
    virtual float getGravity()        const {return gravity;}

    // Return health;
    virtual float getHealth()         const {return health;}

    // Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
    virtual COLLISION_TYPE getCollisionType() {return collisionType;}

	virtual float getResistance()		const {return resistance;}

    ////////////////////////////////////////
    //           Set functions            //
    ////////////////////////////////////////

	virtual void setLeft(float l){setX(l);}
	virtual void setRight(float r){setX(r-getWidth());}
	virtual void setTop(float t){setY(t);}
	virtual void setBot(float b){setY(b-getHeight());}

    // Set velocity.
    virtual void  setVelocity(VECTOR2 v)    {velocity = v;}
	virtual void  setVelocity(float x, float y)    {velocity.x = x; velocity.y = y;}

    // Set delta velocity. Added to velocity in update().
    virtual void  setDeltaV(VECTOR2 dv)     {deltaV = dv;}

    // Set active.
    virtual void  setActive(bool a)         {active = a;}

    // Set health.
    virtual void setHealth(float h)         {health = h;}

    // Set mass.
    virtual void  setMass(float m)          {mass = m;}

    // Set gravitational constant. Default is 6.67428e-11
    virtual void  setGravity(float g)       {gravity = g;}

    // Set radius of collision circle.
    virtual void setCollisionRadius(float r)    {radius = r;}

	virtual void setResistance(float r)		{resistance = r;}

	//void setPositionX(float pos) {spriteData.x = pos;}
	//void setPositionY(float pos) {spriteData.y = pos;} 
	//void incPositionX(float pos) {spriteData.x += pos;}
	//void incPositionY(float pos) {spriteData.y += pos;}
	//void incPosition(VECTOR2 pos) {spriteData.x += pos.x; spriteData.y += pos.y;}

	//virtual void draw() {};

    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////

    // Update Actor.
    // typically called once per frame
    // frameTime is used to regulate the speed of movement and animation
    virtual void update(float frameTime);

    // Initialize Actor
    // Pre: *gamePtr = pointer to Game object
    //      width = width of Image in pixels  (0 = use full texture width)
    //      height = height of Image in pixels (0 = use full texture height)
    //      ncols = number of columns in texture (1 to n) (0 same as 1)
    //      *textureM = pointer to TextureManager object
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    // Activate Actor.
    virtual void activate();

    // Empty ai function to allow Actor objects to be instantiated.
    virtual void ai(float frameTime, Actor &ent);

    // Is this Actor outside the specified rectangle?
    virtual bool outsideRect(RECT rect);

    // Does this Actor collide with ent?
    virtual bool collidesWith(Actor &ent, VECTOR2 &collisionVector);

    // Damage this Actor with weapon.
    virtual void damage(int weapon);

    // Actor bounces after collision with other Actor
    void bounce(VECTOR2 &collisionVector, Actor &ent);

    // Adds the gravitational force to the velocity vector of this Actor
    void virtual gravityForce(Actor *other, float frameTime);
	
	
	void virtual explosion();

	void virtual draw(VECTOR2 screenLoc, COLOR_ARGB c = graphicsNS::FILTER)
	{
		if(getActive()) Image::draw(screenLoc,c);
	}

};

#endif
