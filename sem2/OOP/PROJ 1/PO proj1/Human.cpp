	#include "Human.h"
action_result Human::action() {

	if (active) {
		if (world->get_turn() - last_activation >= ABILITY_DURTION) {
			active = false;
			world->log("Human disactivates his ability");
		}
	}

	if (!can_activate) {
		if (world->get_turn() - last_activation >= ABILITY_DURTION + ABILITY_COOLDOWN)
			can_activate = true;
	}
	int c = ' ';
	while (c = _getch()) { //c tells it to continue

		if (c == 'x' && can_activate) { 
			active = true;
			world->log("Human activates his ability");
			can_activate = false;
			last_activation = world->get_turn();
		}
		else {
			int dx = 0;
			int dy = 0;
			bool failed = false;

			if (active && (world->get_turn() - last_activation >= ABILITY_DURTION - TURNS_WITH_HALF_CHANCE_TO_ACTIVATE) ){
				if (world->get_random_number() <= CHANCE_FOR_ABILITY_TO_FAIL) {
					failed = true;
					active = false;
				}
			}



			if (c == KEY_UP)
				dy = active ? -2 * HUMAN_RANGE : -HUMAN_RANGE; //conditional expressions are implementation of special ability
			else if (c == KEY_DOWN)
				dy = active ? 2 * HUMAN_RANGE : HUMAN_RANGE;
			else if (c == KEY_LEFT)
				dx = active ? -2 * HUMAN_RANGE : -HUMAN_RANGE;
			else if (c == KEY_RIGHT)
				dx = active ? 2 * HUMAN_RANGE : HUMAN_RANGE;
			else if (c == 's') { // make it possible to save game when human lives 
				world->save_game();
				continue;
			}
			else if (c == 'c')
				break;
			else
				continue;

			if (failed)
				active = true;


			if (x_pos + dx >= 0 && x_pos + dx < world->get_width() && y_pos + dy >= 0 && y_pos + dy < world->get_height()) { // check if generated move is in bounds of map
				move = { x_pos + dx, y_pos + dy };
				return MOVED;
			}
			else return STAYED;
			break;

		}
	}
}