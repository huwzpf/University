#pragma once
#include "Animal.h"
class Antelope : public Animal
{
public:
	Antelope(int turn, coordinates position, World* world) : Animal(ANTELOPE_INITIATIVE, ANTELOPE_STRENGHT, turn, position, world, ANTELOPE_RANGE, "antelope", false) {}
	void draw() {
		cout << ANTELOPE_CHARACTER;
	}
	collision_result hostile_collision(Organism* opponent); //override base class collision
};
