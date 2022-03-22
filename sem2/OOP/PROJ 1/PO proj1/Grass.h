#pragma once
#include "Plant.h"
class Grass : public Plant
{
public:
	Grass(int turn, coordinates position, World* world) : Plant(GRASS_STRENGHT, turn, position, world, GRASS_SPREAD_RATE,0, "grass", false) {}
	void draw() {
		cout << GRASS_CHARACTER;
	}
};

