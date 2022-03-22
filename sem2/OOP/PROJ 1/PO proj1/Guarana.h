#pragma once
#include "Plant.h"

class Guarana : public Plant
{
public:
	Guarana(int turn, coordinates position, World* world) : Plant(GUARANA_STRENGHT, turn, position, world, GUARANA_SPREAD_RATE, 0, "guarana", false) {}
	void draw() {
		cout << GUARANA_CHARACTER;
	}
	collision_result collision(Organism* opponent); // override base class method
};

