#include "SosnowskyHogweed.h"

collision_result SosnowskyHogweed::collision(Organism* opponent) {
	world->log(opponent->get_species_name() + (string)" got too close to hogweed and died"); //whatever gets close to hogweed dies
	return KILLED_OPPONENT;
}
