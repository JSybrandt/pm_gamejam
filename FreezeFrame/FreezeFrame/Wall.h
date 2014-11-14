#pragma once

#include "Actor.h"

namespace wallNS{
};

class Wall:public Actor
{
private:
public:
	Wall(){setActive(false);};
	void create(VECTOR2 loc, VECTOR2 size);
};
