#pragma once
#include "Animal.h"

class Human : public Animal
{
private:
	int last_activation;
	bool active;
	bool can_activate;
public:

	Human(int turn, coordinates position, World* world) : Animal(HUMAN_INITIATIVE, HUMAN_STRENGHT, turn, position, world, HUMAN_RANGE, "human", false) {
		last_activation = -1; 
		can_activate = true;
		active = false;
	}
	void draw() {
		cout << HUMAN_CHARACTER;
	}

	
	action_result action(); //override base class method

};

