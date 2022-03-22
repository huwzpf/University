#include "Fox.h"
action_result Fox::acion() {
	coordinates possible_move = generate_move(); //fox never moves to field with stronger enemy
	Organism* possible_opponent = world->get_map_field(possible_move.x, possible_move.y);
	if (possible_opponent != nullptr && possible_opponent->get_strenght() >= strenght) {
		return STAYED;
	}
	else {
		move = possible_move;
		return MOVED;
	}

}