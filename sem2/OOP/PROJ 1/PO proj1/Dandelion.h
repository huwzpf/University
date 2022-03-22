#pragma once
#include "Plant.h"
class Dandelion : public Plant
{
public:
	Dandelion(int turn, coordinates position, World* world) : Plant(DANDELION_STRENGHT, turn, position, world, DANDELION_SPREAD_RATE,DANDELION_SPREAD_ATTEMPTS, "dandelion", false) {}
	void draw() {
		cout << DANDELION_CHARACTER;
	}
};

