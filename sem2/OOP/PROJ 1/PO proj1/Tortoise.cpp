#include "Tortoise.h"

action_result Tortoise::action() {
	if (world->get_random_number() < TORTOISE_PROBABILITY_TO_MOVE) { // tortoise moves only with some probablility
		return Animal::action();
	}
	else
		return STAYED;
}

collision_result Tortoise::hostile_collision(Organism* opponent) { // tortoise scares off enemies below certain strenght
	if (opponent->get_strenght() < TORTOISE_DEFENSE_CAPABILITY) {
		world->log(display() + " defends himself");
		return ATTACKER_ESCAPED;
	}
	else { // if opponent's strenght was to high, call base class method (tortoise dies anyway)
		return Animal::hostile_collision(opponent);
	}
}