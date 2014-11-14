#include "Actor.h"

namespace particleNS{
	const float MAX_LIFETIME = 0.75;
	const float CLOUD_VEL = 350;
	const float CONE_VEL = 250;
	const COLOR_ARGB SMOKE_COLOR = 0xFF8A775C;
}
class Particle:public Actor
{
private:
	float age;
public:
	Particle();
	virtual void update(float frametime);
	void create(VECTOR2 loc,VECTOR2 vel, COLOR_ARGB c);
	virtual void draw(VECTOR2 screenLoc);
};