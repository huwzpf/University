#pragma once
#include "Organism.h"
class Plant : public Organism
{
protected:
	double spread_rate;
	int spread_attempts;
public:
	Plant(int _strenght, int _creation_turn, coordinates _position, World* _world, double _spread_rate, int _spread_attempts, const char* _name, bool _murders_enrything_around) :
		Organism(0, _strenght, _creation_turn, _position, _world, _name, _murders_enrything_around), spread_rate(_spread_rate), spread_attempts(_spread_attempts) { // by default plant's initiative is 0 since they dont move
	}

	virtual action_result action(); //default action method for plants
	virtual collision_result collision(Organism* opponent); //default collision method

	int get_spread_attempts() const {
		return spread_attempts;
	}
};
