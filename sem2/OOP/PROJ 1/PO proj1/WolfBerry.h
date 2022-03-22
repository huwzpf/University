#pragma once
#include "Plant.h"

class WolfBerry : public Plant
{
public:
	WolfBerry(int turn, coordinates position, World* world) : Plant(WOLFBERRY_STRENGHT, turn, position, world, WOLFBERRY_SPREAD_RATE, 0, "wolfberry", false) {}
	void draw() {
		cout << WOLFBERRY_CHARACTER;
	}
	collision_result collision(Organism* opponent); //override base class method
};
