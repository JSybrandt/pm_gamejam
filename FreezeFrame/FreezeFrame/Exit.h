#pragma once

#include "Actor.h"

namespace exitNS{};

class Exit:public Actor
{
private:
public:
	Exit():Actor(){
		edge.top = -32;
		edge.left = -64;
		edge.bottom = 32;
		edge.right = 64;
		collisionType = BOX;
	}
};