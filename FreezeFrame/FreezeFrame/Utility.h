#pragma once

#include "constants.h"
#include "graphics.h"

namespace utilityNS{

	VECTOR2 rotateVector(VECTOR2 v, float d);

	float rand01();

	float toPincipleArgument(float angle);

	float getSign(float n);


	const float JIGGLE_FACTOR = 50;
	VECTOR2 jiggleVector(VECTOR2 v);
}