#pragma once
#include "Animal.h"

class Sheep : public Animal
{
public:
	Sheep(int turn, coordinates position, World* world) : Animal(SHEEP_INITIATIVE, SHEEP_STRENGHT, turn, position, world, SHEEP_RANGE, "sheep", false) {}
	void draw() {
		cout << SHEEP_CHARACTER;
	}
};

