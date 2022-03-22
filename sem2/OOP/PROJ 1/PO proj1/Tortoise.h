#pragma once
#include "Animal.h"
class Tortoise : public Animal
{
public:
	Tortoise(int turn, coordinates position, World* world) : Animal(TORTOISE_INITIATIVE, TORTOISE_STRENGHT, turn, position, world, TORTOISE_RANGE, "tortoise", false) {}
	void draw() {
		cout << TORTOISE_CHARACTER;
	}
	//override base class methods
	action_result action(); 
	collision_result hostile_collision(Organism* opponent);
};
