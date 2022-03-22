#pragma once
#include "Animal.h"
class Wolf : public Animal
{
public:
	Wolf(int turn, coordinates position, World* world): Animal(WOLF_INITIATIVE, WOLF_STRENGHT,turn,position,world, WOLF_RANGE, "wolf", false) {}
	void draw() {
		cout << WOLF_CHARACTER;
	}
};

