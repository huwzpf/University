#pragma once
#include "Organism.h"
class Animal : public Organism
{
protected:
	int move_range;
	coordinates generate_move();
	coordinates move;
public:
	Animal(int _initiative, int _strenght, int _creation_turn, coordinates _position, World* _world, int _move_range, const char* _name, bool _murders_enrything_around) :
		Organism(_initiative, _strenght, _creation_turn, _position, _world, _name, _murders_enrything_around), move_range(_move_range) {}
	//default metods for animals 
	virtual action_result action(); 
	virtual collision_result collision(Organism* opponent);
	//methods needed for move
	virtual collision_result hostile_collision(Organism* opponent);
	coordinates get_move() const {
		return move;
	}
};

