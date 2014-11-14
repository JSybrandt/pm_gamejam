#include "Wall.h"

void Wall::create(VECTOR2 loc, VECTOR2 size)
{
	setActive(true);

	setX(loc.x);setY(loc.y);

	RECT r;
	r.top=0;r.left=0;
	r.right=size.x;r.bottom=size.y;
	setSpriteDataRect(r);

	spriteData.width = size.x;
	spriteData.height = size.y;

	edge.top= -size.y/2;
	edge.bottom= size.y/2;
	edge.left = -size.x/2;
	edge.right = size.x/2;

	collisionType = BOX;

	update(0);
}