#include "Plant.h"
action_result Plant::action(){
	if (world->get_random_number() < spread_rate) {  // if random number (0;1) is lesser than spread rate (what means that this if will be true with spread_rate probability)
		world->log(display() + "spreaded");
		return SPREADED;
	}
	else return STAYED;
}

 collision_result Plant::collision(Organism* opponent) { //by default plants die on contact with any animal
	world->log(display() + " collides with " + opponent->display());
	world->log(opponent->display() + " kills " + display());
	return DIED;
}