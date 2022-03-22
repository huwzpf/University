#include "Animal.h"

action_result Animal::action() {
	move = generate_move();
	return MOVED;
}

coordinates Animal::generate_move() { //default move function 
	direction dir = (direction)(rand() / (RAND_MAX / 4)); // there are 4 possible moving directions represented by int values 0-3

	switch (dir) { 
	case RIGHT:
		if (x_pos + move_range < world->get_width())
			return { x_pos + move_range, y_pos };
		else
			return { x_pos, y_pos };

		break;
	case LEFT:
		if (x_pos - move_range >= 0)
			return { x_pos - move_range, y_pos };
		else
			return { x_pos, y_pos };

		break;
	case UP:
		if (y_pos - move_range >= 0)
			return { x_pos, y_pos - move_range };
		else
			return { x_pos, y_pos };

		break;
	case DOWN:
		if (y_pos + move_range < world->get_height())
			return { x_pos, y_pos + move_range };
		else
			return { x_pos, y_pos };

		break;
	default:
		return { x_pos, y_pos };

	}
}

collision_result Animal::collision(Organism* opponent) { // default function handles animals breeding (since it will be same for all animals )and calls hostile collision method 
	if (this->species_name == opponent->get_species_name()) {
		if (opponent == this) // make sure animal doesnt try to breed with itself
			throw(logic_error("breeding with itself"));
		world->log(display() + " breeds with " + opponent->display());
		return BREEDED;
	}
	else {
		world->log(display() + " collides with " + opponent->display());
		return hostile_collision(opponent);
	}
	
}

collision_result Animal::hostile_collision(Organism* opponent)
{
	if (opponent->get_strenght() < strenght || (opponent->get_strenght() == strenght && opponent->get_age() > creation_turn)) { //base method for solving collision - stronger one survives
		world->log(display() + " kills " + opponent->display());
		return KILLED_OPPONENT;
	}
		
	else {
		world->log(opponent->display() + " kills " + display());
		return DIED;
	}
}

