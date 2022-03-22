#pragma once
#include "Animal.h"
class Fox : public Animal
{
public:
	Fox(int turn, coordinates position, World* world) : Animal(FOX_INITIATIVE, FOX_STRENGHT, turn, position, world, FOX_RANGE, "fox", false) {}
	void draw() {
		cout << FOX_CHARACTER;
	}
	action_result acion(); //override base class action method
};


 