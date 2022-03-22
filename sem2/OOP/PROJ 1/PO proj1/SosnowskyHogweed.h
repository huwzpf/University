#pragma once
#include "Plant.h"

class SosnowskyHogweed : public Plant
{
public:
	SosnowskyHogweed(int turn, coordinates position, World* world) : Plant(HOGWEED_STRENGHT, turn, position, world, HOGWEED_SPREAD_RATE, 0, "hogweed", true) {}
	void draw() {
		cout << HOGWEED_CHARACTER;
	}
	//override base class methods
	collision_result collision(Organism* opponent);
};
