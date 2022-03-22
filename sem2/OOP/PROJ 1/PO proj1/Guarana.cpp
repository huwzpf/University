#include "Guarana.h"

collision_result Guarana::collision(Organism* opponent) {
	opponent->change_strenght(GUARANA_EATING_BOOST); //guaran grants strenght boost to whoever ate it 
	world->log(opponent->get_species_name() + (string)" gains strenght ");
	return Plant::collision(opponent); // call base class collision, guarana gets eaten
}