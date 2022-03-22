#include "WolfBerry.h"

collision_result WolfBerry::collision(Organism* opponent) { //both berry and whoever ate it die 
	world->log(opponent->get_species_name() + (string)" ate wolfberry and died");
	return BOTH_DIED;
}
