#include "Antelope.h"
collision_result Antelope::hostile_collision(Organism* opponent) {
	if (world->get_random_number() < ANTELOPE_PROBABLITY_TO_ESCAPE) {
		if (world->find_free_adjacent(get_position(), ANTELOPE_RANGE, false) != get_position()) {
			world->log(display() + " escaped");
			return DEFENDER_ESCAPED;
		}
		else
			return Animal::hostile_collision(opponent);
	} //if random number was higher than probability to escape or there was no room to escape, use base class collision mechanism 
	else {
		return Animal::hostile_collision(opponent);
	}
}