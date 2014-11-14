#pragma once

#include "Actor.h"

namespace itemNS{
	const float GUN_RADIUS = 45;
};

//TODO: ITEM SWIRLYING BACKGROUND 


class Item:public Actor
{
public:

static enum ItemType
{
	WEAPON,
	SIZE //must be last
};

	Item(){radius = itemNS::GUN_RADIUS;}

	void create(VECTOR2 loc, ItemType t){
		setActive(true);
		setCenter(loc);
		type = t;
	}

	//todo draw based on type

	ItemType getType(){return type;}

private:
	ItemType type;

	


};